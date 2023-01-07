// Copyright 2023 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "config_common.h"

// clang-format off

// List of all GPIO pins available for the keyboard matrix.  Each element of
// the list must be one of the following macros:
//   - PT_PIN(pin) - normal pin;
//   - PT_PIN_LABEL(pin, label) - pin for which the label on the actual PCB is
//     significantly different from the QMK pin name.
#define PINTESTER_PINS  \
        PT_PIN(B12),    \
        PT_PIN(B13),    \
        PT_PIN(B14),    \
        PT_PIN(B15),    \
        PT_PIN(A8),     \
        PT_PIN(A9),     \
        PT_PIN(A10),    \
        PT_PIN(A15),    \
        PT_PIN(B3),     \
        PT_PIN(B4),     \
        PT_PIN(B5),     \
        PT_PIN(B6),     \
        PT_PIN(B7),     \
        PT_PIN(B8),     \
        PT_PIN(B9),     \
        PT_PIN(C13),    \
        PT_PIN(C14),    \
        PT_PIN(C15),    \
        PT_PIN(A0),     \
        PT_PIN(A1),     \
        PT_PIN(A2),     \
        PT_PIN(A3),     \
        PT_PIN(A4),     \
        PT_PIN(A5),     \
        PT_PIN(A6),     \
        PT_PIN(A7),     \
        PT_PIN(B0),     \
        PT_PIN(B1),     \
        PT_PIN(B10),    \
        PT_PIN(B11),    \
        PT_PIN(A13),    \
        PT_PIN(A14)

// clang-format on

// Exclude SWD pins by default.
#define PINTESTER_DEFAULT_IGNORE_PINS A13, A14

// Total number of GPIO pins available for the keyboard matrix (must match the
// number of items in PINTESTER_PINS).
#define PINTESTER_PIN_COUNT 32

// ceil(PINTESTER_PIN_COUNT * PINTESTER_PIN_COUNT / MATRIX_COLS)
// Precalculated to satisfy the data-driven code.
#define MATRIX_ROWS 32
