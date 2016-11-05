<?php

/**
 * @file WPS payment method controller
 * Only deals with express checkout action
 */

class PaymentPaypalExpressCheckoutMethodController extends PaymentPaypalMethodController {

  public $payment_method_configuration_form_elements_callback = 'payment_paypal_express_checkout_method_payment_method_configuration_form_elements';

  public $payment_configuration_form_elements_callback = 'payment_paypal_express_checkout_method_payment_configuration_form_elements';

  function __construct() {
    $this->title = t('Paypal Express checkout');
    $this->description = t("Use paypal express checkout");

    parent::__construct();
  }

  /**
   * Implements PaymentMethodController::execute().
   */
  function execute(Payment $payment) {
    $this->redirect($payment);
  }

  /**
   * Redirect on the offsite payment gateway.
   *
   * @param $payment
   *   A payment object to redirect
   */
  private function redirect(Payment $payment) {

    //Init remote checkout to retrieve a valid token
    $return = $this->setExpressCheckout($payment);

    //Error occured
    if (empty($return['success']) || $return['success'] !== TRUE
      || empty($payment->method->controller_data['endpoint'])
      || empty($payment->method->controller_data['response']['TOKEN'])) {

      $msg_error = (!empty($return['message'])) ? $return['message'] : t('Endpoint and token are missing. Could not redirect current user.');
      //Throw error to caller will set payment in failed status
      throw new PaymentValidationException($msg_error);
    }

    //Prepare redirect url args
    $endpoint = $payment->method->controller_data['endpoint'];
    $token = $payment->method->controller_data['response']['TOKEN'];

    //Retrieve redirect url
    $path_args = $this->getExternalRedirectUrl($endpoint, $token);

    //Change payment status and save it
    $payment->setStatus(new PaymentStatusItem(PAYMENT_PAYPAL_STATUS_REDIRECTED));
    entity_save('payment', $payment);

    //Redirect user and skip all post process
    drupal_goto($path_args['url'], $path_args['options']);
  }

  /**
   * Private function to set up remote payment.
   * @param $payment
   *   Payment object used
   *
   * @return
   *   An array containing keys :
   *   - success : boolean
   *   - message : a error message string if any
   *   Payment object provided would be also altered
   *   to store paypal response in case of success.
   *   These data are available into method controller data
   *   of this payment object.
   *
   * @see PaymentPaypalExpressCheckoutMethodController::redirect()
   */
  private function setExpressCheckout(Payment $payment) {
    global $user;

    //Array result returned to caller
    $return = array(
      'success' => FALSE,
      'message' => '',
    );

    //Build POST datas.
    $data = array(
      'USER' => $payment->method->controller_data['api_username'],
      'PWD' => payment_paypal_get_decrypted_passwd($payment->method->controller_data['api_passwd']),
      'SIGNATURE' => $payment->method->controller_data['api_signature'],
      'METHOD' => 'SetExpressCheckout',
      'VERSION' => PAYMENT_PAYPAL_API_VERSION,
      'PAYMENTREQUEST_0_CURRENCYCODE' => $payment->currency_code,
      'PAYMENTREQUEST_0_PAYMENTACTION' => 'Sale',
      'PAYMENTREQUEST_0_DESC' => $payment->description,
      'PAYMENTREQUEST_0_INVNUM' => $this->getInvoiceID($payment),
      'RETURNURL' => url(PAYMENT_PAYPAL_RETURN_URL . '/' . $payment->pid, array('absolute' => TRUE)),
      'CANCELURL' => url(PAYMENT_PAYPAL_CANCEL_URL . '/' . $payment->pid, array('absolute' => TRUE)),
      'LOCALECODE' => strtoupper(variable_get('site_default_country', 'US')),
      'EMAIL' => (!empty($user->mail)) ? $user->mail : '',
      //Force some defaults settings for the moment
      'CHANNELTYPE' => 'Merchant',
      'GIFTMESSAGEENABLE' => 0,
      'GIFTRECEIPTENABLE' => 0,
      'GIFTWRAPNAME' => 0,
      'BUYEREMAILOPTINENABLE' => 0,
      'SURVEYENABLE' => 0,
    );
    //Merge with settings from method configuration
    $data += $payment->method->controller_data['settings']['express_checkout'];

    //Complete data request with line items
    $this->preparePaypalPaymentRequestData($payment, $data);

    //Build request options.
    $options = array(
      'method' => 'POST',
      'data' => drupal_http_build_query($data),
    );

    //Send datas to Paypal server.
    $result = drupal_http_request($payment->method->controller_data['endpoint'], $options);

    $return = $this->parseHTTPResponse($result, $payment);

    if (!empty($return['response'])) {

      $response = $return['response'];

      $ack = $response['ACK'];
      $correlation_id = $response['CORRELATIONID'];
      $timestamp = strtotime($response['TIMESTAMP']);
      $token = (!empty($response['TOKEN'])) ? $response['TOKEN'] : '';
      $data = array(
        'op' => 'setExpressCheckout',
        'token' => $token,
        'invnum' => $this->getInvoiceID($payment),
      );

      if (strtolower($ack) === 'success' || strtolower($ack) === 'successwithwarning') {
        //Store response into controller data payment method
        $payment->method->controller_data['response'] = $response;
        $return['success'] = TRUE;
      }
      else {
        $return['success'] = FALSE;

        $errors = $this->paypalErrorLog($payment, $response);
        //Store errors into paypal payment data
        $data['errors'] = $errors['paypal_errors'];
        $return['message'] = $errors['message'];
      }

      //Log paypal payment
      $this->log($payment, $correlation_id, '', $ack, $timestamp, $data);
    }

    return $return;
  }

  /**
   * Function helper to construct
   * external redirect URL to paypal payment gateway
   *
   * @param $endpoint
   *   Endpoint used by payment method
   * @param $token
   *   Token returned by paypal server
   *
   * @return
   *   An array keyed by :
   *     - url
   *     - options
   *   as expected by url() function.
   */
  private function getExternalRedirectUrl($endpoint, $token) {
    $base_url = '';
    $options = array(
      'external' => TRUE,
      'query' => array(
        'cmd' => '_express-checkout',
        'token' => $token,
      ),
    );

    //use sandbox gateway
    if ($endpoint === PAYMENT_PAYPAL_ENDPOINT_SANDBOX_URL) {
      $base_url = PAYMENT_PAYPAL_REDIRECT_SANDBOX_URL;
    }
    //Otherwise, use live gateway
    else {
      $base_url = PAYMENT_PAYPAL_REDIRECT_LIVE_URL;
    }

    return array(
      'url' => $base_url,
      'options' => $options,
    );
  }

  /**
   * Public function to complete a remote payment previously executed.
   * It will execute two methods to validate the remote payment :
   *  - retrieve payment previously initialize
   *  - validate this previous payment
   *
   * @param $payment
   *   Payment object previously executed
   *
   * @throw
   *   PaymentException if error occured, and a new failed payment status will
   *   be added to the payment object given.
   */
  public function completePayment(Payment $payment) {

    if (payment_status_is_or_has_ancestor($payment->getStatus()->status, PAYMENT_PAYPAL_STATUS_RETURNED)) {

      //Increase timeout as advice by Paypal API
      $this->setTimeout(FALSE);

      //Two methods are done to valid a remote payment :
      // - getExpressCheckoutDetails : retrieve paypal payment from paypal server
      // - doExpressCheckoutPayment : a kind of "capture" payment, where we
      //   complete the remote payment.
      $methods = array('getExpressCheckoutDetails', 'doExpressCheckoutPayment');
      foreach ($methods as $method) {

        $return = $this->{$method}($payment);

        //If error occured, stop process and set payment in fail status
        if ($return['success'] !== TRUE) {

          $payment->setStatus(new PaymentStatusItem(PAYMENT_STATUS_FAILED));

          $msg = (!empty($return['message'])) ? $return['message'] : t('An error occured while trying to execute payment method paypal "@method" with @action action on payment @pid.', array('@method' => $payment->method->name, '@action' => $method, '@pid' => $payment->pid));
          throw new PaymentException($msg);
        }
      }

      //Reset timeout
      $this->setTimeout(TRUE);

      //Set payment as succeed
      $payment->setStatus(new PaymentStatusItem(PAYMENT_STATUS_SUCCESS));

    }
    //Unexpected status, add a fail status if not already
    elseif (!payment_status_is_or_has_ancestor($payment->getStatus()->status, PAYMENT_STATUS_FAILED)) {

      $payment->setStatus(new PaymentStatusItem(PAYMENT_STATUS_FAILED));
      $msg = t('Wrong payment status receive for payment @pid while trying to complete remote payment : @status', array('@pid' => $payment->pid, '@status' => $payment->getStatus()->status));
      throw new PaymentException($msg);
    }
  }

  private function getExpressCheckoutDetails(Payment $payment) {

    //Array result returned to caller
    $return = array(
      'success' => FALSE,
      'message' => '',
    );

    //Get param args returned by paypal
    $token = $_GET['token'];
    $payer_id = $_GET['PayerID'];

    if (!$token || !$payer_id) {
      $return['message'] = t('Unable to retrieve token and payer id from returned url.');
      $return['success'] = FALSE;
      return $return;
    }

    //Get paypal payment and check token validity
    $paypal_payments = payment_paypal_paypal_payment_load($payment->pid, NULL, TRUE);
    $paypal_payment = array_shift($paypal_payments);
    if (!$paypal_payment || empty($paypal_payment->data['token']) || $paypal_payment->data['token'] !== $token) {
      $return['message'] = t('Unable to load token or invalid token retrieve from URL.');
      $return['success'] = FALSE;
      return $return;
    }

    //Store invoice unique ID into controller
    $payment->method->controller_data['invnum'] = $paypal_payment->data['invnum'];

    //Build POST datas.
    $data = array(
      'USER' => $payment->method->controller_data['api_username'],
      'PWD' => payment_paypal_get_decrypted_passwd($payment->method->controller_data['api_passwd']),
      'SIGNATURE' => $payment->method->controller_data['api_signature'],
      'METHOD' => 'GetExpressCheckoutDetails',
      'VERSION' => PAYMENT_PAYPAL_API_VERSION,
      'TOKEN' => $token,
      'INVNUM' => $paypal_payment->data['invnum'],
    );
    $this->preparePaypalPaymentRequestData($payment, $data);

    //Build request options.
    $options = array(
      'method' => 'POST',
      'data' => drupal_http_build_query($data),
    );

    //Send datas to Paypal server.
    $result = drupal_http_request($payment->method->controller_data['endpoint'], $options);

    $return = $this->parseHTTPResponse($result, $payment);

    if (!empty($return['response'])) {

      //We don't check if token and payer ID matche those returned by paypal
      //as paypal is supposed to deals with these kind of errors.
      $response = $return['response'];

      $ack = $response['ACK'];
      $correlation_id = $response['CORRELATIONID'];
      //Transaction haven't been done yet
      $transaction_id = '';
      $timestamp = strtotime($response['TIMESTAMP']);
      $checkout_status = $response['CHECKOUTSTATUS'];

      $payer_id = $response['PAYERID'];
      $payer_status = $response['PAYERSTATUS'];

      //Add also any other additional data
      $data = array(
        'op' => 'getExpressCheckoutDetails',
        'token' => $token,
        'payer_id' => $payer_id,
        'payer_status' => $payer_status,
        'checkout_status' => $checkout_status,
        'invnum' => $paypal_payment->data['invnum'],
      );

      if (strtolower($ack) === 'success' || strtolower($ack) === 'successwithwarning') {
        //Store response into controller data payment method
        $payment->method->controller_data['response'] = $response;
        $return['success'] = TRUE;
      }
      else {
        $return['success'] = FALSE;

        $errors = $this->paypalErrorLog($payment, $response);
        //Store errors into paypal payments data
        $data['errors'] = $errors['paypal_errors'];
        $return['message'] = $errors['message'];
      }

      //Log paypal payment
      $this->log($payment, $correlation_id, $transaction_id, $ack, $timestamp, $data);
    }

    return $return;
  }

  private function doExpressCheckoutPayment(Payment $payment) {

    //Array result returned to caller
    $return = array(
      'success' => FALSE,
      'message' => '',
    );

    //Get payer ID from URL
    $payer_id = $_GET['PayerID'];

    //Token and payer ID haven't been added successfully from getExpressCheckoutDetails ?
    if (empty($payment->method->controller_data['response']['TOKEN']) || empty($payment->method->controller_data['response']['PAYERID'])) {
      $return['success'] = FALSE;
      $return['message'] = t('Unable to retrieve token or payerid previously returned from paypal server.');
      return $return;
    }
    //Payer ID from URL and returnded by paypal server don't match ?
    elseif ($payer_id !== $payment->method->controller_data['response']['PAYERID']) {
      $return['success'] = FALSE;
      $return['message'] = t('Invalid Payer ID. Payer ID from URL and returned from paypal server don\'t match.');
      return $return;
    }

    //Build POST datas.
    $data = array(
      'USER' => $payment->method->controller_data['api_username'],
      'PWD' => payment_paypal_get_decrypted_passwd($payment->method->controller_data['api_passwd']),
      'SIGNATURE' => $payment->method->controller_data['api_signature'],
      'METHOD' => 'DoExpressCheckoutPayment',
      'VERSION' => PAYMENT_PAYPAL_API_VERSION,
      'TOKEN' => $payment->method->controller_data['response']['TOKEN'],
      'PAYERID' => $payment->method->controller_data['response']['PAYERID'],
      'PAYMENTREQUEST_0_CURRENCYCODE' => $payment->currency_code,
      'PAYMENTREQUEST_0_PAYMENTACTION' => 'Sale',
      'PAYMENTREQUEST_0_DESC' => $payment->description,
      'PAYMENTREQUEST_0_INVNUM' => $payment->method->controller_data['invnum'],
    );
    $this->preparePaypalPaymentRequestData($payment, $data);

    //Build request options.
    $options = array(
      'method' => 'POST',
      'data' => drupal_http_build_query($data),
    );

    //Send datas to Paypal server.
    $result = drupal_http_request($payment->method->controller_data['endpoint'], $options);

    $return = $this->parseHTTPResponse($result, $payment);

    if (!empty($return['response'])) {

      $response = $return['response'];

      $ack = $response['ACK'];
      $correlation_id = $response['CORRELATIONID'];
      $transaction_id = $response['PAYMENTINFO_0_TRANSACTIONID'];
      $timestamp = strtotime($response['TIMESTAMP']);

      $receipt_id = (array_key_exists('PAYMENTINFO_0_RECEIPTID', $response)) ? $response['PAYMENTINFO_0_RECEIPTID'] : '';
      $order_time = $response['PAYMENTINFO_0_ORDERTIME'];
      $payment_status = $response['PAYMENTINFO_0_PAYMENTSTATUS'];
      $data = array(
        'op' => 'doExpressCheckout',
        'receipt_id' => $receipt_id,
        'order_time' => $order_time,
        'payment_status' => $payment_status,
        'invnum' => $payment->method->controller_data['invnum'],
      );

      if (strtolower($ack) === 'success' || strtolower($ack) === 'successwithwarning') {
        $return['success'] = TRUE;
      }
      else {
        $return['success'] = FALSE;

        $errors = $this->paypalErrorLog($payment, $response);
        //Store errors into paypal payment data
        $data['errors'] = $errors['paypal_errors'];
        $return['message'] = $errors['message'];
      }

      //Log paypal payment
      $this->log($payment, $correlation_id, $transaction_id, $ack, $timestamp, $data);
    }

    return $return;
  }

  /**
   * Prepare data items to send to paypal server
   * based on line items payment object given.
   *
   * @param $payment
   *   A payment object containing line items
   * @param $data
   *   A prepared data array to send to paypal server
   *   which will be altered to add items keys
   * @param $payment_num
   *   Number of payment to use in $data array in case of multiple payments.
   *   Not supported yet but implemented anyway.
   *   Default to 0 (first payment).
   */
  private function preparePaypalPaymentRequestData(Payment $payment, &$data, $payment_num = 0) {
    static $data_amount = array();

    //Retrieve additionnal method data amount added to payment
    $shipping_amount = (!empty($payment->method_data['shipping_amount'])) ? $payment->method_data['shipping_amount'] : 0;
    //Must be negative
    $shipping_discount = (!empty($payment->method_data['shipping_discount']) && $payment->method_data['shipping_discount'] < 0) ? $payment->method_data['shipping_discount'] : 0;
    //Force 0 value just for calculation
    $insurance_amount = (!empty($payment->method_data['insurance_amount'])) ? $payment->method_data['insurance_amount'] : 0;
    $handing_amount = (!empty($payment->method_data['handing_amount'])) ? $payment->method_data['handing_amount'] : 0;

    //If not already in static cache, calculate it
    if (!array_key_exists($payment->pid, $data_amount)) {
      $data_amount[$payment->pid] = $this->getPaypalDataAmount($payment, $shipping_amount, $shipping_discount, $insurance_amount, $handing_amount);
    }

    //Payment data
    $data['PAYMENTREQUEST_' . $payment_num . '_AMT'] = $data_amount[$payment->pid]['total_amount'];
    $data['PAYMENTREQUEST_' . $payment_num . '_TAXAMT'] = $data_amount[$payment->pid]['total_tax_amount'];
    $data['PAYMENTREQUEST_' . $payment_num . '_ITEMAMT'] = $data_amount[$payment->pid]['total_items_amount'];

    //Add additionnal amount
    $data['PAYMENTREQUEST_' . $payment_num . '_SHIPPINGAMT'] = $shipping_amount;
    $data['PAYMENTREQUEST_' . $payment_num . '_SHIPDISCAMT'] = $shipping_discount;
    $data['PAYMENTREQUEST_' . $payment_num . '_INSURANCEAMT'] = $insurance_amount;
    $data['PAYMENTREQUEST_' . $payment_num . '_HANDLINGAMT'] = $handing_amount;

    //Payment line item data
    $item_num = 0;
    $line_items = $payment->getLineItems();
    foreach ($line_items as $line_item_name => $line_item) {

      $data['L_PAYMENTREQUEST_' . $payment_num . '_NAME' . $item_num] = $line_item_name;
      $data['L_PAYMENTREQUEST_' . $payment_num . '_DESC' . $item_num] = $line_item->description;
      $data['L_PAYMENTREQUEST_' . $payment_num . '_AMT' . $item_num] = $data_amount[$payment->pid][$line_item_name]['raw_amount'];
      $data['L_PAYMENTREQUEST_' . $payment_num . '_QTY' . $item_num] = $line_item->quantity;
      $data['L_PAYMENTREQUEST_' . $payment_num . '_TAXAMT' . $item_num] = $data_amount[$payment->pid][$line_item_name]['tax_amount'];

      $item_num++;
    }
  }
}