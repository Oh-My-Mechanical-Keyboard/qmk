
#include QMK_KEYBOARD_H
#include <stdio.h>
#include "crkbd.h"

// in the future, should use (1U<<_LAYER_NAME) instead, but needs to be moved to keymap,c
#define L_BASE 0
#define L_DIR 1
#define L_RGB 2
#define L_SYS 3
#define L_BLE 4


char layer_state_str[24];

const char *read_layer_state(void) {
  switch (biton32(layer_state))
  {
  case L_BASE:
    snprintf(layer_state_str, sizeof(layer_state_str), " Layer: Base");
    break;
  case L_DIR:
    snprintf(layer_state_str, sizeof(layer_state_str), " Layer: Function");
    break;
  case L_RGB:
    snprintf(layer_state_str, sizeof(layer_state_str), " Layer: Light");
    break;
  case L_SYS:
    snprintf(layer_state_str, sizeof(layer_state_str), " Layer: System");
    break;
  case L_BLE:
    snprintf(layer_state_str, sizeof(layer_state_str), " Layer: Bluetooth");
    break;
  default:
    snprintf(layer_state_str, sizeof(layer_state_str), " Layer: Undef-%ld", layer_state);
  }

  return layer_state_str;
}
