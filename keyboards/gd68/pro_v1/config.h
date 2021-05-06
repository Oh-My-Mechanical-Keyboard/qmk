#ifndef MASTER_CONFIG_H_
#define MASTER_CONFIG_H_

#include "custom_board.h"

#define THIS_DEVICE_ROWS 5
#define THIS_DEVICE_COLS 15
// {0.10, 0.12, 0.00, 0.02, 0.03}
// {1.06, 1.04 , 0.22, 0.20, 0.08, 0.06, 1.09, 0.05, 0.01, 0.30, 0.31, 0.29, 0.28, 1.13, 1.11}
#define MATRIX_ROW_PINS {PIN12, PIN24, PIN3, PIN17, PIN15}
#define MATRIX_COL_PINS {PIN11, PIN21 , PIN22, PIN7, PIN2, PIN1, PIN26, PIN25, PIN4, PIN20, PIN19, PIN18, PIN14, PIN16, PIN13}

#ifdef ENCODER_ENABLE
	#define ENCODERS_PAD_A { PIN8 }
	#define ENCODERS_PAD_B { PIN9 }
#endif
// #define OLED_DISABLE_TIMEOUT 0
#define OLED_FONT_H "keyboards/pad_ble_new_oled/lib/glcdfont.c"

#define TAP_CODE_DELAY 10
#define IS_LEFT_HAND  true
#define ENABLE_STARTUP_ADV_NOLIST

#endif /* MASTER_CONFIG_H_ */
