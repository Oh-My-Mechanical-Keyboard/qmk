#ifndef MASTER_CONFIG_H_
#define MASTER_CONFIG_H_

#include "custom_board.h"

#define THIS_DEVICE_ROWS 5
#define THIS_DEVICE_COLS 5

#define MATRIX_ROW_PINS { PIN26, PIN25, PIN13, PIN12, PIN21 }
#define MATRIX_COL_PINS { PIN18, PIN16, PIN10, PIN11, PIN8 }

#ifdef ENCODER_ENABLE
	#define ENCODERS_PAD_A { PIN9 }
	#define ENCODERS_PAD_B { PIN22 }
#endif
// #define OLED_DISABLE_TIMEOUT 0
#define OLED_FONT_H "keyboards/pad_ble_new_oled/lib/glcdfont.c"

#define TAP_CODE_DELAY 10
#define IS_LEFT_HAND  true
#define ENABLE_STARTUP_ADV_NOLIST

#endif /* MASTER_CONFIG_H_ */
