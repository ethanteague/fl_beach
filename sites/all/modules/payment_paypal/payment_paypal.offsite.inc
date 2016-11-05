<?php

/**
 * Return url payment method offsite callback.
 * Deals with default behavior againsts a payment object
 * and then let's other context module execute post process with finish callback
 *
 * @param $payment
 *   The payment object retrieve from menu handler
 */
function payment_paypal_return_url(Payment $payment) {

  //Execute payment method to validate and complete previous offsite payment
  //but only if payment have been previously redirect
  if (payment_status_is_or_has_ancestor($payment->getStatus()->status, PAYMENT_PAYPAL_STATUS_REDIRECTED)) {

    try {

      //Change payment status and save it to confirm return url executed
      $payment->setStatus(new PaymentStatusItem(PAYMENT_PAYPAL_STATUS_RETURNED));
      entity_save('payment', $payment);

      $payment->method->controller->completePayment($payment);
    }
    catch (PaymentException $e) {
      //Just save entity, other error handlers have already been done
      entity_save('payment', $payment);
    }
    catch (Exception $e) {
      //Unexpected error occured, be sure payment status set as failed
      //and let's finish callback deals with errors
      $payment->setStatus(new PaymentStatusItem(PAYMENT_STATUS_FAILED));
      entity_save('payment', $payment);
      watchdog('payment_paypal', $e->getMessage(), NULL, WATCHDOG_ERROR);
    }
  }

  //Then let's finish callback deals with cancelled payment
  $payment->finish();
}

/**
 * Cancel url payment method offsite callback.
 * Deals with default behavior againsts a payment object
 * and then let's other context module execute post process with finish callback
 *
 * @param $payment
 *   The payment object retrieve from menu handler
 */
function payment_paypal_cancel_url(Payment $payment) {
  //Set last payment status as cancelled
  $payment->setStatus(new PaymentStatusItem(PAYMENT_STATUS_CANCELLED));
  entity_save('payment', $payment);

  //Let's finish callback deals with cancelled payment
  $payment->finish();
}
