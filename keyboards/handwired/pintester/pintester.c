/*
 * Copyright 2021 Sergey Vlasov <sigprof@gmail.com>
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

#include "pintester.h"
#include <stdio.h>

enum custom_keycode {
    TEST_KEY = SAFE_RANGE,
};

// This “keyboard” does not have any real keymaps; pretend that the whole
// keymap is filled with TEST_KEY.
uint16_t keymap_key_to_keycode(uint8_t layer, keypos_t key) { return TEST_KEY; }

static keypos_t keypos_log_to_phys(keypos_t log_pos) {
    uint16_t bit = log_pos.col + log_pos.row * MATRIX_COLS;
    uint16_t row = bit / PINTESTER_PIN_COUNT;
    if (row < PINTESTER_PIN_COUNT) {
        return (keypos_t){.col = bit % PINTESTER_PIN_COUNT, .row = row};
    } else {
        return (keypos_t){.col = 255, .row = 255};
    }
}

static keypos_t last_phys       = {.col = 255, .row = 255};
static bool     last_pressed    = false;
static bool     reverse_pressed = false;
static uint16_t press_timer;

static keypos_t delayed_release = {.col = 255, .row = 255};
static uint16_t delayed_release_timer;

static void send_pin_name(uint8_t phys_index) {
    send_string(pin_info[phys_index].name);
    if (pin_info[phys_index].label) {
        SEND_STRING("/");
        send_string(pin_info[phys_index].label);
    }
}

static void send_release(keypos_t phys_pos) {
    SEND_STRING(" -");
    if (phys_pos.col == phys_pos.row) {
        send_pin_name(phys_pos.col);
        SEND_STRING("]");
    } else {
        send_pin_name(phys_pos.row);
        SEND_STRING(",");
        send_pin_name(phys_pos.col);
        SEND_STRING(")");
    }
}

#define REVERSE_REPORT_DELAY DEBOUNCE

void matrix_scan_user(void) {
    if ((delayed_release.col != 255) && timer_elapsed(delayed_release_timer) > REVERSE_REPORT_DELAY) {
        send_release(delayed_release);
        delayed_release.col = delayed_release.row = 255;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode == TEST_KEY) {
        keypos_t phys_pos          = keypos_log_to_phys(record->event.key);
        bool     last_phys_updated = false;
        if (phys_pos.col == 255) {
            return false;
        }
        if (record->event.pressed) {
            // If a release key report was pending, send it now.
            if (delayed_release.col != 255) {
                send_release(delayed_release);
                delayed_release.col = delayed_release.row = 255;
            }

            // Reset the “reverse direction key was pressed” flag by default.
            reverse_pressed = false;

            // Send a space if reporting several press events in sequence.
            if (last_pressed) {
                SEND_STRING(" ");
            }

            if (phys_pos.col == phys_pos.row) {
                // For a direct pin key send a report directly.
                SEND_STRING("[");
                send_pin_name(phys_pos.col);
            } else if (last_pressed && (last_phys.row == phys_pos.col) && (last_phys.col == phys_pos.row) && (timer_elapsed(press_timer) < REVERSE_REPORT_DELAY)) {
                // This is the reverse direction key from the one that was just
                // pressed; send a shortened report for it and set the flag
                // which would be used to handle release events.
                reverse_pressed = true;
                SEND_STRING("(<>");
            } else {
                SEND_STRING("(");
                send_pin_name(phys_pos.row);
                SEND_STRING(",");
                send_pin_name(phys_pos.col);
            }
            press_timer = timer_read();
        } else {
            if (last_pressed && (last_phys.col == phys_pos.col) && (last_phys.row == phys_pos.row)) {
                if (delayed_release.col != 255) {
                    delayed_release.col = delayed_release.row = 255;
                    SEND_STRING(")");
                }
                if (phys_pos.col == phys_pos.row) {
                    SEND_STRING("]");
                } else if (reverse_pressed) {
                    reverse_pressed   = false;
                    last_phys.row     = phys_pos.col;
                    last_phys.col     = phys_pos.row;
                    last_phys_updated = true;
                    SEND_STRING(")");
                } else {
                    SEND_STRING(")");
                }
            } else if (last_pressed && reverse_pressed && (last_phys.row == phys_pos.col) && (last_phys.col == phys_pos.row)) {
                reverse_pressed       = false;
                last_phys_updated     = true;
                delayed_release       = phys_pos;
                delayed_release_timer = timer_read();
            } else {
                SEND_STRING(" -");
                if (phys_pos.col == phys_pos.row) {
                    send_pin_name(phys_pos.col);
                    SEND_STRING("]");
                } else {
                    send_pin_name(phys_pos.row);
                    SEND_STRING(",");
                    send_pin_name(phys_pos.col);
                    SEND_STRING(")");
                }
            }
        }

        if (!last_phys_updated) {
            last_phys    = phys_pos;
            last_pressed = record->event.pressed;
        }
    }
    return true;
}
