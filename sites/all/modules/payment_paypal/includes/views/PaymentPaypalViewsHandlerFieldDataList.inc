<?php

/**
 * @file
 * A views field handler.
 */

/**
 * Views field handler for displaying a list of 
 * other extra data returned by paypal server
 */
class PaymentPaypalViewsHandlerFieldDataList extends views_handler_field {

  /**
   * Implements views_handler_field::render().
   */
  function render($values) {
    $output = '';

    if (isset($values->{$this->field_alias})) {
      $data = unserialize($values->{$this->field_alias});
      if ($data) {

        $items = array();
        foreach ($data as $key => $value) {

          //Deals with errors returned
          if (is_array($value)) {
            foreach ($value as $error_num => $errors) {
              foreach ($errors as $code => $error) {
                $items[] = $code . ' : ' . $error;
              }
            }
          }
          //Otherwise, deals with raw values
          else {
            $items[] = $key . ' : ' . check_plain($value);
          }
        }
        $output .= theme('item_list', array('items' => $items));
      } 
    }

    return $output;
  }
}