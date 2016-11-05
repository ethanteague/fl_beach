<?php

/**
 * @file WPP payment method controller
 * Only deals with doDirectPayment action
 */

class PaymentPaypalDirectMethodController extends PaymentPaypalMethodController {

  public $payment_method_configuration_form_elements_callback = 'payment_paypal_method_payment_method_configuration_form_elements';

  public $payment_configuration_form_elements_callback = 'payment_paypal_direct_method_payment_configuration_form_elements';

  function __construct() {
    $this->title = t('Paypal Direct payment');
    $this->description = t("Use paypal direct payment");

    parent::__construct();
  }

  /**
   * Implements PaymentMethodController::execute().
   */
  function execute(Payment $payment) {

    $return = $this->doDirectPayment($payment);

    //No error occured
    if (!empty($return['success']) && $return['success'] === TRUE) {
      $payment->setStatus(new PaymentStatusItem(PAYMENT_STATUS_SUCCESS));
    }
    //Otherwise, throw payment exception
    else {
      //Throw error to caller
      throw new PaymentValidationException($return['message']);
    }
  }

  /**
   * Method which is in charge of making
   * a transaction againsts a paypal server
   */
  private function doDirectPayment(Payment $payment) {

    //Get credit card data
    $credit_card = $payment->context_data['credit_card'];

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
      'METHOD' => 'DoDirectPayment',
      'VERSION' => PAYMENT_PAYPAL_API_VERSION,
      'IPADDRESS' => ip_address(),
      'RETURNFMFDETAILS' => 1,
      'PAYMENTACTION' => 'Sale',
      'ACCT' => $credit_card['number'],
      'CREDITCARDTYPE' => $credit_card['type'],
      'CVV2' => $credit_card['code'],
      'FIRSTNAME' => $credit_card['first_name'],
      'LASTNAME' => $credit_card['last_name'],
      'STREET' => $credit_card['street'],
      'STREET2' => (!empty($credit_card['street2'])) ? $credit_card['street2'] : '',
      'CITY' => $credit_card['city'],
      'STATE' => (!empty($credit_card['state'])) ? $credit_card['state'] : '',
      'ZIP' => (!empty($credit_card['zip'])) ? $credit_card['zip'] : '',
      'COUNTRYCODE' => $credit_card['countrycode'],
      'CURRENCYCODE' => $payment->currency_code,
      'EXPDATE' => $credit_card['exp_month'] . $credit_card['exp_year'],
      'DESC' => $payment->description,
      'INVNUM' => $this->getInvoiceID($payment),
    );
    $this->preparePaypalPaymentRequestData($payment, $data);

    //Build request options.
    $options = array(
      'method' => 'POST',
      'data' => drupal_http_build_query($data),
    );

    //Increase timeout
    $this->setTimeout(FALSE);

    //Send datas to Paypal server.
    $result = drupal_http_request($payment->method->controller_data['endpoint'], $options);

    //Reset previous value
    $this->setTimeout(TRUE);

    $return = $this->parseHTTPResponse($result, $payment);

    if (empty($return['response'])) {
      $return['success'] = FALSE;
    }
    else {

      $response = $return['response'];
      unset($return['response']);

      //Set default value returned
      $ack = $response['ACK'];
      $correlation_id = $response['CORRELATIONID'];
      $transaction_id = '';
      $timestamp = strtotime($response['TIMESTAMP']);
      $data = array();

      //Transaction successed
      if (strtolower($ack) === 'success' || strtolower($ack) === 'successwithwarning') {

        $transaction_id = $response['TRANSACTIONID'];

        //Additionnal data to stored
        $data = array(
          'amt' => $response['AMT'],
          'currency_code' => $response['CURRENCYCODE'],
          'avscode' => $response['AVSCODE'],
          'cvv2match' => $response['CVV2MATCH'],
        );

        $return['success'] = TRUE;

      }
      //Deals with error returned
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

  /**
   * Prepare data items to send to paypal server
   * based on line items payment object given.
   *
   * @param $payment
   *   A payment object containing line items
   * @param $data
   *   A prepared data array to send to paypal server
   *   which will be altered to add items keys
   */
  private function preparePaypalPaymentRequestData(Payment $payment, &$data) {
    static $data_amount = array();

    if (!array_key_exists($payment->pid, $data_amount)) {
      $data_amount[$payment->pid] = $this->getPaypalDataAmount($payment);
    }

    //Payment data
    $data['AMT'] = $data_amount[$payment->pid]['total_amount'];
    $data['TAXAMT'] = $data_amount[$payment->pid]['total_tax_amount'];
    $data['ITEMAMT'] = $data_amount[$payment->pid]['total_items_amount'];

    //Payment line item data
    $item_num = 0;
    $line_items = $payment->getLineItems();
    foreach ($line_items as $line_item_name => $line_item) {

      $data['L_NAME' . $item_num] = $line_item_name;
      $data['L_DESC' . $item_num] = $line_item->description;
      $data['L_AMT' . $item_num] = $data_amount[$payment->pid][$line_item_name]['raw_amount'];
      $data['L_QTY' . $item_num] = $line_item->quantity;
      $data['L_TAXAMT' . $item_num] = $data_amount[$payment->pid][$line_item_name]['tax_amount'];

      $item_num++;
    }
  }
}