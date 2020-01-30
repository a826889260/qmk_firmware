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

#include "numbersetc_ble.h"

__attribute__ ((weak))
void matrix_init_user() {
}

void matrix_init_kb() {
  set_usb_enabled(true);

  // blink on power on
  nrf_gpio_cfg_output(LED_PIN);
  nrf_gpio_cfg_input(SWITCH_PIN, NRF_GPIO_PIN_PULLDOWN);

  for (int i = 0; i < 3; i++) {
    led_on(100);
    led_off(100);
  }
  
  // default settings
  int peer_id = eeconfig_get_advertising_channel();
  //bool usb_enabled = eeconfig_get_usb_enabled();
  bool ble_enabled = eeconfig_get_ble_enabled();
  
  led_advertise(peer_id);
  //set_usb_enabled(usb_enabled);
  set_ble_enabled(ble_enabled);
  
  if (ble_enabled) {
    led_off(500);
    led_advertise(peer_id);
  }
  
  matrix_init_user();
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
  char str[16];

  bool result = true;
  
  int peer_id = 0;
  
  if (record->event.pressed) {
    switch (keycode) {
    case DELBNDS:
      delete_bonds();
      led_delete(5);
      result = false;
      break;
    case AD_WO_L:
      restart_advertising_wo_whitelist();
      led_advertise(5);
      result = false;
      break;
    case USB_EN:
      set_usb_enabled(true);
      eeconfig_set_usb_enabled(true);
      result = false;
      break;
    case USB_DIS:
      set_usb_enabled(false);
      eeconfig_set_usb_enabled(false);
      result = false;
      break;
    case BLE_EN:
      set_ble_enabled(true);
      eeconfig_set_ble_enabled(true);
      led_advertise(eeconfig_get_advertising_channel());
      result = false;
      break;
    case BLE_DIS:
      set_ble_enabled(false);
      eeconfig_set_ble_enabled(false);
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
      eeconfig_set_advertising_channel(0);
      led_advertise(0);
      result = false;
      break;
    case ADV_ID1:
      restart_advertising_id(1);
      eeconfig_set_advertising_channel(1);
      led_advertise(1);
      result = false;
      break;
    case ADV_ID2:
      restart_advertising_id(2);
      eeconfig_set_advertising_channel(2);
      led_advertise(2);
      result = false;
      break;
    case ADV_ID3:
      restart_advertising_id(3);
      eeconfig_set_advertising_channel(3);
      led_advertise(3);
      result = false;
      break;
    case ADV_ID4:
      restart_advertising_id(4);
      eeconfig_set_advertising_channel(4);
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
    case DEL_IDC:
      peer_id = eeconfig_get_advertising_channel();
      delete_bond_id(peer_id);
      led_delete(peer_id);
      result = false;
      break;
    case BATT_LV:
      sprintf(str, "%4dmV", get_vcc());
      send_string(str);
      result = false;
      break;
    case ENT_DFU:
      led_on(480);
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
  for (int i = 0; i < peer_id; i++) {
    led_on(120);
    led_off(120);
  }
  led_on(120);
  led_off(0);
}

void led_delete(int peer_id) {
  for (int i = 0; i < peer_id; i++) {
    led_on(240);
    led_off(240);
  }
  led_on(240);
  led_off(0);
}

void eeconfig_set_advertising_channel(int peer_id) {
  eeconfig_update_user((eeconfig_read_user() & ~0x7) | (peer_id & 0x7));
}

int eeconfig_get_advertising_channel() {
  return (eeconfig_read_user() & 0x7);
}

void eeconfig_set_usb_enabled(bool enabled) {
  eeconfig_update_user((eeconfig_read_user() & ~0x8) | (enabled ? 0x8 : 0x0));
}

bool eeconfig_get_usb_enabled() {
  return (eeconfig_read_user() & 0x8) ? 1 : 0;
}

void eeconfig_set_ble_enabled(bool enabled) {
  eeconfig_update_user((eeconfig_read_user() & ~0x10) | (enabled ? 0x8 : 0x0));
}

bool eeconfig_get_ble_enabled() {
  return (eeconfig_read_user() & 0x10) ? 1 : 0;
}