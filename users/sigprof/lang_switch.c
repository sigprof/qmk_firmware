// Copyright 2023 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "sigprof.h"

// Get the base keycode for language switching.  The assumption is that the
// keycode by itself activates Group 0, and the keycode combined with the right
// Shift activates Group 1.
static uint16_t get_lsw_keycode(void) {
    switch (user_config.user.lang_switch_mode) {
        case 0:
        default:
            return KC_CAPS;

        case 1:
            return C(KC_F15);
    }
}

// Process the custom keycodes for the language switch feature:
//   - `U_LSWM0` - set language switch mode 0 (CapsLock and Shift+CapsLock);
//   - `U_LSWM1` - set language switch mode 1 (Ctrl+F15 and Shift+Ctrl+F15).
bool process_record_lang_switch(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case U_LSWM0:
            if (record->event.pressed) {
                user_config.user.lang_switch_mode = 0;
                update_user_config();
            }
            return false;

        case U_LSWM1:
            if (record->event.pressed) {
                user_config.user.lang_switch_mode = 1;
                update_user_config();
            }
            return false;
    }

    return true;
}

static void td_lang_shift_press(tap_dance_state_t *state, uint8_t mod_kc) {
    // For these tap dances we don't actually want to capture the modifier
    // state at the tap time.
    state->weak_mods    = 0;
    state->oneshot_mods = 0;

    register_code(mod_kc);
}

static void td_lang_shift_release(tap_dance_state_t *state, uint8_t mod_kc, uint16_t lsw_keycode) {
    unregister_code(mod_kc);

    if (state->count == 2) {
        tap_code16(lsw_keycode);
        reset_tap_dance(state);
    }
}

void td_lsft_lsw_press(tap_dance_state_t *state, void *user_data) {
    td_lang_shift_press(state, KC_LSFT);
}

void td_lsft_lsw_release(tap_dance_state_t *state, void *user_data) {
    td_lang_shift_release(state, KC_LSFT, get_lsw_keycode());
}

void td_rsft_lsw_press(tap_dance_state_t *state, void *user_data) {
    td_lang_shift_press(state, KC_RSFT);
}

void td_rsft_lsw_release(tap_dance_state_t *state, void *user_data) {
    td_lang_shift_release(state, KC_RSFT, RSFT(get_lsw_keycode()));
}
