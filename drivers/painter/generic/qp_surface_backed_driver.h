// Copyright 2023 Nick Brassel (@tzarc)
// Copyright 2024 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include "color.h"
#include "qp_internal.h"
#include "qp_surface_internal.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Common driver routines for a device which needs a backing surface

// Driver vtable with extras
typedef struct surface_backed_painter_driver_vtable_t {
    painter_driver_vtable_t base; // must be first, so it can be cast to/from the painter_driver_vtable_t* type

    // Nothing here yet
} surface_backed_painter_driver_vtable_t;

// Device definition
typedef struct surface_backed_painter_driver_t {
    painter_driver_t base; // must be first, so it can be cast to/from the painter_device_t* type

    surface_painter_device_t surface;
} surface_backed_painter_driver_t;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declarations for injecting into concrete driver vtables

bool qp_surface_backed_passthru_pixdata(painter_device_t device, const void *pixel_data, uint32_t native_pixel_count);
bool qp_surface_backed_passthru_viewport(painter_device_t device, uint16_t left, uint16_t top, uint16_t right, uint16_t bottom);
bool qp_surface_backed_passthru_palette_convert(painter_device_t device, int16_t palette_size, qp_pixel_t *palette);
bool qp_surface_backed_passthru_append_pixels(painter_device_t device, uint8_t *target_buffer, qp_pixel_t *palette, uint32_t pixel_offset, uint32_t pixel_count, uint8_t *palette_indices);
bool qp_surface_backed_passthru_append_pixdata(painter_device_t device, uint8_t *target_buffer, uint32_t pixdata_offset, uint8_t pixdata_byte);
