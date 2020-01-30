/*
Copyright 2018 Sekigon

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "matrix.h"

#include "wait.h"
#include "app_ble_func.h"
#include "bootloader.h"

#include "nrf.h"
#include "nrf_power.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"

#include "numbersetc_ble.h"

// From tmk_core/protocol/nrf/matrix.c

void unselect_cols(void);
void select_col(uint8_t col);
matrix_col_t read_rows(void);
matrix_col_t read_col(uint8_t col);

static bool bootloader_flag = false;

void matrix_init_user() {

  // blink (2x)on power on
  nrf_gpio_cfg_output(LED_PIN);
  nrf_gpio_cfg_input(SWITCH_PIN, NRF_GPIO_PIN_PULLDOWN);

  for (int i = 0; i < 2; i++) {
    led_on(100);
    led_off(100);
  }
  
  select_col(1);
  wait_us(50);
  matrix_col_t col = read_rows();
  unselect_cols();
  
  if (col == 0b10) {
    // hold down middle key on second row at power-on to delete bond with master
    delete_bonds();
  } else if (col == 0b1) {
    // hold down middle key on top row at power-on to enter UF2 bootloader
    bootloader_flag = true;
  }
}

void matrix_scan_user() {
  static int cnt;
  if (bootloader_flag && cnt++==400) {
    sd_power_gpregret_set(0, 0x57); // DFU_MAGIC_UF2_RESET
    bootloader_jump();
  }
}
