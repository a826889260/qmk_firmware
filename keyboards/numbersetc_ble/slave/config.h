#ifndef SLAVE_CONFIG_H_
#define SLAVE_CONFIG_H_

#include "custom_board.h"

// Rows are doubled on split configurations
#undef MATRIX_ROWS
#define MATRIX_ROWS 10

#define MATRIX_ROW_PINS { PIN12, PIN13, PIN14, PIN15, PIN16 }
#define MATRIX_COL_PINS { PIN20, PIN19, PIN18, PIN17 }

#define IS_LEFT_HAND false
#define IS_SLAVE true

#endif /* SLAVE_CONFIG_H_ */
