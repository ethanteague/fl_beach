<?php

module_load_include('inc', 'payment_paypal', 'payment_paypal.form');

/**
 * Parent class for paypal method
 */
class PaymentPaypalMethodController extends PaymentMethodController {

  public $controller_data_defaults = array(
    'api_username' => '',
    'api_passwd' => '',
    'api_signature' => '',
    'endpoint' => PAYMENT_PAYPAL_ENDPOINT_URL,
    'show_error' => FALSE,
    'settings' => array(),
  );

  //Be careful, by initialize it at 0, it means no limit !
  public $default_max_execution_time = 0;

  function __construct() {

    $this->default_max_execution_time = ini_get('max_execution_time');
    //Couldn't retrieve value, set default to 30.
    if (!is_numeric($this->default_max_execution_time)) {
      $this->default_max_execution_time = 30;
    }
  }

  /**
   * Implements PaymentMethodController::validate().
   *
   * Be careful, this method is called further time by PaymentMethod object.
   * For example while trying to check availibility of this method.
   *
   * This should be more some internal checks to execute rather than some validate
   * checks againsts the payment webservice...
   * Furthermore, it will coast a lot of time unnecessarily as fail payment will have
   * a corresponding status.
   */
  function validate(Payment $payment, PaymentMethod $payment_method, $strict) {
    //First call parent method
    parent::validate($payment, $payment_method, $strict);

    //If some additionnal internal checks must be done...
  }

  /**
   * Returns an array of amount data expected by paypal server,
   * based on a payment given and its line items.
   *
   * Seems that paypal server only expect two decimals forced/rounded values
   * for payment fields, not payment items fields. In doubt, 2 decimals places
   * are added to each amount values.
   *
   * @FIXME Total amount needs to include shipping and tax rate, but also
   * insurance, handing cost ? Deduce shipping discount ?
   * @FIXME Total item amount only include each line item raw amount (no taxes) ?
   * @FIXME Shipping coast doesn't include handing coast ?
   *
   * @param $payment
   * @param $shipping_amount
   *   A positive float with precision
   * @param $shipping_discount
   *   A negative float
   * @param $insurance_amount
   *   A positive float
   * @param $handing_amount
   *   A positive float
   *
   * @return
   *   An array containing data keyed with :
   *   - total amount : transaction coast (rounded down to 2 decimal precision)
   *   - total tax amount
   *   - total items amount, without taxes (rounded down to 2 decimal precision)
   *   - array keyed by line items name, with :
   *     - raw amount
   *     - tax amount
   */
  public function getPaypalDataAmount(Payment $payment, $shipping_amount = 0, $shipping_discount = 0, $insurance_amount = 0, $handing_amount = 0) {

    $data_amount = array();
    $total_raw_amount = 0;
    $total_tax_amount = 0;

    $line_items = $payment->getLineItems();
    foreach ($line_items as $line_item_name => $line_item) {
      $data_amount[$line_item_name]['raw_amount'] = number_format(round($line_item->totalAmount(FALSE), PAYMENT_PAYPAL_AMOUNT_PRECISION), PAYMENT_PAYPAL_AMOUNT_PRECISION);
      $data_amount[$line_item_name]['tax_amount'] = number_format(round(($data_amount[$line_item_name]['raw_amount'] * ($line_item->tax_rate > 0 ? $line_item->tax_rate : 0)), PAYMENT_PAYPAL_AMOUNT_PRECISION), PAYMENT_PAYPAL_AMOUNT_PRECISION);

      $total_raw_amount += $data_amount[$line_item_name]['raw_amount'];
      $total_tax_amount += $data_amount[$line_item_name]['tax_amount'];
    }

    //Force totals to have 2 decimals precision
    $total_raw_amount = number_format($total_raw_amount, PAYMENT_PAYPAL_AMOUNT_PRECISION);
    $total_tax_amount = number_format($total_tax_amount, PAYMENT_PAYPAL_AMOUNT_PRECISION);

    $data_amount['total_items_amount'] = $total_raw_amount;
    $data_amount['total_tax_amount'] = $total_tax_amount;

    $total_amount = $total_raw_amount + $total_tax_amount + $shipping_amount - $shipping_discount + $insurance_amount + $handing_amount;
    //Round total value in case of float precision from other coast
    //are greater than 2 decimal (shipping, handing, and so on).
    $data_amount['total_amount'] = number_format(round($total_amount, PAYMENT_PAYPAL_AMOUNT_PRECISION), PAYMENT_PAYPAL_AMOUNT_PRECISION);

    return $data_amount;
  }

  /**
   * Log data returned from paypal server into a paypal payment object
   */
  protected function log(Payment $payment, $correlation_id, $transaction_id, $ack, $timestamp, $data) {
    payment_paypal_paypal_payment_log($payment, $correlation_id, $transaction_id, $ack, $timestamp, $data);
  }

  /**
   * Returns a invoice unique id
   * to prevent from duplicate charges
   *
   * @param $payment
   *   A payment object associated to the paypal payment
   */
  protected function getInvoiceID(Payment $payment) {
    global $user;
    return $user->uid . '_' . $payment->pid;
  }

  /**
   * Increase timeout which should exceed default 30 secondes
   * due to paypal API request
   */
  protected function setTimeout($reset = FALSE) {
    $max_execution_time = $this->default_max_execution_time;
    if (!$reset) {

      //Default to 1 (0 means no limit) in case
      //of default execution is enough (>= 60s)
      $time_limit = '1';
      if ($this->default_max_execution_time < 60) {
        $max_execution_time = 60;
        //Get a value to add of the current time executed
        //Should probaly be lower than 60s
        $time_limit = 60 - $this->default_max_execution_time;
      }

      //Add necessary time to current execution
      drupal_set_time_limit($time_limit);
    }
    ini_set('max_execution_time', $max_execution_time);
  }

  /**
   * Function helper to deals with HTTP code
   * and parse response into an array.
   *
   * @param $result
   *   An array of result returned
   *   by drupal_http_request()
   * @param $payment
   *   The payment object used
   *
   * @return
   *   An array keyed by :
   *     - message : error message
   *     - response : response returned and parsed
   *       from the remote server.
   */
  protected function parseHTTPResponse($result, Payment $payment) {
    $return = array(
      'message' => '',
      'response' => array(),
    );

    //Some HTTP error occured
    if (isset($result->error)) {
      $vars = array(
        '@code' => $result->code,
        '@error' => $result->error,
        '@pid' => $payment->pid,
        '@pmid' => $payment->method->pmid,
        '@payment_method' => $payment->method->title_generic,
      );

      $return['message'] = t('@payment_method payment method have encounter some HTTP error while trying to request paypal server. Error code "@code" and error message "@error". Pid payment : @pid and pmid method used : @pmid', $vars);

    }
    //Redirect ?
    elseif ($result->code != 200) {
      $vars = array(
        '@code' => $result->code,
        '@pid' => $payment->pid,
        '@pmid' => $payment->method->pmid,
        '@payment_method' => $payment->method->title_generic,
      );

      $return['message'] = t('@payment_method payment method have encounter some HTTP error while trying to request paypal server. It doesn\'t return a 200 code. The returned code is "@code". Pid payment : @pid and pmid method used : @pmid', $vars);

    }
    //Weird behaviour ?
    elseif (empty($result->data)) {
      $vars = array(
        '@pid' => $payment->pid,
        '@pmid' => $payment->method->pmid,
        '@payment_method' => $payment->method->title_generic,
      );

      $return['message'] = t('@payment_method payment method has no data returned by paypal server. Pid payment : @pid and pmid method used : @pmid', $vars);
    }
    //Otherwise, HTTP request seems to succeed.
    //Deals with returned responses
    else {

      $response = array();
      parse_str($result->data, $response);
      $return['response'] = $response;
    }

    return $return;
  }

  /**
   * Function helper to log
   * paypal errors returned.
   * @param $payment
   *   Payment object executed
   * @param $response
   *   Parsed array response
   *   returned by paypal server
   *
   * @return
   *   An array keyed by :
   *     - paypal_errors : Contains paypal error messages
   *     - message : error to throw to payment system
   */
  protected function paypalErrorLog(Payment $payment, $response) {

    $paypal_msg = array();

    //Retrieve paypal error messages
    $errors_paypal = payment_paypal_sort_paypal_errors($response);
    foreach ($errors_paypal as $num => $error) {

      $vars = array(
        '@error_code' => (!empty($error['error_code'])) ? $error['error_code'] : t('No code'),
        '@short_msg' => (!empty($error['L_SHORTMESSAGE'])) ? $error['L_SHORTMESSAGE'] : t('No short message'),
        '@long_msg' => (!empty($error['L_LONGMESSAGE'])) ? $error['L_LONGMESSAGE'] : t('No long message'),
        '@severity' => (!empty($error['L_SEVERITYCODE'])) ? $error['L_SEVERITYCODE'] : t('No severity'),
      );

      $paypal_msg[] = t('Paypal error code : @error_code. Short message : @short_msg. Long message : @long_msg. Severity : @severity.', $vars);

      //@TODO - this is in charge to the paypal account
      //settings to configure language error retourned ?
      if ($payment->method->controller_data['show_error']) {
        drupal_set_message($vars['@long_msg'], 'error');
      }
    }

    //Log these paypal errors if any
    if ($paypal_msg) {
      $vars = array(
        '@pid' => $payment->pid,
        '@pmid' => $payment->method->pmid,
        '@correlation_id' => $response['CORRELATIONID'],
      );
      $title = t('Paypal error returned for payment pid : @pid with pmid : @pmid and correlation_id : @correlation_id.', $vars);

      $list = theme('item_list', array('items' => $paypal_msg, 'title' => $title));
      watchdog('payment_paypal', $list, NULL, WATCHDOG_ERROR);
    }

    //Prepare message to throw
    $vars = array(
      '@ack' => $response['ACK'],
      '@pid' => $payment->pid,
      '@pmid' => $payment->method->pmid,
      '@correlation_id' => $response['CORRELATIONID'],
    );

    $msg = t('Paypal payment hasn\'t receive a successfull ack : @ack returned by paypal server. Pid payment : @pid and pmid method used : @pmid. Correlation id of this transaction : @correlation_id', $vars);

    return array(
      'paypal_errors' => $errors_paypal,
      'message' => $msg,
    );
  }
}
