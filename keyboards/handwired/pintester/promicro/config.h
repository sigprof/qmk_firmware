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
#define PINTESTER_PINS                  \
        PT_PIN_LABEL(D3, "TXO/1"),      \
        PT_PIN_LABEL(D2, "RXI/0"),      \
        PT_PIN_LABEL(D1, "2"),          \
        PT_PIN_LABEL(D0, "3"),          \
        PT_PIN_LABEL(D4, "4"),          \
        PT_PIN_LABEL(C6, "5"),          \
        PT_PIN_LABEL(D7, "6"),          \
        PT_PIN_LABEL(E6, "7"),          \
        PT_PIN_LABEL(B4, "8"),          \
        PT_PIN_LABEL(B5, "9"),          \
        PT_PIN_LABEL(B6, "10"),         \
        PT_PIN_LABEL(B2, "16"),         \
        PT_PIN_LABEL(B3, "14"),         \
        PT_PIN_LABEL(B1, "15"),         \
        PT_PIN_LABEL(F7, "A0"),         \
        PT_PIN_LABEL(F6, "A1"),         \
        PT_PIN_LABEL(F5, "A2"),         \
        PT_PIN_LABEL(F4, "A3")

/*
 * Total number of GPIO pins available for the keyboard matrix (must match the
 * number of items in PINTESTER_PINS).
 */
#define PINTESTER_PIN_COUNT 18

// ceil(PINTESTER_PIN_COUNT * PINTESTER_PIN_COUNT / MATRIX_COLS)
// Precalculated to satisfy the data-driven code.
#define MATRIX_ROWS 11
