// Copyright 2023 Nick Brassel (@tzarc)
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include "color.h"
#include "qp_internal.h"
#include "qp_surface_internal.h"
#include "qp_surface_backed_driver.h"

#ifdef QUANTUM_PAINTER_SPI_ENABLE
#    include "qp_comms_spi.h"
#endif // QUANTUM_PAINTER_SPI_ENABLE

#ifdef QUANTUM_PAINTER_I2C_ENABLE
#    include "qp_comms_i2c.h"
#endif // QUANTUM_PAINTER_I2C_ENABLE

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Common OLED panel implementation

// Driver vtable with extras
typedef struct oled_panel_painter_driver_vtable_t {
    surface_backed_painter_driver_vtable_t sbp; // must be first, so it can be cast to/from the painter_driver_vtable_t* type

    // Opcodes for normal display operation
    struct {
        uint8_t display_on;
        uint8_t display_off;
        uint8_t set_page;
        uint8_t set_column_lsb;
        uint8_t set_column_msb;
    } opcodes;
} oled_panel_painter_driver_vtable_t;

// Device definition
typedef struct oled_panel_painter_device_t {
    surface_backed_painter_driver_t sbp; // must be first, so it can be cast to/from the painter_device_t* type

    union {
#ifdef QUANTUM_PAINTER_SPI_ENABLE
        // SPI-based configurables
        qp_comms_spi_dc_reset_config_t spi_dc_reset_config;
#endif // QUANTUM_PAINTER_SPI_ENABLE
#ifdef QUANTUM_PAINTER_I2C_ENABLE
        // I2C-based configurables
        qp_comms_i2c_config_t i2c_config;
#endif // QUANTUM_PAINTER_I2C_ENABLE
    };
} oled_panel_painter_device_t;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declarations for injecting into concrete driver vtables

bool qp_oled_panel_power(painter_device_t device, bool power_on);
bool qp_oled_panel_clear(painter_device_t device);

// Helpers for flushing data from the dirty region to the correct location on the OLED
void qp_oled_panel_page_column_flush_rot0(painter_device_t device, surface_dirty_data_t *dirty, const uint8_t *framebuffer);
void qp_oled_panel_page_column_flush_rot90(painter_device_t device, surface_dirty_data_t *dirty, const uint8_t *framebuffer);
void qp_oled_panel_page_column_flush_rot180(painter_device_t device, surface_dirty_data_t *dirty, const uint8_t *framebuffer);
void qp_oled_panel_page_column_flush_rot270(painter_device_t device, surface_dirty_data_t *dirty, const uint8_t *framebuffer);
