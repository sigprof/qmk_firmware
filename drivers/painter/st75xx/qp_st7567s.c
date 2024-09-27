// Copyright 2023 Nick Brassel (@tzarc)
// Copyright 2024 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "qp_internal.h"
#include "qp_comms.h"
#include "qp_oled_panel.h"
#include "qp_st7567s.h"
#include "qp_st75xx_opcodes.h"
#include "qp_surface.h"
#include "qp_surface_internal.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Driver storage
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct st7567s_device_t {
    oled_panel_painter_device_t oled;

    uint8_t framebuffer[SURFACE_REQUIRED_BUFFER_BYTE_SIZE(132, 64, 1)];
} st7567s_device_t;

static st7567s_device_t st7567s_drivers[ST7567S_NUM_DEVICES] = {0};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Quantum Painter API implementations

__attribute__((weak)) bool qp_st7567s_custom_init(painter_device_t device, painter_rotation_t rotation, qp_device_generic_init_sequence_t *init_sequence) {
    return qp_device_generic_init(device, init_sequence);
}

// Initialisation
__attribute__((weak)) bool qp_st7567s_init(painter_device_t device, painter_rotation_t rotation) {
    st7567s_device_t *driver = (st7567s_device_t *)device;

    // Change the surface geometry based on the panel rotation
    if (rotation == QP_ROTATION_90 || rotation == QP_ROTATION_270) {
        driver->oled.surface.base.panel_width  = driver->oled.base.panel_height;
        driver->oled.surface.base.panel_height = driver->oled.base.panel_width;
    } else {
        driver->oled.surface.base.panel_width  = driver->oled.base.panel_width;
        driver->oled.surface.base.panel_height = driver->oled.base.panel_height;
    }

    // Init the internal surface
    if (!qp_init(&driver->oled.surface.base, QP_ROTATION_0)) {
        qp_dprintf("Failed to init internal surface in qp_st7567s_init\n");
        return false;
    }

    // clang-format off
    static const uint8_t st7567s_init_sequence[] = {
        // Command,                       Delay,  N, Data[N]
        ST75XX_RESET,                        10,  0,
        ST75XX_BIAS_LOW,                      0,  0,
        ST75XX_SEG_SCAN_DIR_NORMAL,           0,  0,
        ST75XX_COM_SCAN_DIR_REVERSE,          0,  0,
        ST75XX_REGULATION_RATIO | 5,          0,  0,
        ST75XX_SET_EV,                        0,  1, 0x20,
        ST75XX_POWER_CONTROL_BOOSTER,         0,  0,
        ST75XX_POWER_CONTROL_BOOSTER_VR,      0,  0,
        ST75XX_POWER_CONTROL_BOOSTER_VR_VF,   0,  0,
        ST75XX_DISPLAY_START_LINE,            0,  0,
        ST75XX_ALL_ON_RESUME,                 0,  0,
        ST75XX_NON_INVERTING_DISPLAY,         0,  0,
    };
    static const uint8_t st7567s_enable_sequence[] = {
        ST75XX_DISPLAY_ON,                    0,  0,
    };
    // clang-format on

    qp_device_generic_init_sequence_t init_sequence = {
        .init   = {.data = st7567s_init_sequence, .size = sizeof(st7567s_init_sequence)},
        .enable = {.data = st7567s_enable_sequence, .size = sizeof(st7567s_enable_sequence)},
    };
    return qp_st7567s_custom_init(device, rotation, &init_sequence);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Driver vtable
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const oled_panel_painter_driver_vtable_t st7567s_driver_vtable = {
    .base =
        {
            .init            = qp_st7567s_init,
            .power           = qp_oled_panel_power,
            .clear           = qp_oled_panel_clear,
            .flush           = qp_oled_panel_page_column_flush,
            .pixdata         = qp_oled_panel_passthru_pixdata,
            .viewport        = qp_oled_panel_passthru_viewport,
            .palette_convert = qp_oled_panel_passthru_palette_convert,
            .append_pixels   = qp_oled_panel_passthru_append_pixels,
            .append_pixdata  = qp_oled_panel_passthru_append_pixdata,
        },
    .opcodes =
        {
            .display_on     = ST75XX_DISPLAY_ON,
            .display_off    = ST75XX_DISPLAY_OFF,
            .set_page       = ST75XX_PAGE_ADDR,
            .set_column_lsb = ST75XX_SETCOLUMN_LSB,
            .set_column_msb = ST75XX_SETCOLUMN_MSB,
        },
};

#ifdef QUANTUM_PAINTER_ST7567S_SPI_ENABLE
// Factory function for creating a handle to the ST7567S device
painter_device_t qp_st7567s_make_spi_device(uint16_t panel_width, uint16_t panel_height, pin_t chip_select_pin, pin_t dc_pin, pin_t reset_pin, uint16_t spi_divisor, int spi_mode) {
    for (uint32_t i = 0; i < ST7567S_NUM_DEVICES; ++i) {
        st7567s_device_t *driver = &st7567s_drivers[i];
        if (!driver->oled.base.driver_vtable) {
            painter_device_t surface = qp_make_mono1bpp_surface_advanced(&driver->oled.surface, 1, panel_width, panel_height, driver->framebuffer);
            if (!surface) {
                return NULL;
            }

            // Setup the OLED device
            driver->oled.base.driver_vtable         = (const painter_driver_vtable_t *)&st7567s_driver_vtable;
            driver->oled.base.comms_vtable          = (const painter_comms_vtable_t *)&spi_comms_with_dc_vtable;
            driver->oled.base.native_bits_per_pixel = 1; // 1bpp mono
            driver->oled.base.panel_width           = panel_width;
            driver->oled.base.panel_height          = panel_height;
            driver->oled.base.rotation              = QP_ROTATION_0;
            driver->oled.base.offset_x              = 0;
            driver->oled.base.offset_y              = 0;

            // SPI and other pin configuration
            driver->oled.base.comms_config                                   = &driver->oled.spi_dc_reset_config;
            driver->oled.spi_dc_reset_config.spi_config.chip_select_pin      = chip_select_pin;
            driver->oled.spi_dc_reset_config.spi_config.divisor              = spi_divisor;
            driver->oled.spi_dc_reset_config.spi_config.lsb_first            = false;
            driver->oled.spi_dc_reset_config.spi_config.mode                 = spi_mode;
            driver->oled.spi_dc_reset_config.dc_pin                          = dc_pin;
            driver->oled.spi_dc_reset_config.reset_pin                       = reset_pin;
            driver->oled.spi_dc_reset_config.command_params_uses_command_pin = true;

            if (!qp_internal_register_device((painter_device_t)driver)) {
                memset(driver, 0, sizeof(st7567s_device_t));
                return NULL;
            }

            return (painter_device_t)driver;
        }
    }
    return NULL;
}

#endif // QUANTUM_PAINTER_ST7567S_SPI_ENABLE

#ifdef QUANTUM_PAINTER_ST7567S_I2C_ENABLE
// Factory function for creating a handle to the ST7567S device
painter_device_t qp_st7567s_make_i2c_device(uint16_t panel_width, uint16_t panel_height, uint8_t i2c_address) {
    for (uint32_t i = 0; i < ST7567S_NUM_DEVICES; ++i) {
        st7567s_device_t *driver = &st7567s_drivers[i];
        if (!driver->oled.base.driver_vtable) {
            // Instantiate the surface, intentional swap of width/high due to transpose
            painter_device_t surface = qp_make_mono1bpp_surface_advanced(&driver->oled.surface, 1, panel_width, panel_height, driver->framebuffer);
            if (!surface) {
                return NULL;
            }

            // Setup the OLED device
            driver->oled.base.driver_vtable         = (const painter_driver_vtable_t *)&st7567s_driver_vtable;
            driver->oled.base.comms_vtable          = (const painter_comms_vtable_t *)&i2c_comms_cmddata_vtable;
            driver->oled.base.native_bits_per_pixel = 1; // 1bpp mono
            driver->oled.base.panel_width           = panel_width;
            driver->oled.base.panel_height          = panel_height;
            driver->oled.base.rotation              = QP_ROTATION_0;
            driver->oled.base.offset_x              = 0;
            driver->oled.base.offset_y              = 0;

            // I2C configuration
            driver->oled.base.comms_config       = &driver->oled.i2c_config;
            driver->oled.i2c_config.chip_address = i2c_address;

            if (!qp_internal_register_device((painter_device_t)driver)) {
                memset(driver, 0, sizeof(st7567s_device_t));
                return NULL;
            }

            return (painter_device_t)driver;
        }
    }
    return NULL;
}

#endif // QUANTUM_PAINTER_ST7567S_I2C_ENABLE
