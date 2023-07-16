// Copyright 2023 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <quantum.h>

// Custom EEPROM configuration defined by the userspace.  The size of this
// struct must match the value of `CUSTOM_EECONFIG_USER_SIZE` in `config.h`.
typedef struct __attribute__((packed)) {
    uint8_t lang_switch_mode : 1;
} custom_eeconfig_user_t;

// The structure of the user EEPROM datablock, which includes both the
// userspace part and the optional keymap specific part.
typedef struct __attribute__((packed)) {
    custom_eeconfig_user_t user;
#ifdef CUSTOM_EECONFIG_KEYMAP_SIZE
    custom_eeconfig_keymap_t keymap;
#endif
} user_eeconfig_t;

// The current copy of the persistent configuration.
extern user_eeconfig_t user_config;

void update_user_config(void);

bool process_record_keymap(uint16_t keycode, keyrecord_t *record);

enum user_keycodes {
    DUMMY_USER_KEYCODE = QK_USER - 1,
    U_BTN1R, // Repeat `KC_BTN1` taps while the key is held down
#ifdef CUSTOM_LANG_SWITCH_ENABLE
    U_LSWM0, // Set language switch mode 0 (Caps Lock)
    U_LSWM1, // Set language switch mode 1 (Ctrl+F15)
#endif
    KEYMAP_SAFE_RANGE,
};

#ifdef CUSTOM_LANG_SWITCH_ENABLE

bool process_record_lang_switch(uint16_t keycode, keyrecord_t *record);

void td_lsft_lsw_press(tap_dance_state_t *state, void *user_data);
void td_lsft_lsw_release(tap_dance_state_t *state, void *user_data);
void td_rsft_lsw_press(tap_dance_state_t *state, void *user_data);
void td_rsft_lsw_release(tap_dance_state_t *state, void *user_data);

#    define ACTION_LANG_SWITCH_LSFT ACTION_TAP_DANCE_FN_ADVANCED_WITH_RELEASE(td_lsft_lsw_press, td_lsft_lsw_release, NULL, NULL)
#    define ACTION_LANG_SWITCH_RSFT ACTION_TAP_DANCE_FN_ADVANCED_WITH_RELEASE(td_rsft_lsw_press, td_rsft_lsw_release, NULL, NULL)

#endif
