// Copyright 2021 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_BASE] = LAYOUT_all(
            KC_HOME,      KC_MUTE,     KC_MPLY,
        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,
        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,
        KC_ESC,  KC_TAB,  KC_SPC,  KC_BSPC, KC_ENT
    ),
};

#if 0
bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { /* Left encoder  */
        if (clockwise) {
            tap_code(KC_A);
        } else {
            tap_code(KC_B);
        }
    } else if (index == 1) { /* Center encoder */
        if (clockwise) {
            tap_code(KC_C);
        } else {
            tap_code(KC_D);
        }
    } else if (index == 2) { /* Right encoder */
        if (clockwise) {
            tap_code(KC_E);
        } else {
            tap_code(KC_F);
        }
    }
    return false;
}
#endif
