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

enum custom_lt_state {
    CLT_NONE,
    CLT_SINGLE_TAP,
    CLT_SINGLE_HOLD,
    CLT_DOUBLE_HOLD,
};

static enum custom_lt_state saved_custom_lt_state;

static enum custom_lt_state get_custom_lt_state(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (!state->pressed) {
            return CLT_SINGLE_TAP;
        } else {
            return CLT_SINGLE_HOLD;
        }
    } else if (state->count == 2) {
        return CLT_DOUBLE_HOLD;
    } else {
        return CLT_NONE;
    }
}

static void custom_lt_finished(qk_tap_dance_state_t *state, void *user_data) {
    saved_custom_lt_state = get_custom_lt_state(state);
    switch (saved_custom_lt_state) {
        case CLT_NONE:
            break;
        case CLT_SINGLE_TAP:
            register_code(KC_APP);
            break;
        case CLT_SINGLE_HOLD:
            layer_on(1);
            break;
        case CLT_DOUBLE_HOLD:
            register_code(KC_RCTL);
            break;
    }
}

static void custom_lt_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (saved_custom_lt_state) {
        case CLT_NONE:
            break;
        case CLT_SINGLE_TAP:
            unregister_code(KC_APP);
            break;
        case CLT_SINGLE_HOLD:
            layer_off(1);
            break;
        case CLT_DOUBLE_HOLD:
            unregister_code(KC_RCTL);
            break;
    }
}

qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_CUST_LT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, custom_lt_finished, custom_lt_reset),
};
