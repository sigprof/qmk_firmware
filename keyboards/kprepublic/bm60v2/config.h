#pragma once

#include "config_common.h"

#ifdef RGB_MATRIX_ENABLE
#    define DRIVER_COUNT 1
#    define DRIVER_ADDR_1 0b1010000
#    define DRIVER_1_LED_TOTAL 64
#    define DRIVER_LED_TOTAL DRIVER_1_LED_TOTAL
#endif
