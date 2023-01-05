// Copyright 2023 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "pintester.h"

#ifdef PINTESTER_ACTIVE_HIGH
#    define set_pin_active(pin) do { setPinOutput(pin); writePinHigh(pin); } while(0)
#    define set_pin_inactive(pin) setPinInputLow(pin)
#    define is_pin_active(pin) (!!readPin(pin))
#else
#    define set_pin_active(pin) do { setPinOutput(pin); writePinLow(pin); } while(0)
#    define set_pin_inactive(pin) setPinInputHigh(pin)
#    define is_pin_active(pin) (!readPin(pin))
#endif

// Set the default value for PINTESTER_IGNORE_PINS if it was not defined.
#ifndef PINTESTER_IGNORE_PINS
#    ifdef PINTESTER_DEFAULT_IGNORE_PINS
#        define PINTESTER_IGNORE_PINS PINTESTER_DEFAULT_IGNORE_PINS
#    else
#        define PINTESTER_IGNORE_PINS
#    endif
#endif

// Constant pin information specified in config.h for a particular controller.
const struct pintester_pin_info pin_info[] = {PINTESTER_PINS};

// Runtime pin state for all controller pins.
struct pintester_pin_state pin_state[PINTESTER_PIN_COUNT];

// List of pins which should not be touched even though they are included in
// PINTESTER_PINS.
static const pin_t ignore_pins[] = {PINTESTER_IGNORE_PINS};

static void set_all_pins_inactive(void) {
    for (uint8_t i = 0; i < PINTESTER_PIN_COUNT; i++) {
        if (pin_state[i].enabled) {
            set_pin_inactive(pin_info[i].pin);
        }
    }
}

void matrix_init_custom(void) {
    _Static_assert(sizeof(pin_info) / sizeof(pin_info[0]) == PINTESTER_PIN_COUNT, "PINTESTER_PIN_COUNT must match PINTESTER_PINS");

    // Convert PINTESTER_IGNORE_PINS into per-pin .enabled flags.
    for (uint8_t i = 0; i < PINTESTER_PIN_COUNT; i++) {
        bool enabled = true;
        for (uint8_t j = 0; j < sizeof(ignore_pins) / sizeof(ignore_pins[0]); j++) {
            if (pin_info[i].pin == ignore_pins[j]) {
                enabled = false;
                break;
            }
        }
        pin_state[i].enabled = enabled;
    }

    set_all_pins_inactive();
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    bool changed = false;

    // Read the pin states when no pins are activated as outputs; store those
    // states in the .active field of the pin_state[] array elements.
    set_all_pins_inactive();
    matrix_output_unselect_delay(0, true);
    for (uint8_t phys_col = 0; phys_col < PINTESTER_PIN_COUNT; phys_col++) {
        if (pin_state[phys_col].enabled) {
            pin_state[phys_col].active = is_pin_active(pin_info[phys_col].pin);
        }
    }

    // These variables point to the current row and bit in the logical matrix.
    matrix_row_t *log_p         = current_matrix;
    matrix_row_t  log_row_value = 0;
    uint8_t       log_bit       = 0;

    // Now scan the whole matrix.
    for (uint8_t phys_row = 0; phys_row < PINTESTER_PIN_COUNT; phys_row++) {
        // If the pin is enabled, activate it, otherwise just continue with the
        // rest of scanning loop (do not short circuit it, because the same
        // number of bits must be written to the logical matrix).
        if (pin_state[phys_row].enabled) {
            set_pin_active(pin_info[phys_row].pin);
            matrix_output_select_delay();
        }

        // Read the state of all other pins after activating one pin.
        for (uint8_t phys_col = 0; phys_col < PINTESTER_PIN_COUNT; phys_col++) {
            bool active;
            if (!pin_state[phys_row].enabled || !pin_state[phys_col].enabled) {
                // Ignored pins are always considered inactive.
                active = false;
            } else if (phys_col == phys_row) {
                // Elements on the main diagonal represent “direct pins” (pins
                // that are read as active when no pins are activated as
                // outputs).
                active = pin_state[phys_col].active;
            } else if (pin_state[phys_col].active) {
                // If the pin was read as active when no pins were activated as
                // outputs, most likely it would also read as active when some
                // other pin is activated as output.  Avoid reporting a whole
                // column as active in that case.
                active = false;
            } else {
                // Read the pin state.
                active = is_pin_active(pin_info[phys_col].pin);
            }
            // Write the pin state to the currently accumulated logical row.
            if (active) {
                log_row_value |= MATRIX_ROW_SHIFTER << log_bit;
            }
            // Prepare for the next bit; write the logical row to the matrix if
            // all bits were filled.
            if (++log_bit >= MATRIX_COLS) {
                if (*log_p != log_row_value) {
                    changed = true;
                }
                *log_p++      = log_row_value;
                log_row_value = 0;
                log_bit       = 0;
            }
        }

        // If the pin was activated as output before, deactivate it again.
        if (pin_state[phys_row].enabled) {
            set_pin_inactive(pin_info[phys_row].pin);
            matrix_output_unselect_delay(0, true);
        }
    }

    // Write the last logical row to the matrix if it was filled partially.
    if (log_bit > 0) {
        if (*log_p != log_row_value) {
            changed = true;
        }
        *log_p = log_row_value;
    }

    return changed;
}
