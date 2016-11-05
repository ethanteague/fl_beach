DESCRIPTION
-----------

This module implements paypal payment methods
to use with the <a href="http://drupal.org/project/payment">payment</a> module.
Payment module must be at least the 1.0-beta3 version.

It deals with :

- Paypal Website Payment Pro (WPP) integration
  with direct payment method.
- Paypal Website Standard (WPS) integration
  with express checkout method.

Both of this payment method only deal with single direct sale.
So it doesn't support :

- multiple payments
- recurring payment
- authorization and capture

It integrates views by providing relationship between
payment and paypal payments (data returned by paypal server).

REQUIREMENT
-----------

- Have openSSL loaded and enabled

- Install and enable payment module (at least 1.0-beta3) :
  http://drupal.org/project/payment

- For WPP (direct payment method), install and enable payment credit card
  http://drupal.org/project/payment_credit_card

INSTALL
-------

1 - Before installing and enable the module : 
a default key will be generated and used to crypt
the password API of the merchant account used.
But this is STRONGLY advice to set our own key
in settings.php, rather than store it too in DB :

$conf['payment_paypal_default_crypt_password_key']
= '<Our key to crypt password API credential>';

2 - Install and enable the module.

3 - Go to payment back-office to add the payment method
    called 'Paypal direct' or 'Paypal Express checkout'.
    admin > configuration > payment -> payment method > add
    OR
    admin/config/services/payment/method/add

    Fill default settings and add our API credential data.
    After submitting the settings payment method form and enable the method,
    it will try to reach the associated account and providing current balance.

4 - Method should be available and usable

Use sandbox first to be sure payment are well executed.

NOTE
----

PAYPAL ERROR
When payment method is executed, if paypal server encounter some errors,
they will be log into database (must have at least one log handler such as dblog)
These errors could be throw to end-user but there is no translation done.
This is in charge of the paypal merchant account to configure it.

ROUNDED AMOUNT
Paypal need to receive amounts rounded to a decimal precision of 2.
Line item amount are rounded and tax amount too.

WPS - PAYMENT PAYPAL STATUS

This method add two payment status

- redirect : this status is set just before redirect the user on the payment
             gateway.

- return : The user have successfully complete the remote payment and have been
           redirect to the site. But the remote payment isn't fully complete. At
           this step, money isn't yet transferred and need a last operations.

- cancel : If the remote payment have been cancel by the user.

DEVELOPPER
----------

WPS OFFSITE

Into context module, you could pass additionals amounts to WPS method
through payment object, by added into attribute "method_data" keys :

- shipping_amount : positive float
- shipping_discount : negative float
- insurance_amount : positive float or a string containing 'null' to offer
- handing_amount : positive float
