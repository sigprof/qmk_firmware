// Copyright 2023 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "pintester.h"

#ifdef PINTESTER_ACTIVE_HIGH
#    define set_pin_active(pin) \
        do {                    \
            setPinOutput(pin);  \
            writePinHigh(pin);  \
        } while (0)
#    define set_pin_inactive(pin) setPinInputLow(pin)
#    define is_pin_active(pin) (!!readPin(pin))
#else
#    define set_pin_active(pin) \
        do {                    \
            setPinOutput(pin);  \
            writePinLow(pin);   \
        } while (0)
#    define set_pin_inactive(pin) setPinInputHigh(pin)
#    define is_pin_active(pin) (!readPin(pin))
#endif

// Handle the PINTESTER_INPUT_PINS and PINTESTER_OUTPUT_PINS defines:
//  - either both of them must be defined, or none of them;
//  - if they are defined, PINTESTER_IGNORE_PINS must not be defined, and
//    PINTESTER_DEFAULT_IGNORE_PINS is ignored (this is handled by defining
//    PINTESTER_IGNORE_PINS to an empty value);
//  - if they are not defined, define them to nothing.
#if defined(PINTESTER_INPUT_PINS) && defined(PINTESTER_OUTPUT_PINS)
#    if defined(PINTESTER_IGNORE_PINS)
#        error "PINTESTER_IGNORE_PINS cannot be used together with PINTESTER_INPUT_PINS and PINTESTER_OUTPUT_PINS"
#    else
#        define PINTESTER_IGNORE_PINS
#    endif
#elif defined(PINTESTER_INPUT_PINS) || defined(PINTESTER_OUTPUT_PINS)
#    error "Defining only one of PINTESTER_INPUT_PINS and PINTESTER_OUTPUT_PINS is not supported"
#else
#    define PINTESTER_INPUT_PINS
#    define PINTESTER_OUTPUT_PINS
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

// Matrix startup delay (ms).  Should be longer than the time for the host to
// enumerate the USB device and initialize the HID keyboard driver, so that
// keys that appear always pressed may be properly reported (usually such keys
// are lost when they are detected as pressed before the host completes the USB
// and HID driver initialization).
#ifndef PINTESTER_STARTUP_DELAY
#    define PINTESTER_STARTUP_DELAY 2000
#endif

// Timer and flag to implement PINTESTER_STARTUP_DELAY.
static uint16_t startup_delay_timer;
static bool     startup_delay_timer_active;

// List of pins which should not be touched even though they are included in
// PINTESTER_PINS.
static const pin_t ignore_pins[] = {PINTESTER_IGNORE_PINS};

// Explicit lists of input and output pins.
static const pin_t input_pins[]  = {PINTESTER_INPUT_PINS};
static const pin_t output_pins[] = {PINTESTER_OUTPUT_PINS};

static void set_all_pins_inactive(void) {
    for (uint8_t i = 0; i < PINTESTER_PIN_COUNT; i++) {
        if (pin_state[i].enable_output || pin_state[i].enable_input) {
            set_pin_inactive(pin_info[i].pin);
        }
    }
}

static void init_pin_state(void) {
    _Static_assert(sizeof(pin_info) / sizeof(pin_info[0]) == PINTESTER_PIN_COUNT, "PINTESTER_PIN_COUNT must match PINTESTER_PINS");

    static bool init_done = false;
    if (init_done) {
        return;
    }

    // Convert pin lists into per-pin enable_input and enable_output flags.
    for (uint8_t i = 0; i < PINTESTER_PIN_COUNT; i++) {
        pin_t pin = pin_info[i].pin;
        if (ARRAY_SIZE(input_pins) > 0 && ARRAY_SIZE(output_pins) > 0) {
            // Using explicit lists of input and output pins.
            pin_state[i].enable_input  = false;
            pin_state[i].enable_output = false;
            for (uint8_t j = 0; j < ARRAY_SIZE(input_pins); j++) {
                if (input_pins[j] == pin) {
                    pin_state[i].enable_input = true;
                    break;
                }
            }
            for (uint8_t j = 0; j < ARRAY_SIZE(output_pins); j++) {
                if (output_pins[j] == pin) {
                    pin_state[i].enable_output = true;
                    break;
                }
            }
        } else {
            // Using all pins except ignored ones for both input and output.
            pin_state[i].enable_input  = true;
            pin_state[i].enable_output = true;
            for (uint8_t j = 0; j < ARRAY_SIZE(ignore_pins); j++) {
                if (ignore_pins[j] == pin) {
                    pin_state[i].enable_input  = false;
                    pin_state[i].enable_output = false;
                    break;
                }
            }
        }
    }

    init_done = true;
}

bool pintester_is_pin_enabled(pin_t pin) {
    init_pin_state();
    for (uint8_t i = 0; i < PINTESTER_PIN_COUNT; i++) {
        if (pin_info[i].pin == pin) {
            return pin_state[i].enable_input || pin_state[i].enable_output;
        }
    }
    return false;
}

void matrix_init_custom(void) {
    init_pin_state();
    set_all_pins_inactive();

    startup_delay_timer        = timer_read();
    startup_delay_timer_active = true;
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    // Skip matrix scanning during the startup delay.
    if (startup_delay_timer_active) {
        if (timer_elapsed(startup_delay_timer) <= PINTESTER_STARTUP_DELAY) {
            return false;
        }
        startup_delay_timer_active = false;
    }

    bool changed = false;

    // Read the pin states when no pins are activated as outputs; store those
    // states in the .active field of the pin_state[] array elements.
    set_all_pins_inactive();
    matrix_output_unselect_delay(0, true);
    for (uint8_t phys_col = 0; phys_col < PINTESTER_PIN_COUNT; phys_col++) {
        if (pin_state[phys_col].enable_input) {
            pin_state[phys_col].active = is_pin_active(pin_info[phys_col].pin);
        }
    }

    // These variables point to the current row and bit in the logical matrix.
    matrix_row_t *log_p         = current_matrix;
    matrix_row_t  log_row_value = 0;
    uint8_t       log_bit       = 0;

    // Now scan the whole matrix.
    for (uint8_t phys_row = 0; phys_row < PINTESTER_PIN_COUNT; phys_row++) {
        // If the pin is enabled for output, activate it, otherwise just
        // continue with the rest of scanning loop (do not short circuit it,
        // because the same number of bits must be written to the logical
        // matrix).
        if (pin_state[phys_row].enable_output) {
            set_pin_active(pin_info[phys_row].pin);
            matrix_output_select_delay();
        }

        // Read the state of all other pins after activating one pin.
        for (uint8_t phys_col = 0; phys_col < PINTESTER_PIN_COUNT; phys_col++) {
            bool active;
            if (!pin_state[phys_row].enable_output || !pin_state[phys_col].enable_input) {
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
        if (pin_state[phys_row].enable_output) {
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
