/* Copyright 2020 Chasingendgame
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

#define KC_VLUP KC__VOLUP
#define KC_VLDN KC__VOLDOWN

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap Base Layer
   * ,-------------------.  ,-------------------.
   * | Q | W | E | R | T |  | Y | U | I | O | P |
   * |-------------------|  |-------------------|
   * | A | S | D | F | G |  | H | J | K | L |Ent|
   * |-------------------|  |-------------------|
   * | Fn| Z | X | C | V |  | B | N | M | . |Shf|
   * |-------------------|  |-------------------|
   * |Ctl|Win|Alt| Raise |  | Space |Alt|Ctl|Bks|
   * `-------------------'  `-------------------'
   */
[0] = LAYOUT_10X4(
  KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     \
  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_ENT,   \
  MO(2),    KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_DOT,   KC_LSFT,  \
  KC_LCTL,  KC_LGUI,  KC_LALT,  KC_NO,    MO(1),    KC_NO,    KC_SPC,   KC_RALT,  KC_LGUI,  KC_BSPC),

/* Keymap Symbol/Navigation (Raise) Layer
   * ,-------------------.  ,-------------------.
   * | 1 | 2 | 3 | 4 | 5 |  | 6 | 7 | 8 | 9 | 0 |
   * |-------------------|  |-------------------|
   * | ` | \ | Up|   |Hom|  | [ | - | ; | ' |Ent|
   * |-------------------|  |-------------------|
   * | Fn| Lt| Dn| Rt|End|  | ] | = | , | / |Shf|
   * |-------------------|  |-------------------|
   * |   |   |   | Raise |  | Space |   |   |Del|
   * `-------------------'  `-------------------'
   */
[1] = LAYOUT_10X4(
  KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     \
  KC_GRV,   KC_BSLS,  KC_UP,    KC_NO,    KC_HOME,  KC_LBRC,  KC_MINS,  KC_SCLN,  KC_QUOT,  KC_ENT,   \
  MO(3),    KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_END,   KC_RBRC,  KC_EQL,   KC_COMM,  KC_TRNS,  KC_LSFT,  \
  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  MO(1),    KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_DEL),

/* Keymap Function (Fn) Layer
   * ,-------------------.  ,-------------------.
   * |Esc| F1| F2| F3| F4|  |   |   |Scl|Pau|Psc|
   * |-------------------|  |-------------------|
   * | ` | F5| F6| F7| F8|  |   |   |   |   |Ent|
   * |-------------------|  |-------------------|
   * | Fn| F9|F10|F11|F12|  | V-| V+| Vx|   |Shf|
   * |-------------------|  |-------------------|
   * |   |   |   |  Cfg  |  |       |   |   |   |
   * `-------------------'  `-------------------'
   */
[2] = LAYOUT_10X4(
  KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_NO,    KC_NO,    KC_SLCK,  KC_PAUS,  KC_PSCR,  \
  KC_TAB,   KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_ENT,   \
  MO(2),    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_VLDN,  KC_VLUP,  KC__MUTE, KC_TRNS,  KC_LSFT,  \
  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  MO(3),    KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS),

/* Keymap Config (Cfg) Layer */
[3] = LAYOUT_10X4(
  ADV_ID1,  ADV_ID2,  ADV_ID3,  ADV_ID4,  AD_WO_L,  KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,   \
  DEL_ID1,  DEL_ID2,  DEL_ID3,  DEL_ID4,  DELBNDS,  KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,   \
  KC_NO,    USB_DIS,  USB_EN,   BLE_DIS,  BLE_EN,   KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,   \
  ENT_DFU,  KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO),

};

static bool is_slsh_down = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    /* This makes the "." key "?" when shifted (instead of ">") */
  case KC_DOT:
    if (record->event.pressed) {
      bool is_lsft_down = (get_mods() & MOD_LSFT);
      if ((is_lsft_down && layer_state == 0) || (!is_lsft_down && layer_state != 0)) {
        SEND_STRING(SS_DOWN(X_SLASH));
        is_slsh_down = 1;
        return false;
      }
    }
    else if (is_slsh_down) {
      is_slsh_down = 0;
      SEND_STRING(SS_UP(X_SLASH));
      return false;
    }
    break;
  }
  return true;
}
