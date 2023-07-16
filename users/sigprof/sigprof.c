// Copyright 2023 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "sigprof.h"

_Static_assert(sizeof(custom_eeconfig_user_t) == CUSTOM_EECONFIG_USER_SIZE, "CUSTOM_EECONFIG_USER_SIZE does not match the actual size");
#ifdef CUSTOM_EECONFIG_KEYMAP_SIZE
_Static_assert(sizeof(custom_eeconfig_keymap_t) == CUSTOM_EECONFIG_KEYMAP_SIZE, "CUSTOM_EECONFIG_KEYMAP_SIZE does not match the actual size");
#endif
_Static_assert(sizeof(user_eeconfig_t) == EECONFIG_USER_DATA_SIZE, "EECONFIG_USER_DATA_SIZE does not match the actual size");

user_eeconfig_t user_config;

__attribute__((weak)) void keyboard_post_init_keymap(void) {}

void keyboard_post_init_user(void) {
    eeconfig_read_user_datablock(&user_config);
    keyboard_post_init_keymap();
}

void update_user_config(void) {
    eeconfig_update_user_datablock(&user_config);
}

static deferred_token click_spam_token = INVALID_DEFERRED_TOKEN;
static bool click_spam_enabled;

static uint32_t click_spam_callback(uint32_t trigger_time, void *cb_arg) {
    if (click_spam_enabled) {
        tap_code(KC_BTN1);
        return 100;
    }
    click_spam_token = INVALID_DEFERRED_TOKEN;
    return 0;
}

static bool process_record_click_spam(uint16_t keycode, keyrecord_t *record) {
    if (keycode == U_BTN1R) {
        click_spam_enabled = record->event.pressed;
        if (click_spam_enabled) {
            if (click_spam_token == INVALID_DEFERRED_TOKEN) {
                click_spam_token = defer_exec(1, click_spam_callback, NULL);
            }
        } else {
            if (click_spam_token != INVALID_DEFERRED_TOKEN) {
                cancel_deferred_exec(click_spam_token);
                click_spam_token = INVALID_DEFERRED_TOKEN;
            }
        }
    }
    return true;
}

__attribute__((weak)) bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    return (process_record_keymap(keycode, record)
#ifdef CUSTOM_LANG_SWITCH_ENABLE
            && process_record_lang_switch(keycode, record)
#endif
            && process_record_click_spam(keycode, record)
    );
}
