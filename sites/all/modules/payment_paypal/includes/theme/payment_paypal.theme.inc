<?php

/**
 * Theme function to display credit card images
 *
 * @param $variables
 *   Key 'credit_cards' which is an array containing possible values  :
 *     - paypal
 *     - mastercard
 *     - visa
 */
function theme_payment_paypal_credit_card_img($variables) {
  $output = '';

  $default_credit_cards = array('paypal', 'mastercard', 'visa');
  $credit_cards = $variables['credit_cards'];
  
  $base_path_img = drupal_get_path('module', 'payment_paypal') . '/includes/theme/images/';
  foreach($credit_cards as $credit_card) {
    if (in_array($credit_card, $default_credit_cards, TRUE)) {
      $output .=  theme('image', array('path' => $base_path_img . $credit_card . '.gif', 'title' => ucfirst($credit_card), 'alt' => ucfirst($credit_card)));
    }
  }

  return $output;
}
