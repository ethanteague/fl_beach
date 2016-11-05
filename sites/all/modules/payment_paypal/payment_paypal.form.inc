<?php

module_load_include('inc', 'payment_paypal', 'payment_paypal.paypal');

/**
 * Form build callback implements by payment paypal method
 * PaymentMethodController::payment_method_configuration_form_elements_callback.
 *
 * @return array
 *   A Drupal form.
 */
function payment_paypal_method_payment_method_configuration_form_elements(array $form, array &$form_state) {

  $controller_data = $form_state['payment_method']->controller_data;

  $elements = array();
  $elements['paypal'] = array(
    '#type' => 'fieldset',
    '#title' => t('Paypal settings'),
  );

  $elements['paypal']['api_credential'] = array(
    '#type' => 'fieldset',
    '#title' => t('API credential'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $elements['paypal']['api_credential']['api_username'] = array(
    '#type' => 'textfield',
    '#title' => t('API username'),
    '#default_value' => (!empty($controller_data['api_username'])) ? $controller_data['api_username'] : '',
    '#required' => TRUE,
  );

  $elements['paypal']['api_credential']['api_passwd'] = array(
    '#type' => 'password_confirm',
    '#title' => t('API password'),
  );

  $elements['paypal']['api_credential']['api_signature'] = array(
    '#type' => 'textfield',
    '#title' => t('API signature'),
    '#maxlength' => 255,
    '#default_value' => (!empty($controller_data['api_signature'])) ? $controller_data['api_signature'] : '',
    '#required' => TRUE,
  );

  $elements['paypal']['endpoint'] = array(
    '#type' => 'select',
    '#title' => t('Environnement'),
    '#options' => array(
      PAYMENT_PAYPAL_ENDPOINT_URL => t('Paypal live'),
      PAYMENT_PAYPAL_ENDPOINT_SANDBOX_URL => t('Sandbox'),
      PAYMENT_PAYPAL_ENDPOINT_BETA_SANDBOX_URL => t('Beta sandbox'),
    ),
    '#default_value' => (!empty($controller_data['endpoint'])) ? $controller_data['endpoint'] : PAYMENT_PAYPAL_ENDPOINT_URL,
    '#required' => TRUE,
  );

  $elements['paypal']['show_error'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show paypal long message error to the end user'),
    '#default_value' => (!empty($controller_data['show_error'])) ? 1 : 0,
  );

  return $elements;
}

/**
 * Implements form validate callback for
 * payment_paypal_method_payment_method_configuration_form_elements().
 */
function payment_paypal_method_payment_method_configuration_form_elements_validate(array $form, array &$form_state) {
  $controller_data =& $form_state['payment_method']->controller_data;

  $values = $form_state['values']['controller_form']['paypal'];
  $username = $values['api_credential']['api_username'];
  $passwd = '';
  $signature = $values['api_credential']['api_signature'];
  $endpoint = $values['endpoint'];
  $show_error = $values['show_error'];
  //Set default settings for other paypal method
  $settings = (!empty($controller_data['settings'])) ? $controller_data['settings'] : array();

  //Retrieve key to encrypt passwd
  $key = variable_get('payment_paypal_default_crypt_password_key', FALSE);
  if (!$key) {
    form_set_error(implode('][', $form['paypal']['#parents']) . '][api_credential][api_passwd', t('Unable to retrieve generated key for crypting password account. You must add it to our settings.php file.'));
    return;
  }

  //Encrypt new passwd if any
  if (!empty($values['api_credential']['api_passwd'])) {
    $passwd = payment_paypal_crypt_passwd($key, $values['api_credential']['api_passwd']);
  }
  //Retrieve previous encrypted passwd if any
  elseif (!empty($controller_data['api_passwd'])) {
    $passwd = $controller_data['api_passwd'];
  }
  else {
    form_set_error(implode('][', $form['paypal']['#parents']) . '][api_credential][api_passwd', t('You must enter a password API credential.'));
    return;
  }

  //Check account availability by trying to retrieve balance account...
  $result = payment_paypal_get_balance_account($endpoint, $username, $passwd, $signature);
  if (!$result) {
    form_set_error('', t('Unable to establish a connection with this merchant account or to retrieve balance for primary currency.'));
    return;
  }
  elseif ($result['success'] !== TRUE) {
    form_set_error('', $result['message']);
    return;
  }

  //Store settings in custom DB table
  $controller_data['api_username'] = $username;
  $controller_data['api_passwd'] = $passwd;
  $controller_data['api_signature'] = $signature;
  $controller_data['endpoint'] = $endpoint;
  $controller_data['show_error'] = $show_error;
  $controller_data['settings'] = $settings;

  drupal_set_message($result['message']);
}

/**
 * Form build callback implements by paypal express checkout method
 * PaymentMethodController::payment_method_configuration_form_elements_callback.
 *
 * @return array
 *   A Drupal form.
 */
function payment_paypal_express_checkout_method_payment_method_configuration_form_elements(array $form, array $form_state) {

  $controller_data = $form_state['payment_method']->controller_data;

  $elements = array();
  $elements += payment_paypal_method_payment_method_configuration_form_elements($form, $form_state);

  $elements['paypal']['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Paypal settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
  );

  $elements['paypal']['settings']['NOSHIPPING'] = array(
    '#type' => 'select',
    '#title' => t('No shipping'),
    '#description' => t('Determines where or not PayPal displays shipping address fields on the PayPal pages'),
    '#options' => array(
      0 => t('Display shipping address'),
      1 => t('Does\'t display shipping address'),
      2 => t('Get from buyer\'s account profile'),
    ),
    '#default_value' => (isset($controller_data['settings']['express_checkout']['NOSHIPPING'])) ? $controller_data['settings']['express_checkout']['NOSHIPPING'] : 1,
  );

  $elements['paypal']['settings']['ALLOWNOTE'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow note'),
    '#description' => t('Allow customer to enter a note to the merchant'),
    '#default_value' => (!empty($controller_data['settings']['express_checkout']['ALLOWNOTE'])) ? 1 : 0,
  );

  $elements['paypal']['settings']['SOLUTIONTYPE'] = array(
    '#type' => 'select',
    '#title' => t('Solution type'),
    '#description' => t('Type of checkout flow'),
    '#options' => array(
      'Sole' => t('Paypal account optional'),
      'Mark' => t('Paypal account required'),
    ),
    '#default_value' => (!empty($controller_data['settings']['express_checkout']['SOLUTIONTYPE'])) ? $controller_data['settings']['express_checkout']['SOLUTIONTYPE'] : 'Sole',
  );

  $elements['paypal']['settings']['LANDINGPAGE'] = array(
    '#type' => 'select',
    '#title' => t('Landing page'),
    '#description' => t('Type of PayPal page to display'),
    '#options' => array(
      'Billing' => t('non-PayPal account'),
      'Login' => t('PayPal account login'),
    ),
    '#default_value' => (!empty($controller_data['settings']['express_checkout']['LANDINGPAGE'])) ? $controller_data['settings']['express_checkout']['LANDINGPAGE'] : 'Billing',
  );

  $elements['paypal']['settings']['BRANDNAME'] = array(
    '#type' => 'textfield',
    '#title' => t('Brandname'),
    '#description' => t('A label that overrides the business name in the PayPal hosted checkout.'),
    '#default_value' => (!empty($controller_data['settings']['express_checkout']['BRANDNAME'])) ? $controller_data['settings']['express_checkout']['BRANDNAME'] : '',
    '#maxlength' => 127,
  );

  $elements['paypal']['settings']['CUSTOMERSERVICENUMBER'] = array(
    '#type' => 'textfield',
    '#title' => t('Customer service number'),
    '#description' => t('Merchant Customer Service number displayed on the PayPal Review page.'),
    '#default_value' => (!empty($controller_data['settings']['express_checkout']['CUSTOMERSERVICENUMBER'])) ? $controller_data['settings']['express_checkout']['CUSTOMERSERVICENUMBER'] : '',
    '#maxlength' => 16,
    '#size' => 16,
  );

  return  $elements;
}

/**
 * Implements form validate callback for
 * payment_paypal_express_checkout_method_payment_method_configuration_form_elements().
 */
function payment_paypal_express_checkout_method_payment_method_configuration_form_elements_validate(array $form, array $form_state) {

  payment_paypal_method_payment_method_configuration_form_elements_validate($form, $form_state);
  if (!form_get_errors()) {

    $settings = $form_state['values']['controller_form']['paypal']['settings'];
    $controller_data =& $form_state['payment_method']->controller_data;
    $controller_data['settings']['express_checkout'] = (!empty($controller_data['settings']['express_checkout'])) ? array_merge($controller_data['settings']['express_checkout'], $settings) : $settings;
  }
}

/**
 * Implements
 * PaymentMethodController::payment_configuration_form_elements_callback.
 */
function payment_paypal_direct_method_payment_configuration_form_elements(array $element, array &$form_state) {
  $form = array();

  module_load_include('inc', 'payment_credit_card', 'payment_credit_card.credit_card');

  //Don't use field 'owner' as we need separate field : first and last name
  $fields = array(
    'type' => array('visa', 'mastercard'),
    'code' => '',
  );

  //@TODO better to retrieve it from $element['#parents']
  $default_values = (!empty($form_state['values']['payment_method']['payment_method_controller_payment_configuration']['credit_card'])) ? $form_state['values']['payment_method']['payment_method_controller_payment_configuration']['credit_card'] : array();

  //Display default card supported
  $form['credit_card_img'] = array(
    '#type' => 'markup',
    '#markup' => theme('payment_paypal_credit_card_img', array('credit_cards' => array_merge(array('paypal'), $fields['type']))),
    '#prefix' => '<div class="payment-method-desc payment-method-paypal-direct-desc">',
    '#suffix' => '</div>',
  );

  //Get credit card form
  $credit_card_form = payment_credit_card_credit_card_form($fields, $default_values);
  payment_paypal_credit_card_extented_form(array('zip' => ''), $credit_card_form, $default_values);

  //Add additional fields required by paypal API
  $form += $credit_card_form;

  return $form;
}

/**
 * Implements
 * PaymentMethodController::payment_configuration_form_elements_validate_callback.
 */
function payment_paypal_direct_method_payment_configuration_form_elements_validate(array $element, array &$form_state) {
  $payment = $form_state['payment'];

  //Only when the whole form is submitted in order to proceed for payment
  if (!empty($form_state['triggering_element']['#type']) && $form_state['triggering_element']['#type'] === 'submit') {

    $parents = $element['credit_card']['#parents'];

    //Retrieve credit cards values
    $values = $form_state['values'];
    foreach ($parents as $key) {
      $values = $values[$key];
    }
    $credit_card = $values;

    // Validate the credit card fields.
    module_load_include('inc', 'payment_credit_card', 'payment_credit_card.credit_card');
    $settings = array(
      'form_parents' => $parents,
    );

    if (!payment_credit_card_credit_card_validate($credit_card, $settings)) {
      return FALSE;
    }

    //Store credit card data into payment object itself
    //Indeed, we needs a persistent storage as before beeing
    //executed, payment object will be retrieve from SGBD
    $payment->context_data['credit_card'] = $credit_card;
  }
}

/**
 * Implements
 * PaymentMethodController::payment_configuration_form_elements_callback.
 */
function payment_paypal_express_checkout_method_payment_configuration_form_elements(array $element, array &$form_state) {
  $form = array();

  $output = '';
  $output .= theme('payment_paypal_credit_card_img', array('credit_cards' => array('paypal', 'mastercard', 'visa')));
  $output .= '<p>' . t('By selecting the Paypal Express Checkout payment method, you will be redirect to the paypal gateway.') . '</p>';

  //@TODO - make description custom ?
  $form['description'] = array(
    '#type' => 'markup',
    '#markup' => $output,
    '#prefix' => '<div class="payment-method-desc payment-method-paypal-express-checkout-desc">',
    '#suffix' => '</div>',
  );

  return $form;
}

/**
 * Paypal have some additional data required.
 *
 * @param $fields
 *   An array of optional fields to render
 * @param $form
 *   A structured form containing a root parent element
 *   keyed 'credit_card'.
 * @param $default
 *   An array of default values
 */
function payment_paypal_credit_card_extented_form($fields, &$form, $default = array()) {
  //Merge default values into the default array.
  $default += array(
    'countrycode' => '',
    'first_name' => '',
    'last_name' => '',
    'street' => '',
    'street2' => '',
    'city' => '',
    'state' => '',
    'zip' => '',
  );

  $form['credit_card']['countrycode'] = array(
    '#type' => 'select',
    '#title' => t('Country code'),
    '#options' => payment_paypal_get_country_supported(),
    '#default_value' => $default['countrycode'],
    '#required' => TRUE,
  );

  $form['credit_card']['first_name'] = array(
    '#type' => 'textfield',
    '#title' => t('First name'),
    '#default_value' => $default['first_name'],
    '#required' => TRUE,
    //25 single-byte
    '#maxlength' => 25,
    '#size' => 25,
  );

  $form['credit_card']['last_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Last name'),
    '#default_value' => $default['last_name'],
    '#required' => TRUE,
    //25 single-byte
    '#maxlength' => 25,
    '#size' => 25,
  );

  $form['credit_card']['street'] = array(
    '#type' => 'textfield',
    '#title' => t('Street name'),
    '#default_value' => $default['street'],
    '#required' => TRUE,
    //100 single-byte
    '#maxlength' => 100,
    '#size' => 32,
  );

  if (isset($fields['street2'])) {
    $form['credit_card']['street2'] = array(
      '#type' => 'textfield',
      '#title' => t('Second street address'),
      '#default_value' => $default['street2'],
      '#required' => TRUE,
      //25 single-byte
      '#maxlength' => 100,
      '#size' => 32,
    );
  }

  $form['credit_card']['city'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#default_value' => $default['city'],
    '#required' => TRUE,
    //25 single-byte
    '#maxlength' => 40,
    '#size' => 40,
  );

  if (isset($fields['state'])) {
    $form['credit_card']['state'] = array(
      '#type' => 'textfield',
      '#title' => t('State'),
      '#default_value' => $default['state'],
      '#required' => TRUE,
      //25 single-byte
      '#maxlength' => 40,
      '#size' => 40,
    );
  }

  if (isset($fields['zip'])) {
    $form['credit_card']['zip'] = array(
      '#type' => 'textfield',
      '#title' => t('ZIP / Postal code'),
      '#default_value' => $default['zip'],
      //25 single-byte
      '#maxlength' => 20,
      '#size' => 20,
    );
  }
}