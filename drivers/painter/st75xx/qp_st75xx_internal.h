// Copyright 2023 Nick Brassel (@tzarc)
// Copyright 2024 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include "qp_oled_panel.h"

typedef struct st75xx_device_t {
    oled_panel_painter_device_t oled;

    uint8_t framebuffer[SURFACE_REQUIRED_BUFFER_BYTE_SIZE(132, 64, 1)];
} st75xx_device_t;

bool qp_st75xx_flush(painter_device_t device);
