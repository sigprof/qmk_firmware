/* Copyright 2022 Sergey Vlasov (@sigprof)
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

enum custom_keycodes {
    RT_TOGG = SAFE_RANGE,
    RT_RSET,
    RT_PREV,
    RT_NEXT,
    RT_SEND,
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_all(
            TG(1),        KC_MUTE,     KC_MPLY,
        KC_PAUS, KC_PAUS, KC_PAUS, KC_PAUS, KC_PAUS,
        KC_PAUS, KC_PAUS, KC_PAUS, KC_PAUS, KC_PAUS,
        KC_PAUS, KC_PAUS, KC_PAUS, KC_PAUS, KC_PAUS
    ),
    [1] = LAYOUT_all(
            KC_TRNS,      KC_MUTE,     KC_MPLY,
        RT_TOGG, RT_RSET, RT_PREV, RT_NEXT, RT_SEND,
        RGB_M_P, RGB_HUI, RGB_SAI, RGB_VAI, RGB_SPI,
        RGB_MOD, RGB_HUD, RGB_SAD, RGB_VAD, RGB_SPD
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
static const char colors[3] = {'R', 'G', 'B'};

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
                sprintf(send_buf, "+%d,%c", rgb_led_index, colors[rgb_color_index]);
                send_string(send_buf);
                return false;
        }
    }
    return true;
}
