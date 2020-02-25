#ifndef SLAVE_CONFIG_H_
#define SLAVE_CONFIG_H_

#include "custom_board.h"

#define THIS_DEVICE_ROWS 5
#define MATRIX_ROW_PINS { PIN12, PIN13, PIN14, PIN15, PIN16 }
#define THIS_DEVICE_COLS 4
#define MATRIX_COL_PINS { PIN20, PIN19, PIN18, PIN17 }

#define IS_LEFT_HAND false
#define IS_SLAVE true

#endif /* SLAVE_CONFIG_H_ */
