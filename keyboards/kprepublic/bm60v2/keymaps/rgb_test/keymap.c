/* Copyright 2020 MechMerlin
 * Copyright 2021 Sergey Vlasov
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

#include <is31fl3733.h>

enum custom_keycodes {
    RT_TOGG = SAFE_RANGE,
    RT_RSET,
    RT_PREV,
    RT_NEXT,
    RT_SEND,
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_60_ansi_arrow(
        KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,
        KC_TAB,      KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
        KC_CAPS,       KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,
        KC_LSFT,            KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_RSFT,        KC_UP,   KC_SLSH,
        KC_LCTL,   KC_LGUI,   KC_LALT,                       KC_SPC,                              KC_RALT, MO(1),   KC_LEFT, KC_DOWN, KC_RGHT
    ),
    [1] = LAYOUT_60_ansi_arrow(
        RESET,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______,
        _______,     RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, _______, _______, _______, _______, _______,
        RT_TOGG,       RT_RSET, _______, _______, RT_PREV, RT_NEXT, RT_SEND, _______, _______, _______, _______, _______, _______,
        _______,            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,        _______, _______,
        _______,   _______,   _______,                      _______,                              _______, _______, _______, _______, _______
    ),

};
// clang-format on

static bool    rgb_test_mode   = false;
static uint8_t rgb_led_index   = 0;
static uint8_t rgb_color_index = 0;

void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    if (rgb_test_mode) {
        for (uint8_t i = led_min; i <= led_max; ++i) {
            uint8_t r = 0, g = 0, b = 0;
            if (i == rgb_led_index) {
                r = (rgb_color_index == 0) ? 0xFF : 0;
                g = (rgb_color_index == 1) ? 0xFF : 0;
                b = (rgb_color_index == 2) ? 0xFF : 0;
            }
            rgb_matrix_set_color(i, r, g, b);
        }
    }
}

static char send_buf[32];

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    if (record->event.pressed) {
        switch (keycode) {
            case RT_TOGG:
                rgb_test_mode = !rgb_test_mode;
                return false;

            case RT_RSET:
                rgb_led_index   = 0;
                rgb_color_index = 0;
                return false;

            case RT_PREV:
                if (rgb_color_index > 0) {
                    rgb_color_index--;
                } else {
                    rgb_color_index = 2;
                    if (rgb_led_index > 0) {
                        rgb_led_index--;
                    } else {
                        rgb_led_index = DRIVER_LED_TOTAL - 1;
                    }
                }
                return false;

            case RT_NEXT:
                if (++rgb_color_index > 2) {
                    rgb_color_index = 0;
                    if (++rgb_led_index >= DRIVER_LED_TOTAL) {
                        rgb_led_index = 0;
                    }
                }
                return false;

            case RT_SEND:
                if (rgb_led_index < ISSI_LED_TOTAL) {
                    // IS31FL3733-specific code here
                    is31_led led = g_is31_leds[rgb_led_index];
                    uint8_t  reg = (rgb_color_index == 0) ? led.r : ((rgb_color_index == 1) ? led.g : led.b);
                    sprintf(send_buf, "%d,%c_%d", led.driver, 'A' + (reg >> 4), (reg & 0x0F) + 1);
                } else {
                    static const char colors[3] = {'R', 'G', 'B'};
                    sprintf(send_buf, "+%d,%c", rgb_led_index - ISSI_LED_TOTAL, colors[rgb_color_index]);
                }
                send_string(send_buf);
                return false;
        }
    }
    return true;
}
