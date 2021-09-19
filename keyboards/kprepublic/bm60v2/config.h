#pragma once

#include "config_common.h"

// Note: The RGB LED driver configuration is not inside #ifdef blocks, so that
// the driver code would still compile even without RGBLIGHT_ENABLE and
// RGB_MATRIX_ENABLE (usually this is not required, but with the custom
// IS31FL3733+WS2812 driver setup used by this board the LED controller drivers
// are compiled unconditionally).

// Configure the IS31FL3733 driver for per-key RGB LEDs
#define DRIVER_COUNT 1
#define DRIVER_ADDR_1 0b1010000
#define DRIVER_1_LED_TOTAL 63

#define ISSI_LED_TOTAL DRIVER_1_LED_TOTAL

// Underglow LEDs are WS2812, but someone might want to use RGBLIGHT for them;
// don't use those LEDs in RGB Matrix in that case.
#ifdef RGBLIGHT_ENABLE
#    define WS2812_LED_TOTAL 0
#else
#    define WS2812_LED_TOTAL RGBLED_NUM
#endif

#define DRIVER_LED_TOTAL (ISSI_LED_TOTAL + WS2812_LED_TOTAL)
