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
    _FN_ALT,
    _ADJUST,
};

#define U_FCAPS LT(_FN, KC_CAPS)
#define U_FAPP  LT(_FN, KC_APP)
#define U_MONUM MO(_NUMPAD)
#define U_MOADJ MO(_ADJUST)
#define U_TGNUM TG(_NUMPAD)
#define U_OSRGU OSM(MOD_RGUI)
#define U_OSRCT OSM(MOD_RCTL)
#define U_CPGUP RCTL(KC_PGUP)
#define U_CPGDN RCTL(KC_PGDN)
#define U_FNALT LM(_FN_ALT, MOD_LALT)

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
     * which are used as AltGr and Fn (having the Fn key on the right side
     * seems to be more useful than having Right Ctrl there).  The Fn key is
     * also overloaded to work as App (Menu/Compose) key when tapped.
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
     * │Ctrl│GUI │Alt │                        │AltGr│ApFn │┌───┼───┼───┐
     * └────┴────┴────┴────────────────────────┴─────┴─────┘│ ← │ ↓ │ → │
     *                                                      └───┴───┴───┘
     */

    [_QWERTY] = LAYOUT(
        KC_ESC,    KC_F1,   KC_F2,   KC_F3,   KC_F4,     KC_F5,   KC_F6,   KC_F7,   KC_F8,     KC_F9,   KC_F10,  KC_F11,  KC_F12,     KC_INS,    KC_DEL,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,            KC_PGUP,
        KC_TAB,      KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,        KC_PGDN,
        U_FCAPS,       KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,
        KC_LSFT,            KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,          KC_UP,
        KC_LCTL,   KC_LGUI,   KC_LALT,                       KC_SPC,                              KC_RALT,     U_FAPP,         KC_LEFT, KC_DOWN, KC_RGHT
    ),

    /*
     * Numeric keypad emulation layer.
     *
     * The layout is like on old ThinkPad keyboards (which apparently conform
     * to ISO/IEC 9995-2:2009 Amendment 1:2012), but with some hopefully useful
     * additions:
     *
     * - The whole digit row is mapped to numpad keys, except the “0” key
     *   (which is specified to be mapped to the numpad slash); however, an
     *   extra “0” key is provided to the left of the “1” key.
     *
     * - The usual “-” and “+” keys are mapped to their numpad equivalents
     *   (although that “+” key usually sends the equal sign, for the numpad
     *   emulation purpose a more commonly supported “+” mapping is chosen).
     *
     * - The numpad equal sign key is on the apostrophe/double quote key.
     *
     * - The backslash key is mapped to Num Lock.
     *
     * Unused alphanumeric keys are turned off.
     */
    [_NUMPAD] = LAYOUT(
        _______,   _______, _______, _______, _______,   _______, _______, _______, _______,   _______, _______, _______, _______,    _______,   _______,
        KC_P0,   KC_P1,   KC_P2,   KC_P3,   KC_P4,   KC_P5,   KC_P6,   KC_P7,   KC_P8,   KC_P9,   KC_PSLS, KC_PMNS, KC_PPLS, _______,            _______,
        _______,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_P4,   KC_P5,   KC_P6,   KC_PAST, XXXXXXX, XXXXXXX, KC_NLCK,        _______,
        _______,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_P1,   KC_P2,   KC_P3,   KC_PMNS, KC_PEQL, KC_PENT,
        _______,            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_P0,   XXXXXXX, KC_PDOT, KC_PPLS, _______,          _______,
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
        XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    KC_HOME,   KC_END,
        KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,             U_CPGUP,
        U_MONUM,     KC_BTN1, KC_MS_U, KC_BTN2, KC_WH_U, DM_PLY1, KC_INS,  KC_HOME, KC_UP,   KC_END,  KC_PGUP, KC_VOLU, KC_MUTE, U_TGNUM,        U_CPGDN,
        _______,       KC_MS_L, KC_MS_D, KC_MS_R, KC_WH_D, DM_PLY2, KC_DEL,  KC_LEFT, KC_DOWN, KC_RGHT, KC_PGDN, KC_VOLD, KC_PENT,
        _______,            KC_BTN3, KC_BTN4, KC_BTN5, KC_WH_L, KC_WH_R, KC_PSCR, KC_SLCK, KC_PAUS, U_OSRGU, U_OSRCT, _______,          KC_PGUP,
        _______,   _______,   U_FNALT,                      U_MOADJ,                              _______,     _______,        KC_HOME, KC_PGDN, KC_END
    ),

    /*
     * Fn+Alt layer - dynamic macro management:
     * - Fn+Alt+Esc - stop recording;
     * - Fn+Alt+T   - start recording macro 1;
     * - Fn+Alt+G   - start recording macro 2.
     */
    [_FN_ALT] = LAYOUT(
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

/* vim:set sw=4 sta et: */
