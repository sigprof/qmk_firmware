/* Copyright 2021 sigprof 2021 peepeetee
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

#ifdef RGB_MATRIX_ENABLE
#    include <i2c_master.h>
#    include <is31fl3741.h>
#    include <ws2812.h>

// clang-format off

// Dummy IS31FL3741 config for testing - enables all possible LED locations
const is31_led __flash g_is31_leds[DRIVER_LED_TOTAL] = {
    { 0, CS3_SW1, CS2_SW1, CS1_SW1 },
    { 0, CS3_SW2, CS2_SW2, CS1_SW2 },
    { 0, CS3_SW3, CS2_SW3, CS1_SW3 },
    { 0, CS3_SW4, CS2_SW4, CS1_SW4 },
    { 0, CS3_SW5, CS2_SW5, CS1_SW5 },
    { 0, CS3_SW6, CS2_SW6, CS1_SW6 },
    { 0, CS3_SW7, CS2_SW7, CS1_SW7 },
    { 0, CS3_SW8, CS2_SW8, CS1_SW8 },
    { 0, CS3_SW9, CS2_SW9, CS1_SW9 },

    { 0, CS6_SW1, CS5_SW1, CS4_SW1 },
    { 0, CS6_SW2, CS5_SW2, CS4_SW2 },
    { 0, CS6_SW3, CS5_SW3, CS4_SW3 },
    { 0, CS6_SW4, CS5_SW4, CS4_SW4 },
    { 0, CS6_SW5, CS5_SW5, CS4_SW5 },
    { 0, CS6_SW6, CS5_SW6, CS4_SW6 },
    { 0, CS6_SW7, CS5_SW7, CS4_SW7 },
    { 0, CS6_SW8, CS5_SW8, CS4_SW8 },
    { 0, CS6_SW9, CS5_SW9, CS4_SW9 },

    { 0, CS9_SW1, CS8_SW1, CS7_SW1 },
    { 0, CS9_SW2, CS8_SW2, CS7_SW2 },
    { 0, CS9_SW3, CS8_SW3, CS7_SW3 },
    { 0, CS9_SW4, CS8_SW4, CS7_SW4 },
    { 0, CS9_SW5, CS8_SW5, CS7_SW5 },
    { 0, CS9_SW6, CS8_SW6, CS7_SW6 },
    { 0, CS9_SW7, CS8_SW7, CS7_SW7 },
    { 0, CS9_SW8, CS8_SW8, CS7_SW8 },
    { 0, CS9_SW9, CS8_SW9, CS7_SW9 },

    { 0, CS12_SW1, CS11_SW1, CS10_SW1 },
    { 0, CS12_SW2, CS11_SW2, CS10_SW2 },
    { 0, CS12_SW3, CS11_SW3, CS10_SW3 },
    { 0, CS12_SW4, CS11_SW4, CS10_SW4 },
    { 0, CS12_SW5, CS11_SW5, CS10_SW5 },
    { 0, CS12_SW6, CS11_SW6, CS10_SW6 },
    { 0, CS12_SW7, CS11_SW7, CS10_SW7 },
    { 0, CS12_SW8, CS11_SW8, CS10_SW8 },
    { 0, CS12_SW9, CS11_SW9, CS10_SW9 },

    { 0, CS15_SW1, CS14_SW1, CS13_SW1 },
    { 0, CS15_SW2, CS14_SW2, CS13_SW2 },
    { 0, CS15_SW3, CS14_SW3, CS13_SW3 },
    { 0, CS15_SW4, CS14_SW4, CS13_SW4 },
    { 0, CS15_SW5, CS14_SW5, CS13_SW5 },
    { 0, CS15_SW6, CS14_SW6, CS13_SW6 },
    { 0, CS15_SW7, CS14_SW7, CS13_SW7 },
    { 0, CS15_SW8, CS14_SW8, CS13_SW8 },
    { 0, CS15_SW9, CS14_SW9, CS13_SW9 },

    { 0, CS18_SW1, CS17_SW1, CS16_SW1 },
    { 0, CS18_SW2, CS17_SW2, CS16_SW2 },
    { 0, CS18_SW3, CS17_SW3, CS16_SW3 },
    { 0, CS18_SW4, CS17_SW4, CS16_SW4 },
    { 0, CS18_SW5, CS17_SW5, CS16_SW5 },
    { 0, CS18_SW6, CS17_SW6, CS16_SW6 },
    { 0, CS18_SW7, CS17_SW7, CS16_SW7 },
    { 0, CS18_SW8, CS17_SW8, CS16_SW8 },
    { 0, CS18_SW9, CS17_SW9, CS16_SW9 },

    { 0, CS21_SW1, CS20_SW1, CS19_SW1 },
    { 0, CS21_SW2, CS20_SW2, CS19_SW2 },
    { 0, CS21_SW3, CS20_SW3, CS19_SW3 },
    { 0, CS21_SW4, CS20_SW4, CS19_SW4 },
    { 0, CS21_SW5, CS20_SW5, CS19_SW5 },
    { 0, CS21_SW6, CS20_SW6, CS19_SW6 },
    { 0, CS21_SW7, CS20_SW7, CS19_SW7 },
    { 0, CS21_SW8, CS20_SW8, CS19_SW8 },
    { 0, CS21_SW9, CS20_SW9, CS19_SW9 },

    { 0, CS24_SW1, CS23_SW1, CS22_SW1 },
    { 0, CS24_SW2, CS23_SW2, CS22_SW2 },
    { 0, CS24_SW3, CS23_SW3, CS22_SW3 },
    { 0, CS24_SW4, CS23_SW4, CS22_SW4 },
    { 0, CS24_SW5, CS23_SW5, CS22_SW5 },
    { 0, CS24_SW6, CS23_SW6, CS22_SW6 },
    { 0, CS24_SW7, CS23_SW7, CS22_SW7 },
    { 0, CS24_SW8, CS23_SW8, CS22_SW8 },
    { 0, CS24_SW9, CS23_SW9, CS22_SW9 },

    { 0, CS27_SW1, CS26_SW1, CS25_SW1 },
    { 0, CS27_SW2, CS26_SW2, CS25_SW2 },
    { 0, CS27_SW3, CS26_SW3, CS25_SW3 },
    { 0, CS27_SW4, CS26_SW4, CS25_SW4 },
    { 0, CS27_SW5, CS26_SW5, CS25_SW5 },
    { 0, CS27_SW6, CS26_SW6, CS25_SW6 },
    { 0, CS27_SW7, CS26_SW7, CS25_SW7 },
    { 0, CS27_SW8, CS26_SW8, CS25_SW8 },
    { 0, CS27_SW9, CS26_SW9, CS25_SW9 },

    { 0, CS30_SW1, CS29_SW1, CS28_SW1 },
    { 0, CS30_SW2, CS29_SW2, CS28_SW2 },
    { 0, CS30_SW3, CS29_SW3, CS28_SW3 },
    { 0, CS30_SW4, CS29_SW4, CS28_SW4 },
    { 0, CS30_SW5, CS29_SW5, CS28_SW5 },
    { 0, CS30_SW6, CS29_SW6, CS28_SW6 },
    { 0, CS30_SW7, CS29_SW7, CS28_SW7 },
    { 0, CS30_SW8, CS29_SW8, CS28_SW8 },
    { 0, CS30_SW9, CS29_SW9, CS28_SW9 },

    { 0, CS33_SW1, CS32_SW1, CS31_SW1 },
    { 0, CS33_SW2, CS32_SW2, CS31_SW2 },
    { 0, CS33_SW3, CS32_SW3, CS31_SW3 },
    { 0, CS33_SW4, CS32_SW4, CS31_SW4 },
    { 0, CS33_SW5, CS32_SW5, CS31_SW5 },
    { 0, CS33_SW6, CS32_SW6, CS31_SW6 },
    { 0, CS33_SW7, CS32_SW7, CS31_SW7 },
    { 0, CS33_SW8, CS32_SW8, CS31_SW8 },
    { 0, CS33_SW9, CS32_SW9, CS31_SW9 },

    { 0, CS36_SW1, CS35_SW1, CS34_SW1 },
    { 0, CS36_SW2, CS35_SW2, CS34_SW2 },
    { 0, CS36_SW3, CS35_SW3, CS34_SW3 },
    { 0, CS36_SW4, CS35_SW4, CS34_SW4 },
    { 0, CS36_SW5, CS35_SW5, CS34_SW5 },
    { 0, CS36_SW6, CS35_SW6, CS34_SW6 },
    { 0, CS36_SW7, CS35_SW7, CS34_SW7 },
    { 0, CS36_SW8, CS35_SW8, CS34_SW8 },
    { 0, CS36_SW9, CS35_SW9, CS34_SW9 },

    { 0, CS39_SW1, CS38_SW1, CS37_SW1 },
    { 0, CS39_SW2, CS38_SW2, CS37_SW2 },
    { 0, CS39_SW3, CS38_SW3, CS37_SW3 },
    { 0, CS39_SW4, CS38_SW4, CS37_SW4 },
    { 0, CS39_SW5, CS38_SW5, CS37_SW5 },
    { 0, CS39_SW6, CS38_SW6, CS37_SW6 },
    { 0, CS39_SW7, CS38_SW7, CS37_SW7 },
    { 0, CS39_SW8, CS38_SW8, CS37_SW8 },
    { 0, CS39_SW9, CS38_SW9, CS37_SW9 },
};

// Dummy RGB Matrix config for testing
led_config_t g_led_config = {
    // Matrix location to LED index
    {
        [0 ... MATRIX_ROWS-1] = {
            [0 ... MATRIX_COLS-1] = NO_LED
        }
    },

    // LED index to physical location
    {
        [0 ... DRIVER_LED_TOTAL-1] = {0, 0}
    },

    // LED index to flags
    {
        [0 ... DRIVER_LED_TOTAL-1] = 4
    }
};
// clang-format on

// ==========================================================================
// Custom RGB Matrix driver that combines IS31FL3741 and WS2812
// ==========================================================================
#    if WS2812_LED_TOTAL > 0
LED_TYPE rgb_matrix_ws2812_array[WS2812_LED_TOTAL];
#    endif
static void rgb_matrix_driver_init(void) {
    i2c_init();
    IS31FL3741_init(DRIVER_ADDR_1);
    for (uint8_t index = 0; index < ISSI_LED_TOTAL; index++) {
        bool enabled = true;
        IS31FL3741_set_led_control_register(index, enabled, enabled, enabled);
    }
    IS31FL3741_update_led_control_registers(DRIVER_ADDR_1, 0);
}
static void rgb_matrix_driver_flush(void) {
    IS31FL3741_update_pwm_buffers(DRIVER_ADDR_1, 0);
#    if WS2812_LED_TOTAL > 0
    ws2812_setleds(rgb_matrix_ws2812_array, WS2812_LED_TOTAL);
#    endif
}
static void rgb_matrix_driver_set_color(int index, uint8_t red, uint8_t green, uint8_t blue) {
    if (index < ISSI_LED_TOTAL) {
        IS31FL3741_set_color(index, red, green, blue);
    } else {
#    if WS2812_LED_TOTAL > 0
        rgb_matrix_ws2812_array[index - ISSI_LED_TOTAL].r = red;
        rgb_matrix_ws2812_array[index - ISSI_LED_TOTAL].g = green;
        rgb_matrix_ws2812_array[index - ISSI_LED_TOTAL].b = blue;
#    endif
    }
}
static void rgb_matrix_driver_set_color_all(uint8_t red, uint8_t green, uint8_t blue) {
    IS31FL3741_set_color_all(red, green, blue);
#    if WS2812_LED_TOTAL > 0
    for (uint8_t i = 0; i < WS2812_LED_TOTAL; i++) {
        rgb_matrix_ws2812_array[i].r = red;
        rgb_matrix_ws2812_array[i].g = green;
        rgb_matrix_ws2812_array[i].b = blue;
    }
#    endif
}
// clang-format off
const rgb_matrix_driver_t rgb_matrix_driver = {
    .init          = rgb_matrix_driver_init,
    .flush         = rgb_matrix_driver_flush,
    .set_color     = rgb_matrix_driver_set_color,
    .set_color_all = rgb_matrix_driver_set_color_all,
};
// clang-format on
#endif /* RGB_MATRIX_ENABLE */
