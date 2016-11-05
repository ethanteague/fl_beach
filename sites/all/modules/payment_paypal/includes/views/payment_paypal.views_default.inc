<?php
/**
 * @file
 * Default views.
 */

/**
 * Implements hook_views_default_views().
 */
function payment_paypal_views_default_views() {
  $views = array();

  $paypal_payment_view = payment_paypal_views_paypal_payment();
  $views[$paypal_payment_view->name] = $paypal_payment_view;

  return $views;
}

/**
 * Default paypal payment views based on payment entity
 */
function payment_paypal_views_paypal_payment() {
  $view = new view;
  $view->name = 'payment_paypal';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'payment_paypal_payment';
  $view->human_name = 'Paypal payments';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Paypal payments';
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer payment paypal';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '30';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 0;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'correlation_id' => 'correlation_id',
    'timestamp' => 'timestamp',
    'transaction_id' => 'transaction_id',
    'data_list' => 'data_list',
    'pid' => 'pid',
    'title' => 'title',
    'context' => 'context',
    'currency_code' => 'currency_code',
    'amount' => 'amount',
    'quantity' => 'quantity',
    'tax_rate' => 'tax_rate',
    'amount_total' => 'amount_total',
  );
  $handler->display->display_options['style_options']['default'] = 'timestamp';
  $handler->display->display_options['style_options']['info'] = array(
    'correlation_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'timestamp' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'transaction_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'data_list' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'pid' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'context' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'currency_code' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'amount' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'quantity' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'tax_rate' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'amount_total' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = 'There is currently no paypal payment available';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['empty']['area']['tokenize'] = 0;
  /* Relationship: Payment paypal: payment */
  $handler->display->display_options['relationships']['pid']['id'] = 'pid';
  $handler->display->display_options['relationships']['pid']['table'] = 'payment_paypal_payment';
  $handler->display->display_options['relationships']['pid']['field'] = 'pid';
  $handler->display->display_options['relationships']['pid']['required'] = 1;
  /* Relationship: Payment: Last status item */
  $handler->display->display_options['relationships']['psiid_last']['id'] = 'psiid_last';
  $handler->display->display_options['relationships']['psiid_last']['table'] = 'payment';
  $handler->display->display_options['relationships']['psiid_last']['field'] = 'psiid_last';
  $handler->display->display_options['relationships']['psiid_last']['relationship'] = 'pid';
  $handler->display->display_options['relationships']['psiid_last']['required'] = 1;
  /* Relationship: Payment: Line items */
  $handler->display->display_options['relationships']['pid_line_item']['id'] = 'pid_line_item';
  $handler->display->display_options['relationships']['pid_line_item']['table'] = 'payment';
  $handler->display->display_options['relationships']['pid_line_item']['field'] = 'pid_line_item';
  $handler->display->display_options['relationships']['pid_line_item']['relationship'] = 'pid';
  $handler->display->display_options['relationships']['pid_line_item']['required'] = 1;
  /* Relationship: Payment: Payment method */
  $handler->display->display_options['relationships']['pmid']['id'] = 'pmid';
  $handler->display->display_options['relationships']['pmid']['table'] = 'payment';
  $handler->display->display_options['relationships']['pmid']['field'] = 'pmid';
  $handler->display->display_options['relationships']['pmid']['relationship'] = 'pid';
  $handler->display->display_options['relationships']['pmid']['required'] = 1;
  /* Field: Payment paypal: Correlation ID */
  $handler->display->display_options['fields']['correlation_id']['id'] = 'correlation_id';
  $handler->display->display_options['fields']['correlation_id']['table'] = 'payment_paypal_payment';
  $handler->display->display_options['fields']['correlation_id']['field'] = 'correlation_id';
  /* Field: Payment paypal: Payment date */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'payment_paypal_payment';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['external'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['timestamp']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['timestamp']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['html'] = 0;
  $handler->display->display_options['fields']['timestamp']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['timestamp']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['timestamp']['hide_empty'] = 0;
  $handler->display->display_options['fields']['timestamp']['empty_zero'] = 0;
  $handler->display->display_options['fields']['timestamp']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'short';
  /* Field: Payment paypal: Transaction ID */
  $handler->display->display_options['fields']['transaction_id']['id'] = 'transaction_id';
  $handler->display->display_options['fields']['transaction_id']['table'] = 'payment_paypal_payment';
  $handler->display->display_options['fields']['transaction_id']['field'] = 'transaction_id';
  $handler->display->display_options['fields']['transaction_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['transaction_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['transaction_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['transaction_id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['transaction_id']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['transaction_id']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['transaction_id']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['transaction_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['transaction_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['transaction_id']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['transaction_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['transaction_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['transaction_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['transaction_id']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['transaction_id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['transaction_id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['transaction_id']['empty_zero'] = 0;
  $handler->display->display_options['fields']['transaction_id']['hide_alter_empty'] = 1;
  /* Field: Payment paypal: Additional data list */
  $handler->display->display_options['fields']['data_list']['id'] = 'data_list';
  $handler->display->display_options['fields']['data_list']['table'] = 'payment_paypal_payment';
  $handler->display->display_options['fields']['data_list']['field'] = 'data_list';
  $handler->display->display_options['fields']['data_list']['label'] = 'Additional data';
  $handler->display->display_options['fields']['data_list']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['data_list']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['data_list']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['data_list']['alter']['external'] = 0;
  $handler->display->display_options['fields']['data_list']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['data_list']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['data_list']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['data_list']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['data_list']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['data_list']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['data_list']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['data_list']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['data_list']['alter']['html'] = 0;
  $handler->display->display_options['fields']['data_list']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['data_list']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['data_list']['hide_empty'] = 0;
  $handler->display->display_options['fields']['data_list']['empty_zero'] = 0;
  $handler->display->display_options['fields']['data_list']['hide_alter_empty'] = 1;
  /* Field: Payment: Payment ID */
  $handler->display->display_options['fields']['pid']['id'] = 'pid';
  $handler->display->display_options['fields']['pid']['table'] = 'payment';
  $handler->display->display_options['fields']['pid']['field'] = 'pid';
  $handler->display->display_options['fields']['pid']['relationship'] = 'pid';
  $handler->display->display_options['fields']['pid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['pid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['pid']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['pid']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['pid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['pid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['pid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['pid']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['pid']['format_plural'] = 0;
  /* Field: Payment status item: Status title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'payment_status_item';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'psiid_last';
  $handler->display->display_options['fields']['title']['label'] = 'Status';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['external'] = 0;
  $handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 1;
  /* Field: Payment: Context */
  $handler->display->display_options['fields']['context']['id'] = 'context';
  $handler->display->display_options['fields']['context']['table'] = 'payment';
  $handler->display->display_options['fields']['context']['field'] = 'context';
  $handler->display->display_options['fields']['context']['relationship'] = 'pid';
  /* Field: Payment: Currency code */
  $handler->display->display_options['fields']['currency_code']['id'] = 'currency_code';
  $handler->display->display_options['fields']['currency_code']['table'] = 'payment';
  $handler->display->display_options['fields']['currency_code']['field'] = 'currency_code';
  $handler->display->display_options['fields']['currency_code']['relationship'] = 'pid';
  $handler->display->display_options['fields']['currency_code']['label'] = 'Currency';
  $handler->display->display_options['fields']['currency_code']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['currency_code']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['currency_code']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['currency_code']['alter']['external'] = 0;
  $handler->display->display_options['fields']['currency_code']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['currency_code']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['currency_code']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['currency_code']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['currency_code']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['currency_code']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['currency_code']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['currency_code']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['currency_code']['alter']['html'] = 0;
  $handler->display->display_options['fields']['currency_code']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['currency_code']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['currency_code']['hide_empty'] = 0;
  $handler->display->display_options['fields']['currency_code']['empty_zero'] = 0;
  $handler->display->display_options['fields']['currency_code']['hide_alter_empty'] = 1;
  /* Field: SUM(Payment line item: Amount) */
  $handler->display->display_options['fields']['amount']['id'] = 'amount';
  $handler->display->display_options['fields']['amount']['table'] = 'payment_line_item';
  $handler->display->display_options['fields']['amount']['field'] = 'amount';
  $handler->display->display_options['fields']['amount']['relationship'] = 'pid_line_item';
  $handler->display->display_options['fields']['amount']['group_type'] = 'sum';
  /* Field: Payment line item: Quantity */
  $handler->display->display_options['fields']['quantity']['id'] = 'quantity';
  $handler->display->display_options['fields']['quantity']['table'] = 'payment_line_item';
  $handler->display->display_options['fields']['quantity']['field'] = 'quantity';
  $handler->display->display_options['fields']['quantity']['relationship'] = 'pid_line_item';
  $handler->display->display_options['fields']['quantity']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['external'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['quantity']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['quantity']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['html'] = 0;
  $handler->display->display_options['fields']['quantity']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['quantity']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['quantity']['hide_empty'] = 0;
  $handler->display->display_options['fields']['quantity']['empty_zero'] = 0;
  $handler->display->display_options['fields']['quantity']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['quantity']['format_plural'] = 0;
  /* Field: AVG(Payment line item: Tax rate) */
  $handler->display->display_options['fields']['tax_rate']['id'] = 'tax_rate';
  $handler->display->display_options['fields']['tax_rate']['table'] = 'payment_line_item';
  $handler->display->display_options['fields']['tax_rate']['field'] = 'tax_rate';
  $handler->display->display_options['fields']['tax_rate']['relationship'] = 'pid_line_item';
  $handler->display->display_options['fields']['tax_rate']['group_type'] = 'avg';
  $handler->display->display_options['fields']['tax_rate']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['tax_rate']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['tax_rate']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['tax_rate']['alter']['external'] = 0;
  $handler->display->display_options['fields']['tax_rate']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['tax_rate']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['tax_rate']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['tax_rate']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['tax_rate']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['tax_rate']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['tax_rate']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['tax_rate']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['tax_rate']['alter']['html'] = 0;
  $handler->display->display_options['fields']['tax_rate']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['tax_rate']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['tax_rate']['hide_empty'] = 0;
  $handler->display->display_options['fields']['tax_rate']['empty_zero'] = 0;
  $handler->display->display_options['fields']['tax_rate']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['tax_rate']['set_precision'] = 0;
  $handler->display->display_options['fields']['tax_rate']['precision'] = '2';
  $handler->display->display_options['fields']['tax_rate']['format_plural'] = 0;
  /* Field: SUM(Payment line item: Total amount) */
  $handler->display->display_options['fields']['amount_total']['id'] = 'amount_total';
  $handler->display->display_options['fields']['amount_total']['table'] = 'payment_line_item';
  $handler->display->display_options['fields']['amount_total']['field'] = 'amount_total';
  $handler->display->display_options['fields']['amount_total']['relationship'] = 'pid_line_item';
  $handler->display->display_options['fields']['amount_total']['group_type'] = 'sum';
  $handler->display->display_options['fields']['amount_total']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['amount_total']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['amount_total']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['amount_total']['alter']['external'] = 0;
  $handler->display->display_options['fields']['amount_total']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['amount_total']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['amount_total']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['amount_total']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['amount_total']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['amount_total']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['amount_total']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['amount_total']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['amount_total']['alter']['html'] = 0;
  $handler->display->display_options['fields']['amount_total']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['amount_total']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['amount_total']['hide_empty'] = 0;
  $handler->display->display_options['fields']['amount_total']['empty_zero'] = 0;
  $handler->display->display_options['fields']['amount_total']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['amount_total']['set_precision'] = 1;
  $handler->display->display_options['fields']['amount_total']['precision'] = '2';
  $handler->display->display_options['fields']['amount_total']['format_plural'] = 0;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/content/payment_paypal';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Paypal payments';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $translatables['payment_paypal'] = array(
    t('Master'),
    t('Paypal payments'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('There is currently no paypal payment available'),
    t('paypal payment - payment'),
    t('Last status item'),
    t('Line items'),
    t('Payment method'),
    t('Correlation ID'),
    t('Payment date'),
    t('Transaction ID'),
    t('Additional data'),
    t('Payment ID'),
    t('.'),
    t(','),
    t('Status'),
    t('Context'),
    t('Currency'),
    t('Amount'),
    t('Quantity'),
    t('Tax rate'),
    t('Total amount'),
    t('Page'),
  );

  return $view;
}