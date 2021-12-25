// Copyright 2021 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "winry315.h"

#if !defined(VIA_ENABLE) && defined(ENCODER_ENABLE)
#    ifndef MEDIA_KEY_DELAY
#        define MEDIA_KEY_DELAY 10
#    endif
bool encoder_update_kb(uint8_t index, bool clockwise) {
    if (!encoder_update_user(index, clockwise)) {
        return false;
    }
    if (index == 0) { /* Left encoder  */
        if (clockwise) {
            tap_code(KC_PGDN);
        } else {
            tap_code(KC_PGUP);
        }
    } else if (index == 1) { /* Center encoder */
        if (clockwise) {
            tap_code_delay(KC_VOLU, MEDIA_KEY_DELAY);
        } else {
            tap_code_delay(KC_VOLD, MEDIA_KEY_DELAY);
        }
    } else if (index == 2) { /* Right encoder */
        if (clockwise) {
            tap_code_delay(KC_MNXT, MEDIA_KEY_DELAY);
        } else {
            tap_code_delay(KC_MPRV, MEDIA_KEY_DELAY);
        }
    }
    return true;
}
#endif

#ifdef RGB_MATRIX_ENABLE

// LED mapping:
//   0 - right encoder, top right
//   1 - right encoder, top left
//   2 - center encoder, top right
//   3 - center encoder, top left
//   4 - left encoder, top right
//   5 - left encoder, top left
//   6 - row 0, column 0
//   7 - row 1, column 0
//   8 - row 2, column 0
//   9 - row 2, column 1
//  10 - row 1, column 1
//  11 - row 0, column 1
//  12 - row 0, column 2
//  13 - row 1, column 2
//  14 - row 2, column 2
//  15 - row 2, column 3
//  16 - row 1, column 3
//  17 - row 0, column 3
//  18 - row 0, column 4
//  19 - row 1, column 4
//  20 - row 2, column 4
//  21 - underglow, right bottom
//  22 - underglow, left bottom
//  23 - underglow, left middle
//  24 - underglow, left top
//  25 - underglow, right top
//  26 - underglow, right middle

#    define WIDTH_MM 94
#    define HEIGHT_MM 90
#    define WIDTH_UNITS (112 * 2)
#    define HEIGHT_UNITS (32 * 2)
#    define LED(x_mm, y_mm) \
        { (WIDTH_MM / 2 + x_mm) * WIDTH_UNITS / WIDTH_MM, (HEIGHT_MM / 2 - y_mm) * HEIGHT_UNITS / HEIGHT_MM }

// clang-format off
led_config_t g_led_config = {
    {
        { 6, 11, 12, 17, 18, 7, 10, 13, 16, 19, 8, 9, 14, 15, 20, 2, 0, 4, NO_LED, NO_LED, NO_LED, NO_LED, NO_LED, NO_LED }
    },
    {
        LED( 35,  36),
        LED( 21,  36),
        LED( 8,   34),
        LED(-8,   34),
        LED(-21,  36),
        LED(-35,  36),
        LED(-38,   5),
        LED(-38, -14),
        LED(-38, -33),
        LED(-19, -33),
        LED(-19, -14),
        LED(-19,   5),
        LED(  0,   5),
        LED(  0, -14),
        LED(  0, -33),
        LED( 19, -33),
        LED( 19, -14),
        LED( 19,   5),
        LED( 38,   5),
        LED( 38, -14),
        LED( 38, -33),
        LED( 28, -35),
        LED(-28, -35),
        LED(-37,  -9),
        LED(-42,  40),
        LED( 42,  40),
        LED( 37,  -9)
    },
    {
        1, 1, 1, 1, 1, 1, // use modifier color for encoders
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, // regular keys
        2, 2, 2, 2, 2, 2 // underglow
    }
};
// clang-format on

#endif
