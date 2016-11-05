<?php

/**
 * Retrieve a balance to an account 
 * for the primary currency set.
 */
function payment_paypal_get_balance_account($endpoint, $username, $passwd_crypted, $signature) {
  $return = array(
    'success' => FALSE,
    'message' => '',    
  );

  $data = array(
    'METHOD' => 'GetBalance',
    'USER' => $username,
    'PWD' => payment_paypal_get_decrypted_passwd($passwd_crypted),
    'SIGNATURE' => $signature,
    'VERSION' => PAYMENT_PAYPAL_API_VERSION,
    'RETURNALLCURRENCIES' => 0,
  );

  $options = array(
    'method' => 'POST',
    'data' => drupal_http_build_query($data),
  );

  $result = drupal_http_request($endpoint, $options);

  $response = array();
  parse_str($result->data, $response);

  if (isset($result->error)) {
    $return['success'] = FALSE;

    $vars = array(
      '@code' => $result->code,
      '@error' => $result->error,
    );

    $return['message'] = t('Unable to establish a connection with the account provided. An HTTP error occured while trying to request paypal server. Returned code : @code and error message : @error', $vars);
  }
  elseif ($result->code != 200) {
    $return['success'] = FALSE;

      $vars = array(
        '@code' => $result->code,
      );

      $return['message'] = t('Request to paypal server doesn\'t return a 200 code. Returned code : @code', $vars);
  }
  else {

    $ack = $response['ACK'];

    //Error returned by paypal
    if (strtolower($ack) !== 'success' && strtolower($ack) !== 'successwithwarning') {

      $return['success'] = FALSE;
      $msg = '';

      $paypal_msg = array();
      $errors_paypal = payment_paypal_sort_paypal_errors($response);
      foreach ($errors_paypal as $num => $error) {

        $vars = array(
          '@error_code' => (!empty($error['error_code'])) ? $error['error_code'] : t('No code'),
          '@short_msg' => (!empty($error['L_SHORTMESSAGE'])) ? $error['L_SHORTMESSAGE'] : t('No short message'),
          '@long_msg' => (!empty($error['L_LONGMESSAGE'])) ? $error['L_LONGMESSAGE'] : t('No long message'),
          '@severity' => (!empty($error['L_SEVERITYCODE'])) ? $error['L_SEVERITYCODE'] : t('No severity'),
        );

        $paypal_msg[] = t('Paypal error code : @error_code. Short message : @short_msg. Long message : @long_msg. Severity : @severity.', $vars);
      }

      if ($paypal_msg) {
        $title = t('Errors returned by paypal server while tring to get balance to this account :');
        $msg = theme('item_list', array('items' => $paypal_msg, 'title' => $title));
      }

      $return['message'] = $msg;

    }
    //Success response
    else {
      //We request to retrieve only primary currency
      $vars = array(
        '@balance' => $response['L_AMT0'],
        '@currency_code' => $response['L_CURRENCYCODE0'],
      );

      $return['success'] = TRUE;
      $return['message'] = t('Connection establish to account successfully. Current balance is : @balance with primary currency set to @currency_code', $vars);
    }
  }

  return $return;
}

/**
 * Sort and order error returned by paypal server
 *
 * @param $response
 *   An array containing response from paypal server
 *
 * @return
 *   An array keyed by number of error, and keyed by :
 *   - code
 *   - L_SHORTMESSAGE
 *   - L_LONGMESSAGE
 *   - L_SEVERITYCODE
 */
function payment_paypal_sort_paypal_errors($response) {
  $errors = array();

  ksort($response);
  foreach ($response as $key => $data) {
    $explode = explode('L_ERRORCODE', $key);
    if (count($explode) === 2) {

      $num = $explode[1];
      $errors[$num]['error_code'] = $data;

      //Find associated error returned
      $special_keys = array('L_SHORTMESSAGE', 'L_LONGMESSAGE', 'L_SEVERITYCODE');
      foreach ($special_keys as $special_key) {

        if (array_key_exists($special_key . $num, $response)) {
          $errors[$num][$special_key] = $response[$special_key . $num];
          unset($response[$special_key . $num]);
        }
      }
    }
  }

  return $errors;
}

/**
 * @TODO ugly hard code
 *
 * Hope we could use Paypal API to be up-to-date
 *
 * @see https://cms.paypal.com/us/cgi-bin/?cmd=_render-content&content_ID=developer/e_howto_api_nvp_country_codes 
 */
function payment_paypal_get_country_supported() {
  return array(
    'AI' => 'Anguilla',
    'AR' => 'Argentina',
    'AU' => 'Australia',
    'AT' => 'Austria',
    'BE' => 'Belgium',
    'BR' => 'Brazil',
    'CA' => 'Canada',
    'CL' => 'Chile',
    'CN' => 'China',
    'CR' => 'Costa Rica',
    'DK' => 'Denmark',
    'DO' => 'Dominican Republic',
    'EC' => 'Ecuador',
    'FI' => 'Finland',
    'FR' => 'France',
    'DE' => 'Germany',
    'GR' => 'Greece',
    'HK' => 'Hong Kong',
    'IS' => 'Iceland',
    'IN' => 'India',
    'IE' => 'Ireland',
    'IL' => 'Israel',
    'IT' => 'Italy',
    'JM' => 'Jamaica',
    'JP' => 'Japan',
    'LU' => 'Luxembourg',
    'MY' => 'Malaysia',
    'MX' => 'Mexico',
    'MC' => 'Monaco',
    'NL' => 'Netherlands',
    'NZ' => 'New Zealand',
    'NO' => 'Norway',
    'PT' => 'Portugal',
    'SG' => 'Singapore',
    'KR' => 'South Korea',
    'ES' => 'Spain',
    'SE' => 'Sweden',
    'CH' => 'Switzerland',
    'TW' => 'Taiwan',
    'TH' => 'Thailand',
    'TR' => 'Turkey',
    'GB' => 'United Kingdom',
    'US' => 'United States',
    'UY' => 'Uruguay',
  );
}

/**
 * Retrieve a list of supported currency code by paypal.
 * In order to check if currency code provided by Payment
 * is supported.
 * 
 * @todo find method to call from paypal API
 * to get them dynamically rather than ugly hard code
 * not up-to-date...
 *
 * @see https://cms.paypal.com/us/cgi-bin/?cmd=_render-content&content_ID=developer/e_howto_api_nvp_currency_codes
 */
function payment_paypal_currency_code_paypal_direct_supported() {
  return array(
    'AUD',
    'CAD',
    'CZK',
    'DKK',
    'EUR',
    'HUF',
    'JPY',
    'NOK',
    'NZD',
    'PLN',
    'GBP',
    'SGD',
    'SEK',
    'CHF',
    'USD',
  );
}