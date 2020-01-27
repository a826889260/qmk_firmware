/* Copyright 2018 Eucalyn
 * Copyright 2018 Sekigon
 * Copyright 2019 Gachiham
 * Copyright 2020 Chasingendgame
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

#include "app_ble_func.h"
#include "nrf_gpio.h"

enum custom_keycodes {
    AD_WO_L = SAFE_RANGE, /* Start advertising without whitelist  */
    BLE_DIS,              /* Disable BLE HID sending              */
    BLE_EN,               /* Enable BLE HID sending               */
    USB_DIS,              /* Disable USB HID sending              */
    USB_EN,               /* Enable USB HID sending               */
    DELBNDS,              /* Delete all bonding                   */
    ADV_ID0,              /* Start advertising to PeerID 0        */
    ADV_ID1,              /* Start advertising to PeerID 1        */
    ADV_ID2,              /* Start advertising to PeerID 2        */
    ADV_ID3,              /* Start advertising to PeerID 3        */
    ADV_ID4,              /* Start advertising to PeerID 4        */
    BATT_LV,              /* Display battery level in milli volts */
    DEL_ID0,              /* Delete bonding of PeerID 0           */
    DEL_ID1,              /* Delete bonding of PeerID 1           */
    DEL_ID2,              /* Delete bonding of PeerID 2           */
    DEL_ID3,              /* Delete bonding of PeerID 3           */
    DEL_ID4,              /* Delete bonding of PeerID 4           */
    ENT_DFU,              /* Start bootloader                     */
    ENT_SLP,              /* Deep sleep mode                      */
    RGBRST,
};

extern keymap_config_t keymap_config;

enum {
  BASE=0,
  META,
  SYMB,
  CTRL,
  ABS_MOUSE,
  GAME
};

// Fillers to make layering more clear
#define _______ KC_TRNS
#define XXXXXXX KC_NO
#define FN_NLCK LT(1, KC_NLCK)

// https://github.com/qmk/qmk_firmware/blob/master/docs/keycodes.md

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT(
    FN_NLCK,   KC_PSLS,   KC_PAST,   KC_PMNS, \
    KC_P7,     KC_P8,     KC_P9,     KC_PPLS, \
    KC_P4,     KC_P5,     KC_P6,     _______, \
    KC_P1,     KC_P2,     KC_P3,     KC_PENT, \
    KC_P0,     _______,   KC_PDOT,   _______ \
  ),
  [1] = LAYOUT(
    _______,   USB_DIS,   USB_EN,    BLE_DIS, \
    _______,   _______,   ENT_DFU,   BLE_EN, \
    ADV_ID4,   AD_WO_L,   _______,   _______, \
    ADV_ID1,   ADV_ID2,   ADV_ID3,   ENT_DFU, \
    ADV_ID0,   _______,   DELBNDS,   _______ \
  )
};

const uint16_t PROGMEM fn_actions[] = {

};

// Inverted to save battery life

void led_on(int delay_ms) {
    nrf_gpio_pin_clear(LED_PIN);
    if (delay_ms > 0)
      nrf_delay_ms(delay_ms);
}

void led_off(int delay_ms) {
    nrf_gpio_pin_set(LED_PIN);
    if (delay_ms > 0)
      nrf_delay_ms(delay_ms);
}

void led_advertise(int peer_id) {
  led_off(500);
  for (int i = 0; i < peer_id; i++) {
    led_on(120);
    led_off(120);
  }
  led_on(120);
  led_off(500);
  led_on(0);
}

void led_delete(int peer_id) {
  led_off(500);
  for (int i = 0; i < peer_id; i++) {
    led_on(240);
    led_off(240);
  }
  led_on(240);
  led_off(500);
  led_on(0);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  char str[16];

  bool result = true;
  
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
      result = false;
      break;
    case USB_DIS:
      set_usb_enabled(false);
      result = false;
      break;
    case BLE_EN:
      set_ble_enabled(true);
      led_off(120);
      led_on(120);
      led_off(120);
      led_on(0);
      result = false;
      break;
    case BLE_DIS:
      set_ble_enabled(false);
      led_off(240);
      led_on(120);
      led_off(240);
      led_on(0);
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
      sprintf(str, "%4dmV", get_vcc());
      send_string(str);
      result = false;
      break;
    case ENT_DFU:
      led_off(240);
      led_on(480);
      led_off(240);
      led_on(0);
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

  return result;
}

