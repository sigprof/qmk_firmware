/* Copyright 2020 Sergey Vlasov <sigprof@gmail.com>
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
#include QMK_KEYBOARD_H

enum layer_names {
    _QWERTY,
    _NUMPAD,
    _FN,
    _FN_CTL,
    _ADJUST,
};

enum tap_dance_ids {
    TD_RALT,
    TD_RCTL,
};

#define U_FCAPS LT(_FN, KC_CAPS)
#define U_FAPP  LT(_FN, KC_APP)
#define U_TRALT TD(TD_RALT)
#define U_TRCTL TD(TD_RCTL)
#define U_NBSLS LT(_NUMPAD, KC_BSLS)
#define U_MOADJ OSL(_ADJUST) //MO(_ADJUST)
#define U_TGNUM TG(_NUMPAD)
#define U_OSRGU OSM(MOD_RGUI)
#define U_OSRCT OSM(MOD_RCTL)
#define U_CPGUP RCTL(KC_PGUP)
#define U_CPGDN RCTL(KC_PGDN)
#define U_FNCTL LM(_FN_CTL, MOD_LCTL)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ANSI QWERTY keyboard adapted to the ID80 physical layout.
     *
     * Two keys in the right column are used as PgUp and PgDn (using the top
     * row key as PgDn turned out to be really inconvenient).  Two keys in the
     * top row (separated by 0.25U) are used as Ins and Del (although the Del
     * key is located far from the main block, reversing the order of the
     * Ins/Del pair does not seem to be good).
     *
     * There are only two modifier keys on the right side of the spacebar,
     * which perform multiple functions through tap dances; here they are
     * labeled as “TRAlt” and “TRCtl”, but the primary function of the “TRCtl”
     * key is switching to the Fn layer.
     *
     * On the left side the Caps Lock key is also overloaded to work as Fn on
     * hold and Caps Lock on tap.
     *
     * ┌───┐┌───┬───┬───┬───┐┌───┬───┬───┬───┐┌───┬───┬───┬───┐┌───┐┌───┐
     * │Esc││F1 │F2 │F3 │F4 ││F5 │F6 │F7 │F8 ││F9 │F10│F11│F12││Ins││Del│
     * └───┘└───┴───┴───┴───┘└───┴───┴───┴───┘└───┴───┴───┴───┘└───┘└───┘
     * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐┌───┐
     * │ ` │ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 0 │ - │ = │ Backsp││PgU│
     * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤├───┤
     * │ Tab │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │ [ │ ] │  \  ││PgD│
     * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤└───┘
     * │CapsFn│ A │ S │ D │ F │ G │ H │ J │ K │ L │ ; │ ' │  Enter │
     * ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────┬───┘
     * │ Shift  │ Z │ X │ C │ V │ B │ N │ M │ , │ . │ / │ Shift│┌───┐
     * ├────┬───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┴┬──┴──┬───┘│ ↑ │
     * │Ctrl│GUI │Alt │                        │TRAlt│TRCtl│┌───┼───┼───┐
     * └────┴────┴────┴────────────────────────┴─────┴─────┘│ ← │ ↓ │ → │
     *                                                      └───┴───┴───┘
     */

    [_QWERTY] = LAYOUT(
        KC_ESC,    KC_F1,   KC_F2,   KC_F3,   KC_F4,     KC_F5,   KC_F6,   KC_F7,   KC_F8,     KC_F9,   KC_F10,  KC_F11,  KC_F12,     KC_INS,    KC_DEL,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,            KC_PGUP,
        KC_TAB,      KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, U_NBSLS,        KC_PGDN,
        U_FCAPS,       KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,
        KC_LSFT,            KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,          KC_UP,
        KC_LCTL,   KC_LGUI,   KC_LALT,                       KC_SPC,                              U_TRALT,     U_TRCTL,        KC_LEFT, KC_DOWN, KC_RGHT
    ),

    /*
     * Numeric keypad emulation layer.
     * Unused alphanumeric keys are turned off.
     */
    [_NUMPAD] = LAYOUT(
        _______,   _______, _______, _______, _______,   _______, _______, _______, _______,   _______, _______, _______, _______,    _______,   _______,
        XXXXXXX, KC_P1,   KC_P2,   KC_P3,   KC_P4,   KC_P5,   KC_P6,   KC_P7,   KC_P8,   KC_P9,   KC_P0,   KC_PMNS, KC_PPLS, _______,            _______,
        _______,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_P4,   KC_P5,   KC_P6,   KC_PAST, KC_PENT, KC_NLCK, KC_NLCK,        _______,
        _______,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_P1,   KC_P2,   KC_P3,   KC_PPLS, KC_PAST, KC_PENT,
        _______,            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_P0,   KC_PCMM, KC_PDOT, KC_PSLS, _______,          _______,
        _______,   _______,   _______,                      _______,                              _______,     _______,        _______, _______, _______
    ),

    /*
     * Function layer:
     * - mouse emulation
     * - cursor and nav cluster keys (for compatibility with 60%)
     * - one-shot RCtrl and RGUI
     * - Num Lock key
     * - toggle number pad emulation layer
     * - momentary switch for the number pad emulation layer (because of layer
     *   ordering, using that switch requires releasing the key which was used
     *   to switch to the function layer)
     * - media keys
     */
    [_FN] = LAYOUT(
        XXXXXXX,   KC_F13,  KC_F14,  KC_F15,  KC_F16,    KC_F17,  KC_F18,  KC_F19,  KC_F20,    KC_F21,  KC_F22,  KC_F23,  KC_F24,     KC_HOME,   KC_END,
        KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,             U_CPGUP,
        XXXXXXX,     KC_BTN1, KC_MS_U, KC_BTN2, KC_WH_U, DM_PLY1, KC_INS,  KC_HOME, KC_UP,   KC_END,  KC_PGUP, KC_VOLU, KC_MUTE, U_TGNUM,        U_CPGDN,
        _______,       KC_MS_L, KC_MS_D, KC_MS_R, KC_WH_D, DM_PLY2, KC_DEL,  KC_LEFT, KC_DOWN, KC_RGHT, KC_PGDN, KC_VOLD, KC_PENT,
        _______,            KC_BTN3, KC_BTN4, KC_BTN5, KC_WH_L, KC_WH_R, KC_PSCR, KC_SLCK, KC_PAUS, U_OSRGU, U_OSRCT, _______,          KC_PGUP,
        U_FNCTL,   _______,   _______,                      U_MOADJ,                              _______,     _______,        KC_HOME, KC_PGDN, KC_END
    ),

    /*
     * Fn+Ctrl layer - dynamic macro management:
     * - Fn+Ctrl+Esc - stop recording;
     * - Fn+Ctrl+T   - start recording macro 1;
     * - Fn+Ctrl+G   - start recording macro 2.
     */
    [_FN_CTL] = LAYOUT(
        DM_RSTP,   _______, _______, _______, _______,   _______, _______, _______, _______,   _______, _______, _______, _______,    _______,   _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,            _______,
        _______,     _______, _______, _______, _______, DM_REC1, _______, _______, _______, _______, _______, _______, _______, _______,        _______,
        _______,       _______, _______, _______, _______, DM_REC2, _______, _______, _______, _______, _______, _______, _______,
        _______,            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______,   _______,   _______,                      _______,                              _______,     _______,        _______, _______, _______
    ),

    /*
     * Adjust layer:
     * - backlight control
     * - RGB underglow control
     * - reset to bootloader
     * - EEPROM reset
     * - NKRO config
     */
    [_ADJUST] = LAYOUT(
        RESET,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX, NK_OFF,  NK_ON,   DEBUG,      XXXXXXX,   XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, NK_TOGG, EEP_RST,            XXXXXXX,
        XXXXXXX,     BL_TOGG, BL_DEC,  BL_INC,  BL_BRTG, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,        XXXXXXX,
        _______,       RGB_TOG, RGB_MOD, RGB_HUI, RGB_SAI, RGB_VAI, RGB_SPI, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        _______,            RGB_M_P, RGB_RMOD,RGB_HUD, RGB_SAD, RGB_VAD, RGB_SPD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,          XXXXXXX,
        _______,   _______,   _______,                      _______,                              _______,     _______,        XXXXXXX, XXXXXXX, XXXXXXX
    ),
};

static struct {
    bool group_led : 1;
} indicators;

static void update_indicators(void) {
    if (indicators.group_led) {
        backlight_set(get_backlight_level());
    } else {
        backlight_set(0);
    }
}

void keyboard_post_init_user(void) {
    rgblight_set_effect_range(0, 16);
    update_indicators();
}

bool led_update_user(led_t led_state) {
    // Assume that "grp_led:scroll" is used to make the Scroll Lock LED
    // indicate the input language (XKB group).
    if (indicators.group_led != led_state.scroll_lock) {
        indicators.group_led = led_state.scroll_lock;
        update_indicators();
    }

    // Keep the default Caps Lock LED function
    return true;
}

bool get_clear_oneshot_layer(uint16_t keycode, keyrecord_t *record, bool default_value) {
    switch (keycode) {
        case RGB_TOG:
        case RGB_MOD:
        case RGB_HUI:
        case RGB_SAI:
        case RGB_VAI:
        case RGB_SPI:
            return false;

        case RGB_M_P:
        case RGB_RMOD:
        case RGB_HUD:
        case RGB_SAD:
        case RGB_VAD:
        case RGB_SPD:
            return true;
    }
    return default_value;
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case U_FCAPS:
        case U_FAPP:
            return true;

        default:
            return false;
    }
}

static const uint16_t PROGMEM combo_prtsc[] = { KC_F9, KC_F10, COMBO_END };
static const uint16_t PROGMEM combo_scrlk[] = { KC_F10, KC_F11, COMBO_END };
static const uint16_t PROGMEM combo_pause[] = { KC_F11, KC_F12, COMBO_END };
static const uint16_t PROGMEM combo_fapp[]  = { KC_INS, KC_DEL, COMBO_END };

combo_t key_combos[] = {
    COMBO(combo_prtsc, KC_PSCR),
    COMBO(combo_scrlk, KC_SLCK),
    COMBO(combo_pause, KC_PAUS),
    COMBO(combo_fapp,  U_FAPP),
};

uint16_t COMBO_LEN = sizeof(key_combos) / sizeof(key_combos[0]);

// Tap dance for the “Right Alt” key:
// - hold: KC_RALT
// - tap: KC_RALT
// - tap and hold: KC_RGUI
// - double tap: KC_RGUI
// - double tap and hold: KC_RGUI+KC_RALT
// - triple tap: KC_RGUI+KC_RALT
enum td_ralt_state {
    TD_RALT_NOOP,
    TD_RALT_RALT,
    TD_RALT_RGUI,
    TD_RALT_RGUI_RALT,
};

static enum td_ralt_state td_ralt_state;

static void td_ralt_finished(qk_tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            td_ralt_state = TD_RALT_RALT;
            break;
        case 2:
            td_ralt_state = TD_RALT_RGUI;
            break;
        case 3:
            td_ralt_state = TD_RALT_RGUI_RALT;
            break;
        default:
            td_ralt_state = TD_RALT_NOOP;
            break;
    }

    switch (td_ralt_state) {
        case TD_RALT_NOOP:
            break;
        case TD_RALT_RALT:
            register_code(KC_RALT);
            break;
        case TD_RALT_RGUI:
            register_code(KC_RGUI);
            break;
        case TD_RALT_RGUI_RALT:
            register_code(KC_RGUI);
            register_code(KC_RALT);
            break;
    }
}

static void td_ralt_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (td_ralt_state) {
        case TD_RALT_NOOP:
            break;
        case TD_RALT_RALT:
            unregister_code(KC_RALT);
            break;
        case TD_RALT_RGUI:
            unregister_code(KC_RGUI);
            break;
        case TD_RALT_RGUI_RALT:
            unregister_code(KC_RALT);
            unregister_code(KC_RGUI);
            break;
    }
}

// Tap dance for the “Right Ctrl” key:
// - hold: MO(_FN)
// - tap: KC_APP
// - tap and hold: KC_RCTL
// - double tap: KC_RCTL
// - double tap and hold: KC_APP
// - triple tap: KC_APP
enum td_rctl_state {
    TD_RCTL_NOOP,
    TD_RCTL_MO_FN,
    TD_RCTL_APP,
    TD_RCTL_RCTL,
};

static enum td_rctl_state td_rctl_state;

static void td_rctl_finished(qk_tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            if (state->pressed) {
                td_rctl_state = TD_RCTL_MO_FN;
            } else {
                td_rctl_state = TD_RCTL_APP;
            };
            break;
        case 2:
            td_rctl_state = TD_RCTL_RCTL;
            break;
        case 3:
            td_rctl_state = TD_RCTL_APP;
            break;
        default:
            td_rctl_state = TD_RCTL_NOOP;
            break;
    }

    switch (td_rctl_state) {
        case TD_RCTL_NOOP:
            break;
        case TD_RCTL_MO_FN:
            layer_on(_FN);
            break;
        case TD_RCTL_APP:
            register_code(KC_APP);
            break;
        case TD_RCTL_RCTL:
            register_code(KC_RCTL);
            break;
    }
}

static void td_rctl_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (td_rctl_state) {
        case TD_RCTL_NOOP:
            break;
        case TD_RCTL_MO_FN:
            layer_off(_FN);
            break;
        case TD_RCTL_APP:
            unregister_code(KC_APP);
            break;
        case TD_RCTL_RCTL:
            unregister_code(KC_RCTL);
            break;
    }
}

qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_RALT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_ralt_finished, td_ralt_reset),
    [TD_RCTL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_rctl_finished, td_rctl_reset),
};

/* vim:set sw=4 sta et: */
