#include QMK_KEYBOARD_H

#ifdef RGB_MATRIX_ENABLE

#    include <i2c_master.h>
#    include <is31fl3733.h>
#    include <ws2812.h>

// clang-format off

const is31_led __flash g_is31_leds[DRIVER_LED_TOTAL] = {
    { 0, A_1, C_1, B_1 },
    { 0, A_2, C_2, B_2 },
    { 0, A_3, C_3, B_3 },
    { 0, A_4, C_4, B_4 },
    { 0, A_5, C_5, B_5 },
    { 0, A_6, C_6, B_6 },
    { 0, A_7, C_7, B_7 },
    { 0, A_8, C_8, B_8 },
    { 0, A_9, C_9, B_9 },
    { 0, A_10, C_10, B_10 },
    { 0, A_11, C_11, B_11 },
    { 0, A_12, C_12, B_12 },
    { 0, A_13, C_13, B_13 },
    { 0, A_14, C_14, B_14 },
    { 0, D_1, F_1, E_1 },
    { 0, D_2, F_2, E_2 },
    { 0, D_3, F_3, E_3 },
    { 0, D_4, F_4, E_4 },
    { 0, D_5, F_5, E_5 },
    { 0, D_6, F_6, E_6 },
    { 0, D_7, F_7, E_7 },
    { 0, D_8, F_8, E_8 },
    { 0, D_9, F_9, E_9 },
    { 0, D_10, F_10, E_10 },
    { 0, D_11, F_11, E_11 },
    { 0, D_12, F_12, E_12 },
    { 0, D_13, F_13, E_13 },
    { 0, D_14, F_14, E_14 },
    { 0, G_1, I_1, H_1 },
    { 0, G_2, I_2, H_2 },
    { 0, G_3, I_3, H_3 },
    { 0, G_4, I_4, H_4 },
    { 0, G_5, I_5, H_5 },
    { 0, G_6, I_6, H_6 },
    { 0, G_7, I_7, H_7 },
    { 0, G_8, I_8, H_8 },
    { 0, G_9, I_9, H_9 },
    { 0, G_10, I_10, H_10 },
    { 0, G_11, I_11, H_11 },
    { 0, G_12, I_12, H_12 },
    { 0, G_14, I_14, H_14 },
    { 0, J_1, L_1, K_1 },
    { 0, J_2, L_2, K_2 },
    { 0, J_3, L_3, K_3 },
    { 0, J_4, L_4, K_4 },
    { 0, J_5, L_5, K_5 },
    { 0, J_6, L_6, K_6 },
    { 0, J_7, L_7, K_7 },
    { 0, J_8, L_8, K_8 },
    { 0, J_9, L_9, K_9 },
    { 0, J_10, L_10, K_10 },
    { 0, J_12, L_12, K_12 },
    { 0, J_13, L_13, K_13 },
    { 0, J_14, L_14, K_14 },
    { 0, G_16, I_16, H_16 },
    { 0, G_15, I_15, H_15 },
    { 0, G_13, I_13, H_13 },
    { 0, D_15, F_15, E_15 },
    { 0, J_15, L_15, K_15 },
    { 0, J_11, L_11, K_11 },
    { 0, J_16, L_16, K_16 },
    { 0, A_16, C_16, B_16 },
    { 0, D_16, F_16, E_16 },
};

led_config_t g_led_config = {
    {
        {      0,      1,      2,      3,      4,      5,      6,      7,      8,      9,     10,     11,     12,     13, },
        {     14,     15,     16,     17,     18,     19,     20,     21,     22,     23,     24,     25,     26,     27, },
        {     28,     29,     30,     31,     32,     33,     34,     35,     36,     37,     38,     39, NO_LED,     40, },
        {     41,     42,     43,     44,     45,     46,     47,     48,     49,     50, NO_LED,     51,     52,     53, },
        {     54,     55,     56, NO_LED, NO_LED,     57, NO_LED, NO_LED, NO_LED,     58,     59,     60,     61,     62, },
    },
    {
        {  0,  0}, { 16,  0}, { 32,  0}, { 48,  0}, { 64,  0}, { 80,  0}, { 96,  0}, {112,  0}, {128,  0}, {144,  0}, {160,  0}, {176,  0}, {192,  0}, {216,  0},
        {  4, 16}, { 24, 16}, { 40, 16}, { 56, 16}, { 72, 16}, { 88, 16}, {104, 16}, {120, 16}, {136, 16}, {152, 16}, {168, 16}, {184, 16}, {200, 16}, {220, 16},
        {  6, 32}, { 28, 32}, { 44, 32}, { 60, 32}, { 76, 32}, { 92, 32}, {108, 32}, {124, 32}, {140, 32}, {156, 32}, {172, 32}, {188, 32}, {214, 32},
        { 10, 48}, { 36, 48}, { 52, 48}, { 68, 48}, { 84, 48}, {100, 48}, {116, 48}, {132, 48}, {148, 48}, {164, 48}, {186, 48}, {208, 48}, {224, 48},
        {  2, 64}, { 22, 64}, { 42, 64}, {102, 64}, {160, 64}, {176, 64}, {192, 64}, {208, 64}, {224, 64},
#    if WS2812_LED_TOTAL > 0
        { 28, 40}, { 62, 40}, { 96, 40}, {130, 40}, {164, 40}, {198, 40},
#    endif
    },
    {
        1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
        1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
        1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1, 4,
        1, 1, 1, 1, 1, 1, 1, 1, 1,
#    if WS2812_LED_TOTAL > 0
        2, 2, 2, 2, 2, 2,
#    endif
    },
};

// clang-format on

// ==========================================================================
// Custom RGB Matrix driver that combines IS31FL3733 and WS2812
// ==========================================================================

#    if WS2812_LED_TOTAL > 0
LED_TYPE rgb_matrix_ws2812_array[WS2812_LED_TOTAL];
#    endif

static void rgb_matrix_driver_init(void) {
    i2c_init();
    IS31FL3733_init(DRIVER_ADDR_1, 0);
    for (uint8_t index = 0; index < ISSI_LED_TOTAL; index++) {
        bool enabled = true;
        IS31FL3733_set_led_control_register(index, enabled, enabled, enabled);
    }
    IS31FL3733_update_led_control_registers(DRIVER_ADDR_1, 0);
}

static void rgb_matrix_driver_flush(void) {
    IS31FL3733_update_pwm_buffers(DRIVER_ADDR_1, 0);
#    if WS2812_LED_TOTAL > 0
    ws2812_setleds(rgb_matrix_ws2812_array, WS2812_LED_TOTAL);
#    endif
}

static void rgb_matrix_driver_set_color(int index, uint8_t red, uint8_t green, uint8_t blue) {
    if (index < ISSI_LED_TOTAL) {
        IS31FL3733_set_color(index, red, green, blue);
    } else {
#    if WS2812_LED_TOTAL > 0
        rgb_matrix_ws2812_array[index - ISSI_LED_TOTAL].r = red;
        rgb_matrix_ws2812_array[index - ISSI_LED_TOTAL].g = green;
        rgb_matrix_ws2812_array[index - ISSI_LED_TOTAL].b = blue;
#    endif
    }
}

static void rgb_matrix_driver_set_color_all(uint8_t red, uint8_t green, uint8_t blue) {
    IS31FL3733_set_color_all(red, green, blue);
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
