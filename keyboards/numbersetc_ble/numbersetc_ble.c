/*
 * numbersetc_ble.c
 *
 *  Created on: 2030/01/23
 *      Author: Gachiham
 */



#include "matrix.h"
#include "quantum.h"
#include "nrf.h"
#include "app_ble_func.h"

#include "nrf_power.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrf_log.h"

// Inverted to preserve battery life
#define STATUS_LED_ON  nrf_gpio_pin_clear(LED_PIN)
#define STATUS_LED_OFF nrf_gpio_pin_set(LED_PIN)

void matrix_init_user() {
  set_usb_enabled(true);

  // blink on power on
  nrf_gpio_cfg_output(LED_PIN);
  nrf_gpio_cfg_input(SWITCH_PIN, NRF_GPIO_PIN_PULLDOWN);

  for (int i = 0; i < 3; i++) {
    STATUS_LED_ON;
    nrf_delay_ms(100);

    STATUS_LED_OFF;
    nrf_delay_ms(100);
  }

  STATUS_LED_ON;
}