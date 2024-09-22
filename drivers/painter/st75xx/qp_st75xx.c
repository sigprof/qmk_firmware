// Copyright 2023 Nick Brassel (@tzarc)
// Copyright 2024 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "qp_internal.h"
#include "qp_comms.h"
#include "qp_oled_panel.h"
#include "qp_st75xx_internal.h"
#include "qp_st75xx_opcodes.h"
#include "qp_surface.h"
#include "qp_surface_internal.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Quantum Painter API implementations

// Screen flush
bool qp_st75xx_flush(painter_device_t device) {
    st75xx_device_t *driver = (st75xx_device_t *)device;

    if (!driver->oled.surface.dirty.is_dirty) {
        return true;
    }

    switch (driver->oled.base.rotation) {
        default:
        case QP_ROTATION_0:
            qp_oled_panel_page_column_flush_rot0(device, &driver->oled.surface.dirty, driver->framebuffer);
            break;
        case QP_ROTATION_90:
            qp_oled_panel_page_column_flush_rot90(device, &driver->oled.surface.dirty, driver->framebuffer);
            break;
        case QP_ROTATION_180:
            qp_oled_panel_page_column_flush_rot180(device, &driver->oled.surface.dirty, driver->framebuffer);
            break;
        case QP_ROTATION_270:
            qp_oled_panel_page_column_flush_rot270(device, &driver->oled.surface.dirty, driver->framebuffer);
            break;
    }

    // Clear the dirty area
    qp_flush(&driver->oled.surface);

    return true;
}
