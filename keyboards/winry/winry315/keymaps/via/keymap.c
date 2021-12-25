// Copyright 2021 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#define LAYOUT_via(             \
      k17,     k15,    k16,     \
    k23,k22, k19,k18, k21,k20,  \
     k00, k01, k02, k03, k04,   \
     k05, k06, k07, k08, k09,   \
     k10, k11, k12, k13, k14    \
) { \
    { k00, k01, k02, k03, k04, k05, k06, k07, k08, k09, k10, k11, k12, k13, k14, k15, k16, k17, k18, k19, k20, k21, k22, k23 } \
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_via(
            KC_HOME,           KC_MUTE,           KC_MPLY,
        KC_PGUP, KC_PGDN,  KC_VOLD, KC_VOLU,  KC_MPRV, KC_MNXT,
             KC_1,    KC_2,    KC_3,    KC_4,    KC_5,
             KC_6,    KC_7,    KC_8,    KC_9,    KC_0,
             KC_ESC,  KC_TAB,  KC_SPC,  KC_BSPC, KC_ENT
    ),
    [1 ... 7] = LAYOUT_via(
            KC_TRNS,           KC_TRNS,           KC_TRNS,
        KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS,
             KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
             KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
             KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),
};
