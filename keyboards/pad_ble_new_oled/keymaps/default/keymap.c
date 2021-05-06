/*
Copyright 2018 Sekigon
Copyright 2019 hatano.h

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

#include QMK_KEYBOARD_H
#include "app_ble_func.h"
#include "nrf_gpio.h"
#include "app_usbd.h"


#ifdef WPM_ENABLE
  #include "wpm.h"
#endif

#include "pad_ble_new_oled.h"
#include "eeprom.h"
#include "string.h"
#include "eeconfig.h"
#include "action.h"
#include "timer.h"
#include "host.h"
#include "app_timer.h"


const uint8_t is_master = IS_LEFT_HAND;

// int RGB_current_mode;
#ifdef RGBLIGHT_ENABLE
//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;
void nrfmicro_power_enable(bool enable);
#endif

#ifdef OS_CYCLE
uint8_t current_os=0;
#endif

enum custom_keycodes {
    // BLE keys
    NORM = SAFE_RANGE,	  /* Normal mode */
    AD_WO_L,		  /* Start advertising without whitelist  */
    SEL_BLE,		  /* Select BLE HID Sending		  */
    SEL_USB,		  /* Select USB HID Sending		  */
    TOG_HID,		  /* Toggle BLE/USB HID Sending		  */
    DELBNDS,              /* Delete all bonding                   */
    ADV_ID0,              /* Start advertising to PeerID 0        */
    ADV_ID1,              /* Start advertising to PeerID 1        */
    ADV_ID2,              /* Start advertising to PeerID 2        */
    ADV_ID3,              /* Start advertising to PeerID 3        */
    ADV_ID4,              /* Start advertising to PeerID 4        */
    ADV_ID5,              /* Start advertising to PeerID 5        */
    BATT_LV,              /* Display battery level in milli volts */
    DEL_ID0,              /* Delete bonding of PeerID 0           */
    DEL_ID1,              /* Delete bonding of PeerID 1           */
    DEL_ID2,              /* Delete bonding of PeerID 2           */
    DEL_ID3,              /* Delete bonding of PeerID 3           */
    DEL_ID4,              /* Delete bonding of PeerID 4           */
    DEL_ID5,              /* Delete bonding of PeerID 5           */
    ENT_DFU,              /* Start bootloader                     */
    ENT_SLP,              /* Deep sleep mode                      */

    RGBRST,
    RGBTOG,
    CST_MVP,
    PRINTER,
    OS_SWITCH,
    VOLUP
};

enum { TO_SETTINGS=0 };

extern keymap_config_t keymap_config;

enum layer_number {
    _BASE = 0,
    _DIR,
    _RGB_SETTINGS,
    _SYS,
    _BLE_SETTINGS
};

// Fillers to make layering more clear
#define _______ KC_TRNS
#define XXXXXXX KC_NO

void dance_layer_TO_SETTINGS(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 3) {  // TAP THREE TIMES TO ENTER SETTINGS
        layer_on(_SYS);
        reset_tap_dance (state);
    } else
    {
    	register_code (KC_GRV);
    }
}
void dance_layer_TO_SETTINGS_RESET(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 3) {  // TAP THREE TIMES TO ENTER SETTINGS
    } else
    {
    	unregister_code (KC_GRV);
    }
}

qk_tap_dance_action_t tap_dance_actions[] = {[TO_SETTINGS] = ACTION_TAP_DANCE_FN_ADVANCED(NULL,dance_layer_TO_SETTINGS,dance_layer_TO_SETTINGS_RESET)};

// define keymaps
const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE]=LAYOUT(
    KC_MUTE,
    KC_7, KC_8, KC_9, KC_PPLS, TO(_BLE_SETTINGS),
    KC_4, KC_5, KC_6, KC_PMNS, TO(2),
    KC_1, KC_2, KC_3, KC_PAST, TO(3),
    KC_0, M(0), KC_DOT, KC_PSLS, KC_EQL),

    [_DIR]=LAYOUT(
    KC_TRNS,
    TO(_BASE), KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_UP, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_LEFT, KC_DOWN, KC_RGHT, KC_TRNS, KC_TRNS),

    [_RGB_SETTINGS]=LAYOUT(
    KC_TRNS,
    TO(_BASE), KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    RGB_TOG, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),

    [_SYS]=LAYOUT(
    KC_TRNS,
    TO(_BASE), TO(_BLE_SETTINGS), KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_PGUP, KC_HOME, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_PGDN, KC_END, KC_TRNS, KC_TRNS),

    [_BLE_SETTINGS]=LAYOUT(
    TO(_BASE),
    ADV_ID0, ADV_ID1, AD_WO_L, SEL_BLE, SEL_USB,
    DEL_ID0, DEL_ID1, DELBNDS, SEL_USB, DELBNDS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, AD_WO_L,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
};
const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt) {
  // keyevent_t event = record->event;

  switch (id) {
    case 0:
      if (record->event.pressed) {
        return MACRO( T(0), T(0), END );
      }
      break;
  }
  return MACRO_NONE;
}

#ifndef TAPPING_TERM_PER_KEY
#define get_tapping_term(kc)	(TAPPING_TERM)
#endif

const char *read_layer_state(void);
const char *read_logo(void);
void set_keylog(uint16_t keycode, keyrecord_t *record);
const char *read_keylog(void);
const char *read_keylogs(void);

static bool process_record_user_special(uint16_t keycode, bool pressed) {
  switch (keycode) {
  case NORM:
    if (pressed) {
      set_single_persistent_default_layer(_BASE);
    }
    break;
  case DELBNDS:
    if (pressed)
      delete_bonds();
    break;
  case AD_WO_L:
    if (pressed)
      restart_advertising_wo_whitelist();
    break;
  case SEL_BLE:
    if (pressed) {
      select_ble();
      ble_soft_set = false;
      restart_advertising_wo_whitelist();
    }
    break;
  case SEL_USB:
    if (pressed) {
      select_usb();
      ble_soft_set = true;
    }
    break;
  case TOG_HID:
#ifndef NRF_SEPARATE_KEYBOARD_SLAVE
    // workaround:
    // get_ble_enabled() macro(in app_ble_func.h) is incorrect.
    if (pressed) {
      bool ble = get_ble_enabled();
      if (ble) {
        select_usb();
        ble_disconnect();
      } else {
        select_ble();
      }
    }
#endif
    break;
  case ADV_ID0:
  case ADV_ID1:
  case ADV_ID2:
  case ADV_ID3:
  case ADV_ID4:
  case ADV_ID5:
    if (pressed) {
      select_ble();
      restart_advertising_id(keycode-ADV_ID0);
    }
    break;
  case DEL_ID0:
  case DEL_ID1:
  case DEL_ID2:
  case DEL_ID3:
  case DEL_ID4:
  case DEL_ID5:
    if (pressed)
      delete_bond_id(keycode-DEL_ID0);
    break;
  case BATT_LV:
    if (pressed) {
      char str[16];
      sprintf(str, "%4dmV", get_vcc());
      send_string(str);
    }
    break;
  case ENT_DFU:
    if (pressed)
      bootloader_jump();
    break;
  case ENT_SLP:
    break;

  default:
    // other unspecial keys
    return true;
  }
  return true;
}

char bat_state_str[24];
char bat_percentage_str[24];
void set_bat_state(void) {
  uint8_t value = nrf_gpio_pin_read(SWITCH_PIN);

  if(value)
  {
    snprintf(bat_state_str, sizeof(bat_state_str), "VOLT: %4d MV",
           get_vcc());
    snprintf(bat_percentage_str, sizeof(bat_percentage_str), "&': %d %%",
           (get_vcc()-2400)/18);
  } else {
    snprintf(bat_state_str, sizeof(bat_state_str), "VOLT: CHECK SWITCH");
    snprintf(bat_percentage_str, sizeof(bat_percentage_str), "&': CHECK SWITCH");
  }

}

const char *read_bat_state(void) {
  return bat_state_str;
}

const char *read_bat_percentage(void) {
  return bat_percentage_str;
}

char hid_state_str[24];
const char *read_hid_state(void) {
#if defined IS_LEFT_HAND  &&  IS_LEFT_HAND == true
  snprintf(hid_state_str, sizeof(hid_state_str), "(): %s %s",
           (get_usb_enabled()) ? "!\"" : " ",
           (get_ble_enabled()) ? "#$" : " ");
#endif
  return hid_state_str;
}

#ifdef WPM_ENABLE
char wpm_str[24];
void set_wpm(void) {
    snprintf(wpm_str, sizeof(wpm_str), "*+: %d", get_current_wpm());
}
const char *read_wpm(void) {
  return wpm_str;
}
#endif

#ifdef RGBLIGHT_ENABLE
char rgb_state_str[24];
const char *read_rgb_state(void)
{
  snprintf(rgb_state_str, sizeof(rgb_state_str), "LIGHT %s MODE: %d",
    rgblight_config.enable ? "ON " : "OFF", rgblight_config.mode);
  return rgb_state_str;
}
#endif

char mod_shift_win_str[24];
const char *read_shift_win_state(void)
{
  snprintf(mod_shift_win_str, sizeof(mod_shift_win_str), "%s %s",
    get_mods() & MOD_BIT(KC_LSHIFT)? "]^" : "  ",
    get_mods() & MOD_BIT(KC_LGUI)? "\\" : " " );
  return mod_shift_win_str;
}

char mod_ctrl_alt_str[24];
const char *read_ctrl_alt_state(void)
{
  snprintf(mod_ctrl_alt_str, sizeof(mod_ctrl_alt_str), "%s %s",
    get_mods() & MOD_BIT(KC_LCTL)? "_" : " ",
    get_mods() & MOD_BIT(KC_LALT)? "{" : " " );
  return mod_ctrl_alt_str;
}

char version_str[24];
const char *read_version(void)
{
  snprintf(version_str, sizeof(version_str), "VER 0510 EEPROM");
  return version_str;
}

//caps lock state not working

char caps_lock_state[24];
const char *read_caps_lock_state(void)
{
  uint8_t leds = host_keyboard_leds();
  snprintf(caps_lock_state, sizeof(caps_lock_state), "CL:%s",
    (leds & (1 << USB_LED_CAPS_LOCK)) ? "1" : "0");
  return caps_lock_state;
}

char battery_charging_state[24];
const char *read_battery_charging_state(void)
{
  // if (nrfx_power_usbstatus_get() != NRFX_POWER_USB_STATE_CONNECTED ||
  //     nrfx_power_usbstatus_get() != NRFX_POWER_USB_STATE_READY) {
  //   snprintf(battery_charging_state, sizeof(battery_charging_state), "NO USB");
  //   return battery_charging_state;
  // }
  uint8_t value=nrf_gpio_pin_read(SWITCH_PIN);
  if(!value){ //switch not open
     snprintf(battery_charging_state, sizeof(battery_charging_state), "CHECK SWITCH");
     return battery_charging_state;
  }
  // if(nrf_gpio_pin_read(PIN12)){ // high: charged
  //   snprintf(battery_charging_state, sizeof(battery_charging_state), "CHARGE DONE");
  // }else{
  //   snprintf(battery_charging_state, sizeof(battery_charging_state), "CHARGING");
  // }
  return battery_charging_state;
}



bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  #ifdef OLED_DRIVER_ENABLE
    oled_on();
  #endif
  switch(keycode)
  {
    static bool caps_state=0;
    case KC_CAPS:
    if (record->event.pressed) {
      if(caps_state){
        caps_state=!caps_state;
        nrf_gpio_pin_clear(LED_PIN);
      }
      else{
        caps_state=!caps_state;
        nrf_gpio_pin_set(LED_PIN);
      }
    }
    break;
    case RGBRST:
      #ifdef RGBLIGHT_ENABLE
        if (record->event.pressed) {
          nrfmicro_power_enable(true);
          eeconfig_update_rgblight_default();
          rgblight_enable();
          // RGB_current_mode = rgblight_config.mode;
          // NRF_LOG_INFO("RGBRST, RGB_current_mode: %d\n", RGB_current_mode);
        }
      #endif
      break;

    case RGB_TOG:
      #ifdef RGBLIGHT_ENABLE
        if (record->event.pressed) {
          if(rgblight_config.enable) {
            nrfmicro_power_enable(false);
            // eeconfig_update_rgblight_default();
            // rgblight_enable();
          }
          else{
            nrfmicro_power_enable(true);
            // rgblight_disable();
          }
        }
      #endif
      break;
    #ifdef OS_CYCLE
    case PRINTER:
        if (record->event.pressed) {
          current_os=eeconfig_read_os();
          if(current_os==0){
            register_code(KC_0);
            unregister_code(KC_0);
          }else{
            register_code(KC_1);
            unregister_code(KC_1);
          }
        }
      break;
    #endif
    #ifdef OS_CYCLE
    case OS_SWITCH:

        if (record->event.pressed) {
          current_os=eeconfig_read_os();
          if(current_os==0){
            current_os=1;
            eeconfig_update_os(current_os);
          }else{
            current_os=0;
            eeconfig_update_os(current_os);
          }
        }
      break;
      #endif
    case VOLUP:
        if (record->event.pressed) {
        	register_code(KC_VOLU);
        	unregister_code(KC_VOLU);

          // register_code(KC_A);
        	// unregister_code(KC_A);
          // wait_us(3000);

          // register_code(KC_B);
        	// unregister_code(KC_B);
          // uint32_t held_keycode_timer = timer_read32();
          // register_code(KC_A);
        	// unregister_code(KC_A);
          // while ((timer_read32()- held_keycode_timer < 1000)){
          //   app_usbd_event_queue_process();
          // }
          // register_code(KC_B);
        	// unregister_code(KC_B);
        }
      break;
  }
  if (record->event.pressed) {
    set_bat_state();
    // set_keylog(keycode, record);
    #ifdef WPM_ENABLE
      update_wpm(keycode);
      set_wpm();
    #endif
    // eeprom_write_dword(EECONFIG_RGBLIGHT, 666);`
  }
  switch (keycode) {
  default:
    // unset_layer(record);
    return process_record_user_special(keycode, record->event.pressed);
  }
  return true;
}

#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    //   oled_scroll_set_area(0,2);
    // oled_scroll_set_speed(4);
    // oled_scroll_right();
    return OLED_ROTATION_0;
}
void oled_task_user() {
    switch(biton32(layer_state))
    {
      case _BASE:
      case _DIR:
        oled_write_ln(read_layer_state(), false);
        oled_write_ln(read_hid_state(), false);
        oled_write_ln(read_bat_percentage(), false);
        #ifdef WPM_ENABLE
          oled_write_ln(read_wpm(), false);
        #else
          oled_write_ln("JasonRen_Pad", false);
        #endif
        // oled_write_ln(ead_shift_win_state(, false);
        // matrix_write(matrix,read_ctrl_alt_state(, false);
        //others:
        // matrix_write(matrix,read_caps_lock_state(, false);
        // oled_write_ln(read_keylog(, false);
        // oled_write_ln(read_keylogs(, false);
        //oled_write_ln(read_mode_icon(keymap_config.swap_lalt_lgui, false);
        //oled_write_ln(read_host_led_state(, false);
        //oled_write_ln(read_timelog(, false);
      break;
      case _SYS:
        oled_write_ln(",- =>          [ESC", false);
        oled_write_ln("B: ENTER BOOT", false);
        oled_write_ln(read_version(), false);
      break;
      case _BLE_SETTINGS:
        oled_write_ln(",- #$          [ESC", false);
        oled_write_ln("T: TOGGLE HID", false);
        oled_write_ln("DEL: DEL BONDS", false);
        oled_write_ln("JasonRen_Pad", false);
      break;
      case _RGB_SETTINGS:
      #ifdef RGBLIGHT_ENABLE
        oled_write_ln(",- ;<          [ESC", false);
        oled_write_ln(read_rgb_state(), false);
        oled_write_ln("T: TOGGLE E: RESET ", false);
        oled_write_ln( "M: MODE HUE:ENCODER", false);
      #else
        oled_write_ln(",- ;<          [ESC", false);
        oled_write_ln("RGB NOT SUPPORTED", false);
        oled_write_ln("JasonRen_Pad", false);
        oled_write_ln("", false);

      #endif
      break;
      default:
      oled_write_ln("ERROR LAYER", false);
      oled_write_ln("JasonRen_Pad", false);
      oled_write_ln("JasonRen_Pad", false);
      oled_write_ln("JasonRen_Pad", false);
      break;
    }
}

#endif


keyevent_t encoder_cw  = {
    .key = (keypos_t){.row = 1, .col = 1},
    .pressed = false
};

keyevent_t encoder_ccw  = {
    .key = (keypos_t){.row = 1, .col = 1},
    .pressed = false
};
uint8_t encoder_cw_kc = KC_NO;
uint8_t encoder_ccw_kc = KC_NO;


static void my_timeout_handler_cw (void * p_context){
				// unregister_code(KC_VOLU);
	encoder_cw.pressed = false;
  encoder_rot = true;
    encoder_cw.time = (timer_read() | 1); 
    action_exec(encoder_cw, encoder_cw_kc);
}
static void my_timeout_handler_ccw (void * p_context){
				// unregister_code(KC_VOLU);
	encoder_ccw.pressed = false;
  encoder_rot = true;

    encoder_ccw.time = (timer_read() | 1); 
    action_exec(encoder_ccw, encoder_ccw_kc);
}


//Media keys WORKS
#ifdef ENCODER_ENABLE
void encoder_update_user(uint8_t index, bool clockwise) {
  switch(biton32(layer_state))
  {
    case _BASE:
      if(clockwise){
        encoder_cw_kc = KC_VOLU;
        encoder_cw.pressed = true;
        encoder_rot = true;

        encoder_cw.time = (timer_read() | 1);  //time should not be 0 
        action_exec(encoder_cw, encoder_cw_kc);
        APP_TIMER_DEF(my_timer_id);
        app_timer_create(&my_timer_id, APP_TIMER_MODE_SINGLE_SHOT, my_timeout_handler_cw);
        app_timer_start(my_timer_id, APP_TIMER_TICKS(16), NULL); 
       // usbd_send_consumer(KEYCODE2CONSUMER(KC_AUDIO_VOL_DOWN));

          // uint32_t held_keycode_timer = timer_read32();
          // register_code(KC_A);
        	// unregister_code(KC_A);
          // while (timer_elapsed32(held_keycode_timer) < 2000){ /* no-op */ }
          // register_code(KC_B);
        	// unregister_code(KC_B);
        // register_code(KC_VOLU);
        // wait_ms(10);
        // unregister_code(KC_VOLU);
      }
      else{
        encoder_ccw_kc = KC_VOLD;
        encoder_ccw.pressed = true;
        encoder_rot = true;

        encoder_ccw.time = (timer_read() | 1);  //time should not be 0 
        action_exec(encoder_ccw, encoder_ccw_kc);
        APP_TIMER_DEF(my_timer_id);   
        app_timer_create(&my_timer_id, APP_TIMER_MODE_SINGLE_SHOT, my_timeout_handler_ccw);
        app_timer_start(my_timer_id, APP_TIMER_TICKS(16), NULL);
 
        // usbd_send_consumer(KEYCODE2CONSUMER(KC_AUDIO_VOL_UP));
        // register_code(KC_VOLD);
        // wait_ms(10);
        // unregister_code(KC_VOLD);
      }
      break;
    case _DIR:
      if(clockwise){
        register_code16(KC_PGUP);
        unregister_code16(KC_PGUP);
      }
      else{
        register_code16(KC_PGDN);
        unregister_code16(KC_PGDN);
      }
      break;
    case _SYS:
    case _BLE_SETTINGS:
    break;
    case _RGB_SETTINGS:
      if (get_mods() & MOD_BIT(KC_LALT)) {
        if(clockwise){
          rgblight_decrease_val();
        } else {
          rgblight_increase_val();
        }
      } else if (get_mods() & MOD_BIT(KC_LCTL)){
        if(clockwise){
          rgblight_decrease_sat();
        } else {
          rgblight_increase_sat();
        }
      } else {
        if(clockwise){
          rgblight_decrease_hue();
        } else {
          rgblight_increase_hue();
        }
      }
      break;
    default:
    break;
  }
}
#endif



// void led_set_user(uint8_t usb_led) {
//     if (usb_led & (1 << USB_LED_CAPS_LOCK)) {
//         // PORTB &= ~(1 << 2);
//         nrf_gpio_pin_set(LED_PIN);
//     } else {
//         // PORTB |= (1 << 2);
//         nrf_gpio_pin_clear(LED_PIN);
//     }

//     // led_set_user(usb_led);
// }


// void led_set_kb(uint8_t usb_led) {
//   // put your keyboard LED indicator (ex: Caps Lock LED) toggling code here
//   if (IS_LED_ON(usb_led, USB_LED_CAPS_LOCK)) {
//     nrf_gpio_pin_set(LED_PIN);
//   } else {
//     nrf_gpio_pin_clear(LED_PIN);
//   }

//   led_set_user(usb_led);
// }

