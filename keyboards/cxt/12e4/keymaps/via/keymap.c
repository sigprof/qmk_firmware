// Copyright 2022 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_A,    KC_B,    KC_C,    KC_D,      KC_0,        KC_1,
        KC_E,    KC_F,    KC_G,    KC_H,            KC_2,
        KC_I,    KC_J,    KC_K,    KC_L,            KC_3
    )
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [0] = {
        ENCODER_CCW_CW(KC_WH_L, KC_WH_R),  ENCODER_CCW_CW(KC_WH_U, KC_WH_D),
                        ENCODER_CCW_CW(KC_PGUP, KC_PGDN),
                        ENCODER_CCW_CW(KC_VOLD, KC_VOLU),
    },
};
#endif
