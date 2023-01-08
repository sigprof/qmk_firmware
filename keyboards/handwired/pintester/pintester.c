// Copyright 2023 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "pintester.h"
#include <stdio.h>

_Static_assert((PINTESTER_PIN_COUNT * PINTESTER_PIN_COUNT) <= (MATRIX_ROWS * MATRIX_COLS), "The matrix size is too small to fit PINTESTER_PIN_COUNT * PINTESTER_PIN_COUNT bits");

enum custom_keycode {
    TEST_KEY = SAFE_RANGE,
};

// This “keyboard” does not have any real keymaps; pretend that the whole
// keymap is filled with TEST_KEY.
uint16_t keymap_key_to_keycode(uint8_t layer, keypos_t key) {
    return TEST_KEY;
}

// Convert the matrix position from the logical representation (where column
// numbers are limited to 0...31 to fit the row data into uint32_t) to the
// physical representation (where row and column numbers correspond to the
// indices in pin_info[] and pin_state[]).
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
        // The release event for the reverse direction key did not arrive
        // before the timeout; send the original release report independently.
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
            // Handle key press events.

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
                // This is the reverse direction key for the one that was just
                // pressed; send a shortened report for it and set the flag
                // which would be used to handle release events.
                reverse_pressed = true;
                SEND_STRING("(<>");
            } else {
                // Send the normal report for the pressed key.
                SEND_STRING("(");
                send_pin_name(phys_pos.row);
                SEND_STRING(",");
                send_pin_name(phys_pos.col);
            }
            press_timer = timer_read();
        } else {
            // Handle key release events.
            if (last_pressed && (last_phys.col == phys_pos.col) && (last_phys.row == phys_pos.row)) {
                // The same key as in last_phys was released - send the short
                // form of release report for it.
                if (delayed_release.col != 255) {
                    // If a delayed release was pending in this state, it must
                    // be for the reverse direction key which happened to be
                    // registered as released first.  Send the short form for
                    // that key too, and reset the delayed release state.
                    delayed_release.col = delayed_release.row = 255;
                    SEND_STRING(")");
                }
                if (phys_pos.col == phys_pos.row) {
                    // Send the short form for a direct pin key release.
                    SEND_STRING("]");
                } else if (reverse_pressed) {
                    // Normal press -> reverse press -> reverse release; send
                    // the short form for the reverse key and restore the state
                    // as if it was after the normal press.  If the next event
                    // is the release of the normal key, it will be handled by
                    // sending the short form of release report.
                    reverse_pressed   = false;
                    last_phys.row     = phys_pos.col;
                    last_phys.col     = phys_pos.row;
                    last_phys_updated = true;
                    SEND_STRING(")");
                } else {
                    // Send the short form for a non-direct pin key release.
                    SEND_STRING(")");
                }
            } else if (last_pressed && reverse_pressed && (last_phys.row == phys_pos.col) && (last_phys.col == phys_pos.row)) {
                // Both the normal and the reverse key were pressed, and now
                // the normal key is registered as released.  In theory it is
                // possible that the normal and reverse key are independent, so
                // instead of reporting the release event in the short form
                // immediately (which would be incorrect if the keys are
                // independent) the actual released key is remembered in
                // delayed_release; it would be reported independently if the
                // matching release event for the reverse key does not arrive
                // before REVERSE_REPORT_DELAY.
                reverse_pressed       = false;
                last_phys_updated     = true;
                delayed_release       = phys_pos;
                delayed_release_timer = timer_read();
            } else {
                // The release event does not match the latest press event, and
                // needs to be reported in the full format.
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

        // Remember the last processed event unless it's one of the special
        // cases already handled above.
        if (!last_phys_updated) {
            last_phys    = phys_pos;
            last_pressed = record->event.pressed;
        }
    }
    return true;
}
