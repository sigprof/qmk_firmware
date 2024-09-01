// Copyright 2023 Nick Brassel (@tzarc)
// Copyright 2024 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "color.h"
#include "qp_internal.h"
#include "qp_draw.h"
#include "qp_surface_backed_driver.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Surface passthru
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool qp_surface_backed_passthru_pixdata(painter_device_t device, const void *pixel_data, uint32_t native_pixel_count) {
    surface_backed_painter_driver_t *driver = (surface_backed_painter_driver_t *)device;
    return driver->surface.base.validate_ok && driver->surface.base.driver_vtable->pixdata(&driver->surface.base, pixel_data, native_pixel_count);
}

bool qp_surface_backed_passthru_viewport(painter_device_t device, uint16_t left, uint16_t top, uint16_t right, uint16_t bottom) {
    surface_backed_painter_driver_t *driver = (surface_backed_painter_driver_t *)device;
    return driver->surface.base.validate_ok && driver->surface.base.driver_vtable->viewport(&driver->surface.base, left, top, right, bottom);
}

bool qp_surface_backed_passthru_palette_convert(painter_device_t device, int16_t palette_size, qp_pixel_t *palette) {
    surface_backed_painter_driver_t *driver = (surface_backed_painter_driver_t *)device;
    return driver->surface.base.validate_ok && driver->surface.base.driver_vtable->palette_convert(&driver->surface.base, palette_size, palette);
}

bool qp_surface_backed_passthru_append_pixels(painter_device_t device, uint8_t *target_buffer, qp_pixel_t *palette, uint32_t pixel_offset, uint32_t pixel_count, uint8_t *palette_indices) {
    surface_backed_painter_driver_t *driver = (surface_backed_painter_driver_t *)device;
    return driver->surface.base.validate_ok && driver->surface.base.driver_vtable->append_pixels(&driver->surface.base, target_buffer, palette, pixel_offset, pixel_count, palette_indices);
}

bool qp_surface_backed_passthru_append_pixdata(painter_device_t device, uint8_t *target_buffer, uint32_t pixdata_offset, uint8_t pixdata_byte) {
    surface_backed_painter_driver_t *driver = (surface_backed_painter_driver_t *)device;
    return driver->surface.base.validate_ok && driver->surface.base.driver_vtable->append_pixdata(&driver->surface.base, target_buffer, pixdata_offset, pixdata_byte);
}
