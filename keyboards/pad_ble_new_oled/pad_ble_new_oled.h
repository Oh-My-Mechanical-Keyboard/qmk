#ifndef KEYBOARDS_PAD_BLE_NEW_OLED_H_
#define KEYBOARDS_PAD_BLE_NEW_OLED_H_

#include "quantum.h"
#include "timer.h"

extern bool ble_soft_set;

#define LAYOUT( \
	                    K04, \
	K10, K11, K12, K13, K14, \
	K20, K21, K22, K23, K24, \
	K30, K31, K32, K33, K34, \
	K40, K41, K42, K43, K44  \
) { \
	{ KC_NO, KC_NO, KC_NO, KC_NO, K04 }, \
	{ K10,   K11,   K12,   K13,   K14 }, \
	{ K20,   K21,   K22,   K23,   K24 }, \
	{ K30,   K31,   K32,   K33,   K34 }, \
	{ K40,   K41,   K42,   K43,   K44 }  \
}

#endif /* KEYBOARDS_PAD_BLE_NEW_OLED_H_ */
