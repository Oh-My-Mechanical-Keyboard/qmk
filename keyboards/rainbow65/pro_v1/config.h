#ifndef MASTER_CONFIG_H_
#define MASTER_CONFIG_H_

#include "custom_board.h"

#define THIS_DEVICE_ROWS 5
#define THIS_DEVICE_COLS 15

#define MATRIX_ROW_PINS { PIN10, PIN26, PIN25, PIN4, PIN2 }
#define MATRIX_COL_PINS { PIN11, PIN21, PIN22, PIN7, PIN24, PIN3, PIN1, PIN15, PIN13, PIN14, PIN16, PIN17, PIN18, PIN19, PIN12 }

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
