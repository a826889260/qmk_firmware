/*
 * numbersetc_ble.c
 *
 *  Created on: 2030/01/23
 *      Author: Chasingendgame
 */

#include "matrix.h"
#include "app_ble_func.h"
#include "numbersetc_ble.h"

#include "nrf.h"
#include "nrf_power.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrf_log.h"

// From tmk_core/protocol/nrf/matrix.c
void unselect_cols(void);
void select_col(uint8_t col);
matrix_col_t read_rows(void);
matrix_col_t read_col(uint8_t col);

// Forward declarations
void numbersetc_init_usb();
void numbersetc_init_ble();
void numbersetc_init_user(bool is_slave);

void numbersetc_matrix_init_user(bool is_slave) {
  numbersetc_init_usb();
  numbersetc_init_ble();
  numbersetc_init_user(is_slave);
}

static int bootloader_flag = 0;

void numbersetc_init_user(bool is_slave) {

  // configure status LED as output
  nrf_gpio_cfg_output(LED_PIN);
  // the switch pin reports whether or not the battery is connected
  nrf_gpio_cfg_input(SWITCH_PIN, NRF_GPIO_PIN_PULLDOWN);

  // blink 2x for master, 1x for slave
  led_on(200);
  led_off(100);
  if (!is_slave) {
    led_off(200);
    led_on(200);
  }
  led_off(0);
  
  select_col(1);
  wait_us(50);
  matrix_col_t col = read_rows();
  unselect_cols();
  
  #if IS_SLAVE
  if (col == 0b10) {
    // hold down S6 at power-on to delete bond with master
    delete_bonds();
  }
  else
  #endif
  if (col == 0b1) {
    // hold down S2 at power-on to enter UF2 bootloader
    bootloader_flag = 1;
  }
  #if !IS_SLAVE
  else if (numbersetc_get_ble_enabled()) {
    // master restarts advertising if bluetooth is enabled at startup
    bootloader_flag = 2;
  }
  #endif
}

void numbersetc_matrix_scan_user(bool is_slave) {
  static int count;
  if (bootloader_flag == 1 && count ++== 10) {
    sd_power_gpregret_set(0, 0x57); // DFU_MAGIC_UF2_RESET
    bootloader_jump();
  }
  #if !IS_SLAVE
  if (bootloader_flag == 2 && count ++== 100) {
    bootloader_flag = 0;
    //restart_advertising_wo_whitelist();
  }
  #endif
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
  char str[16];

  bool result = true;

  if (record->event.pressed) {
    switch (keycode) {
    case DELBNDS:
      delete_bonds();
      led_delete(-1);
      result = false;
      break;
    case AD_WO_L:
      restart_advertising_wo_whitelist();
      led_advertise(-1);
      result = false;
      break;
    case USB_EN:
      numbersetc_set_usb_enabled(true);
      result = false;
      break;
    case USB_DIS:
      numbersetc_set_usb_enabled(false);
      result = false;
      break;
    case BLE_EN:
      numbersetc_set_ble_enabled(true);
      led_on(40);
      led_off(40);
      result = false;
      break;
    case BLE_DIS:
      numbersetc_set_ble_enabled(false);
      led_on(40);
      led_off(40);
      led_on(40);
      led_off(40);
      led_on(40);
      led_off(0);
      result = false;
      break;
    case ADV_ID0:
      restart_advertising_id(0);
      led_advertise(0);
      result = false;
      break;
    case ADV_ID1:
      restart_advertising_id(1);
      led_advertise(1);
      result = false;
      break;
    case ADV_ID2:
      restart_advertising_id(2);
      led_advertise(2);
      result = false;
      break;
    case ADV_ID3:
      restart_advertising_id(3);
      led_advertise(3);
      result = false;
      break;
    case ADV_ID4:
      restart_advertising_id(4);
      led_advertise(4);
      result = false;
      break;
    case DEL_ID0:
      delete_bond_id(0);
      led_delete(0);
      result = false;
      break;
    case DEL_ID1:
      delete_bond_id(1);
      led_delete(1);
      result = false;
      break;
    case DEL_ID2:
      delete_bond_id(2);
      led_delete(2);
      result = false;
      break;
    case DEL_ID3:
      delete_bond_id(3);
      led_delete(3);
      result = false;
      break;
    case BATT_LV:
      // this doesn't really work since the nrfmicro uses a voltage regulator.
      // the voltage is always at 3.3v
      sprintf(str, "%4dmV", get_vcc());
      send_string(str);
      result = false;
      break;
    case ENT_DFU:
      led_on(500);
      led_off(0);
      sd_power_gpregret_set(0, DFU_MAGIC_UF2_RESET);
      bootloader_jump();
      result = false;
      break;
    }
  }
  else if (!record->event.pressed) {
    switch (keycode) {
    case ENT_SLP:
      sleep_mode_enter();
      result = false;
      break;
    }
  }
  
  // put your per-action keyboard code here
  // runs for every action, just before processing by the firmware
  if (result) {
    result = process_record_user(keycode, record);
  }
  else {
    layer_state = 0;
  }
  return result;
}

void led_on(int delay_ms) {
    nrf_gpio_pin_set(LED_PIN);
    if (delay_ms > 0)
      nrf_delay_ms(delay_ms);
}

void led_off(int delay_ms) {
    nrf_gpio_pin_clear(LED_PIN);
    if (delay_ms > 0)
      nrf_delay_ms(delay_ms);
}

void led_advertise(int peer_id) {
  led_on(250);
  for (int i = 0; i < peer_id + 1; i++) {
    led_off(100);
    led_on(100);
  }
  led_off(0);
}

void led_delete(int peer_id) {
  led_on(50);
  led_off(50);
  led_on(250);
  for (int i = 0; i < peer_id + 1; i++) {
    led_off(100);
    led_on(100);
  }
  led_off(0);
}

#define USB_ENABLE_MASK 0x8
#define BLE_ENABLE_MASK 0x10

void numbersetc_init_usb() {
    set_usb_enabled((eeconfig_read_user() & USB_ENABLE_MASK) ? true : false);
}

bool numbersetc_get_usb_enabled() {
    return (eeconfig_read_user() & USB_ENABLE_MASK) ? true : false;
}

void numbersetc_set_usb_enabled(bool enabled) {
    set_usb_enabled(enabled ? true : false);
    eeconfig_update_user((eeconfig_read_user() & ~USB_ENABLE_MASK) | (enabled ? USB_ENABLE_MASK : 0));
}

void numbersetc_init_ble() {
    set_ble_enabled(numbersetc_get_ble_enabled());
}

bool numbersetc_get_ble_enabled() {
    return (eeconfig_read_user() & BLE_ENABLE_MASK) ? true : false;
}

void numbersetc_set_ble_enabled(bool enabled) {
    set_ble_enabled(enabled ? true : false);
    eeconfig_update_user((eeconfig_read_user() & ~BLE_ENABLE_MASK) | (enabled ? BLE_ENABLE_MASK : 0));
}