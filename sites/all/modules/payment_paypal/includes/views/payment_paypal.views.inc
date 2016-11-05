<?php

/**
 * Implements hook_views_data() 
 */
function payment_paypal_views_data() {
  $data = array();

  $data['payment_paypal_payment']['table']['base'] = array(
    'field' => 'correlation_id',
    'title' => t('Paypal payment'),
    'help' => t('Contains paypal payments data returns by paypal server.'),
    'access query tag' => 'administer payment paypal',
  );

  $data['payment_paypal_payment']['table']['group']  = t('Paypal payment');

  $data['payment_paypal_payment']['table']['join']['payment'] = array(
    'left_field' => 'pid',
    'field' => 'pid',
  );

  $data['payment_paypal_payment']['pid']['relationship'] = array(
    'title' => t('payment'),
    'label' => t('paypal payment - payment'),
    'help' => t('Retrieve payment associated to this paypal payment.'),
    'handler' => 'views_handler_relationship',
    'base' => 'payment',
    'field' => 'pid',
  );

  //Most of case useless : we could found it from payment relationship
  $data['payment_paypal_payment']['pmid']['relationship'] = array(
    'title' => t('payment method'),
    'label' => t('paypal payment - payment method'),
    'help' => t('Retrieve payment method associated to this paypal payment. In most of case, you don\t need to use it as you could retrieve it from payment relationship instead.'),
    'handler' => 'views_handler_relationship',
    'base' => 'payment_method',
    'field' => 'pmid',
  );

  $data['payment_paypal_payment']['correlation_id'] = array(
    'title' => t('Correlation ID'),
    'help' => t('Unique id returns by paypal server'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['payment_paypal_payment']['transaction_id'] = array(
    'title' => t('Transaction ID'),
    'help' => t('Transaction id returns by paypal server'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['payment_paypal_payment']['timestamp'] = array(
    'title' => t('Payment date'),
    'help' => t('Timestamp returns by paypal server'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['payment_paypal_payment']['data'] = array(
    'title' => t('Additional data'),
    'help' => t('Additionnal data returns by paypal server'),
    'field' => array(
      'handler' => 'views_handler_field_serialized',
    ),
  );

  $data['payment_paypal_payment']['data_list'] = array(
    'title' => t('Additional data list'),
    'help' => t('List of additionnal data returns by paypal server'),
    'field' => array(
      'real field' => 'data',
      'handler' => 'PaymentPaypalViewsHandlerFieldDataList',
    ),
  );

  return $data;
}