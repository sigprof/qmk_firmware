// Copyright 2021 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define DYNAMIC_KEYMAP_LAYER_COUNT 8

// Enable a limited form of RGB Matrix support in VIA (requires redefining the
// effect list in the VIA JSON, which then becomes not 100% compatible with the
// RGBLIGHT firmwares).
#define VIA_QMK_RGBLIGHT_ENABLE
