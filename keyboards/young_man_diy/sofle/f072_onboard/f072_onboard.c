// Copyright 2025 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

void board_init(void) {
    // F072 default config assumes that I2C1 DMA is remapped
    SYSCFG->CFGR1 |= SYSCFG_CFGR1_I2C1_DMA_RMP;
}

/////////////////////////////////////////////////////////////////////////////
// Custom bootmagic implementation to handle handedness issues
//
// This keyboard does not have hardware handedness detection, and matrix pins
// for the left and right sides are not the same, therefore with the default
// bootmagic code it would be impossible to enter the bootloader if the
// handedness is wrong.  This custom bootmagic code checks pins for both sides
// for the bootmagic trigger key, and also supports setting the EEPROM
// handedness by holding a “handmagic” trigger key.
/////////////////////////////////////////////////////////////////////////////

#ifndef BOOTMAGIC_DEBOUNCE
#    if defined(DEBOUNCE) && DEBOUNCE > 0
#        define BOOTMAGIC_DEBOUNCE (DEBOUNCE * 2)
#    else
#        define BOOTMAGIC_DEBOUNCE 30
#    endif
#endif

// This prototype should really be provided by bootmagic.h
void bootmagic_reset_eeprom(void);

static bool bootmagic_read_key(pin_t output_pin, pin_t input_pin) {
    gpio_set_pin_output(output_pin);
    gpio_write_pin_low(output_pin);
    matrix_output_select_delay();
    bool state = !gpio_read_pin(input_pin);
    gpio_set_pin_input_high(output_pin);
    // Pass true to always use the full delay (the output pin might be used as an input pin on the other side).
    matrix_output_unselect_delay(output_pin, true);
    return state;
}

void bootmagic_scan(void) {
    // Configure all relevant pins (matrix_init() configured pins only for one hand).
    gpio_set_pin_input_high(BOOTMAGIC_OUTPUT_PIN_LEFT);
    gpio_set_pin_input_high(BOOTMAGIC_INPUT_PIN_LEFT);
    gpio_set_pin_input_high(BOOTMAGIC_OUTPUT_PIN_RIGHT);
    gpio_set_pin_input_high(BOOTMAGIC_INPUT_PIN_RIGHT);
    gpio_set_pin_input_high(HANDMAGIC_OUTPUT_PIN_LEFT);
    gpio_set_pin_input_high(HANDMAGIC_INPUT_PIN_LEFT);
    gpio_set_pin_input_high(HANDMAGIC_OUTPUT_PIN_RIGHT);
    gpio_set_pin_input_high(HANDMAGIC_INPUT_PIN_RIGHT);
    //matrix_output_unselect_delay(BOOTMAGIC_OUTPUT_PIN_LEFT, true);
    matrix_io_delay();

    // Read the state of all magic keys.
    bool bootmagic_left_pressed  = bootmagic_read_key(BOOTMAGIC_OUTPUT_PIN_LEFT, BOOTMAGIC_INPUT_PIN_LEFT);
    bool bootmagic_right_pressed = bootmagic_read_key(BOOTMAGIC_OUTPUT_PIN_RIGHT, BOOTMAGIC_INPUT_PIN_RIGHT);
    bool handmagic_left_pressed  = bootmagic_read_key(HANDMAGIC_OUTPUT_PIN_LEFT, HANDMAGIC_INPUT_PIN_LEFT);
    bool handmagic_right_pressed = bootmagic_read_key(HANDMAGIC_OUTPUT_PIN_RIGHT, HANDMAGIC_INPUT_PIN_RIGHT);

    // Handle the bootmagic keys.
    if (bootmagic_left_pressed || bootmagic_right_pressed) {
        bootmagic_reset_eeprom();
        if ((BOOTMAGIC_OUTPUT_PIN_LEFT != BOOTMAGIC_OUTPUT_PIN_RIGHT) || (BOOTMAGIC_INPUT_PIN_LEFT != BOOTMAGIC_INPUT_PIN_RIGHT)) {
            // If the bootmagic pins for hands are different, handedness can be determined here too.
            eeconfig_update_handedness(bootmagic_left_pressed);
        }
        bootloader_jump();
    }

    // Handle the handedness configuration keys.
    if (handmagic_left_pressed || handmagic_right_pressed) {
        bool old_handedness = eeconfig_read_handedness();
        bool new_handedness = handmagic_left_pressed;
        if (old_handedness != new_handedness) {
            eeconfig_update_handedness(new_handedness);
            // matrix_init() had been already called at this point, so the only way to update handedness is to restart the firmware.
            mcu_reset();
        }

        // Wait until the handedness configuration key had been released before continuing the startup.
        do {
            wait_ms(1);
            handmagic_left_pressed  = bootmagic_read_key(HANDMAGIC_OUTPUT_PIN_LEFT, HANDMAGIC_INPUT_PIN_LEFT);
            handmagic_right_pressed = bootmagic_read_key(HANDMAGIC_OUTPUT_PIN_RIGHT, HANDMAGIC_INPUT_PIN_RIGHT);
        } while (handmagic_left_pressed || handmagic_left_pressed);
        wait_ms(BOOTMAGIC_DEBOUNCE);
    }
}
