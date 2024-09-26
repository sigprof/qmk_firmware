// Copyright 2023 Nick Brassel (@tzarc)
// Copyright 2024 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include "gpio.h"
#include "qp_init.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Quantum Painter ST7567S configurables (add to your keyboard's config.h)

#if defined(QUANTUM_PAINTER_ST7567S_SPI_ENABLE) && !defined(ST7567S_NUM_SPI_DEVICES)
/**
 * @def This controls the maximum number of SPI ST7567S devices that Quantum Painter can communicate with at any one time.
 *      Increasing this number allows for multiple displays to be used.
 */
#    define ST7567S_NUM_SPI_DEVICES 1
#else
#    define ST7567S_NUM_SPI_DEVICES 0
#endif

#if defined(QUANTUM_PAINTER_ST7567S_I2C_ENABLE) && !defined(ST7567S_NUM_I2C_DEVICES)
/**
 * @def This controls the maximum number of I2C ST7567S devices that Quantum Painter can communicate with at any one time.
 *      Increasing this number allows for multiple displays to be used.
 */
#    define ST7567S_NUM_I2C_DEVICES 1
#else
#    define ST7567S_NUM_I2C_DEVICES 0
#endif

#define ST7567S_NUM_DEVICES ((ST7567S_NUM_SPI_DEVICES) + (ST7567S_NUM_I2C_DEVICES))

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Quantum Painter ST7567S custom initialization hook

bool qp_st7567s_custom_init(painter_device_t device, painter_rotation_t rotation, qp_device_generic_init_sequence_t *init_sequence);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Quantum Painter ST7567S device factories

#ifdef QUANTUM_PAINTER_ST7567S_SPI_ENABLE

/**
 * Factory method for an ST7567S SPI LCD device.
 *
 * @param panel_width[in] the width of the display in pixels (usually 128)
 * @param panel_height[in] the height of the display in pixels (usually 64)
 * @param chip_select_pin[in] the GPIO pin used for SPI chip select
 * @param dc_pin[in] the GPIO pin used for D/C control
 * @param reset_pin[in] the GPIO pin used for RST
 * @param spi_divisor[in] the SPI divisor to use when communicating with the display
 * @param spi_mode[in] the SPI mode to use when communicating with the display
 * @return the device handle used with all drawing routines in Quantum Painter
 */
painter_device_t qp_st7567s_make_spi_device(uint16_t panel_width, uint16_t panel_height, pin_t chip_select_pin, pin_t dc_pin, pin_t reset_pin, uint16_t spi_divisor, int spi_mode);

#endif // QUANTUM_PAINTER_ST7567S_SPI_ENABLE

#ifdef QUANTUM_PAINTER_ST7567S_I2C_ENABLE

/**
 * Factory method for an ST7567S I2C LCD device.
 *
 * @param panel_width[in] the width of the display in pixels (usually 128)
 * @param panel_height[in] the height of the display in pixels (usually 64)
 * @param i2c_address[in] the I2C address to use
 * @return the device handle used with all drawing routines in Quantum Painter
 */
painter_device_t qp_st7567s_make_i2c_device(uint16_t panel_width, uint16_t panel_height, uint8_t i2c_address);

#endif // QUANTUM_PAINTER_ST7567S_I2C_ENABLE
