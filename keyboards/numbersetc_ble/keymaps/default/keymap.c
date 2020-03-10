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

extern keymap_config_t keymap_config;

// Fillers to make layering more clear
#define _______ KC_TRNS
#define XXXXXXX KC_NO
#define FN_NLCK LT(1, KC_NLCK)

// https://github.com/qmk/qmk_firmware/blob/master/docs/keycodes.md

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_4X6(
    FN_NLCK,   KC_PSLS,   KC_PAST,   KC_PMNS, \
    KC_P7,     KC_P8,     KC_P9,     KC_PPLS, \
    KC_P4,     KC_P5,     KC_P6,     _______, \
    KC_P1,     KC_P2,     KC_P3,     KC_PENT, \
    KC_P0,     _______,   KC_PDOT,   _______ \
  ),
  [1] = LAYOUT_4X6(
    _______,   USB_DIS,   USB_EN,    BLE_DIS, \
    _______,   DELBNDS,   ENT_DFU,   BLE_EN, \
    ADV_ID4,   _______,   BATT_LV,   _______, \
    ADV_ID1,   ADV_ID2,   ADV_ID3,   ENT_DFU, \
    ADV_ID0,   _______,   DEL_IDC,   _______ \
  )
};

const uint16_t PROGMEM fn_actions[] = {

};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  return true;
}

