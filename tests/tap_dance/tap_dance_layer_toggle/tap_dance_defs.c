// Copyright 2022 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "quantum.h"
#include "tap_dance_defs.h"

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case CUST_XY:
            if (record->event.pressed) {
                tap_code(KC_X);
            } else {
                tap_code(KC_Y);
            }
            return false;
        case CUST_ZW:
            if (record->event.pressed) {
                tap_code(KC_Z);
            } else {
                tap_code(KC_W);
            }
            return false;
    }
    return true;
}

qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_L_TOGG] = ACTION_TAP_DANCE_LAYER_TOGGLE(KC_ESC, 1),
};
