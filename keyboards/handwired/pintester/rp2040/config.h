// Copyright 2023 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "config_common.h"

/*
 * List of all GPIO pins available for the keyboard matrix.  Each element of
 * the list must be one of the following macros:
 *   - PT_PIN(pin) - normal pin;
 *   - PT_PIN_LABEL(pin, label) - pin for which the label on the actual PCB is
 *     significantly different from the QMK pin name.
 */
#define PINTESTER_PINS  \
        PT_PIN(GP0),    \
        PT_PIN(GP1),    \
        PT_PIN(GP2),    \
        PT_PIN(GP3),    \
        PT_PIN(GP4),    \
        PT_PIN(GP5),    \
        PT_PIN(GP6),    \
        PT_PIN(GP7),    \
        PT_PIN(GP8),    \
        PT_PIN(GP9),    \
        PT_PIN(GP10),   \
        PT_PIN(GP11),   \
        PT_PIN(GP12),   \
        PT_PIN(GP13),   \
        PT_PIN(GP14),   \
        PT_PIN(GP15),   \
        PT_PIN(GP16),   \
        PT_PIN(GP17),   \
        PT_PIN(GP18),   \
        PT_PIN(GP19),   \
        PT_PIN(GP20),   \
        PT_PIN(GP21),   \
        PT_PIN(GP22),   \
        PT_PIN(GP23),   \
        PT_PIN(GP24),   \
        PT_PIN(GP25),   \
        PT_PIN(GP26),   \
        PT_PIN(GP27),   \
        PT_PIN(GP28),   \
        PT_PIN(GP29)

// Ignore pins which are not available on Raspberry Pi Pico by default.
#define PINTESTER_DEFAULT_IGNORE_PINS   GP23, GP24, GP25, GP29

/*
 * Total number of GPIO pins available for the keyboard matrix (must match the
 * number of items in PINTESTER_PINS).
 */
#define PINTESTER_PIN_COUNT 30

// ceil(PINTESTER_PIN_COUNT * PINTESTER_PIN_COUNT / MATRIX_COLS)
// Precalculated to satisfy the data-driven code.
#define MATRIX_ROWS 29
