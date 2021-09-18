#pragma once

#include "config_common.h"

// Configure the IS31FL3733 driver for per-key RGB LEDs
#define DRIVER_COUNT 1
#define DRIVER_ADDR_1 0b1010000
#define DRIVER_1_LED_TOTAL 64 /* FIXME */

#define ISSI_LED_TOTAL DRIVER_1_LED_TOTAL

// Underglow LEDs are WS2812, but someone might want to use RGBLIGHT for them;
// don't use those LEDs in RGB Matrix in that case.
#ifdef RGBLIGHT_ENABLE
#    define WS2812_LED_TOTAL 0
#else
#    define WS2812_LED_TOTAL RGBLED_NUM
#endif

#define DRIVER_LED_TOTAL (ISSI_LED_TOTAL + WS2812_LED_TOTAL)
