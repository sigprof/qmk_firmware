/*
 * Copyright 2021 Sergey Vlasov <sigprof@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID    0xFEED
#define PRODUCT_ID   0x6480
#define DEVICE_VER   0x0001
#define MANUFACTURER QMK

/*
 * Key matrix configuration parameters are set in config.h files for specific
 * boards, and then the matrix configuration is finalized in post_config.h.
 */

/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCE 5

/* define if matrix has ghost (lacks anti-ghosting diodes) */
//#define MATRIX_HAS_GHOST

/* disable these deprecated features by default */
#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION

/* Disable features which cannot be used by this “keyboard”. */
#define NO_ACTION_TAPPING
#define NO_ACTION_LAYER

/*
 * This should be at least 2 to handle both events for a direct connection of
 * two pins without a diode in a single scan.
 */
#define QMK_KEYS_PER_SCAN 4
