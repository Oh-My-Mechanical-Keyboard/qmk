#include "pad_ble_new_oled.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrf_power.h"
#include "nrfx_power.h"
#include "nrf.h"
#include "app_ble_func.h"
#undef PACKED



#ifdef RGBLIGHT_ENABLE
//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;
#endif
bool ble_soft_set = false;

// adafruit bootloader, send "dfu" to debug serial port
#define DFU_MAGIC_UF2_RESET             0x57
void bootloader_jump(void) {
  sd_power_gpregret_set(0, DFU_MAGIC_UF2_RESET);
  NVIC_SystemReset();
}

static bool ble_flag = false;

void nrfmicro_power_enable(bool enable) {
	if (enable) {
		nrf_gpio_cfg_output(POWER_PIN);	//disable power pin feature
		nrf_gpio_pin_set(POWER_PIN);
	} else {
		
		nrf_gpio_cfg_output(POWER_PIN); //disable power pin feature
		nrf_gpio_pin_clear(POWER_PIN); // works
	}
}

void check_ble_switch(bool init) {
	uint8_t value = nrf_gpio_pin_read(SWITCH_PIN);

	if (init || ble_flag != value) {
		ble_flag = value;

		// mind that it doesn't disable BLE completely, it only disables send
		set_usb_enabled(!ble_flag);
		set_ble_enabled(ble_flag);

		nrf_gpio_pin_clear(LED_PIN);

		if (ble_flag) {
			// blink twice on ble enabled
			for (int i=0; i<2; i++) {
				nrf_gpio_pin_set(LED_PIN);
				nrf_delay_ms(100);
				nrf_gpio_pin_clear(LED_PIN);
				nrf_delay_ms(100);
			}
		}
	}
}

//switch to usb mode when usb cable connected
void check_usb_power()
{
	if (nrfx_power_usbstatus_get() == NRFX_POWER_USB_STATE_CONNECTED ||
    	nrfx_power_usbstatus_get() == NRFX_POWER_USB_STATE_READY) {
		ble_disconnect();
		if(!get_usb_enabled() && get_ble_enabled())
		{
    		set_usb_enabled(true);
			set_ble_enabled(false);
		}
  	} else {
  		if(get_usb_enabled() && !get_ble_enabled())
		{
    		set_usb_enabled(false);
			set_ble_enabled(true);
			restart_advertising_wo_whitelist();
		}
  	}
}

void eeprom_update(void);

void nrfmicro_init() {
  // configure pins
  nrf_gpio_cfg_output(POWER_PIN);
  nrf_gpio_cfg_output(LED_PIN);
  nrf_gpio_cfg_input(SWITCH_PIN, NRF_GPIO_PIN_PULLDOWN);

//   nrf_gpio_cfg_input(PIN12, NRF_GPIO_PIN_PULLDOWN);	//detect charging pin

  nrf_delay_ms(100);

  // eeconfig_read_rgblight();

  check_ble_switch(true);

}

void update_caps_led()
{
	if (host_keyboard_leds() & (1<<USB_LED_CAPS_LOCK)){
		nrf_gpio_pin_set(LED_PIN);
	} else {
		nrf_gpio_pin_clear(LED_PIN);
	}

}

void nrfmicro_update() {
  // check_ble_switch(false);
	if (!ble_soft_set) {
  		// check_usb_power();
	} else {
		if (get_usb_enabled() && !get_ble_enabled()) {
			ble_disconnect();
		}
	}
	eeprom_update(); 
}

void keyboard_post_init_kb()
{
	nrfmicro_power_enable(false); //power pin
  if(rgblight_config.enable)
  {
  	nrfmicro_power_enable(true); //power pin
  }

}





