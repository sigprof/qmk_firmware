// Copyright 2023 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// Including "config_common.h" here breaks the build (QMK defines for GPIO pins
// conflict with some names from Kinetis headers).

// clang-format off

// List of all GPIO pins available for the keyboard matrix.  Each element of
// the list must be one of the following macros:
//   - PT_PIN(pin) - normal pin;
//   - PT_PIN_LABEL(pin, label) - pin for which the label on the actual PCB is
//     significantly different from the QMK pin name.
#define PINTESTER_PINS           \
        PT_PIN_LABEL(B16,  "0"), \
        PT_PIN_LABEL(B17,  "1"), \
        PT_PIN_LABEL(D0,   "2"), \
        PT_PIN_LABEL(A12,  "3"), \
        PT_PIN_LABEL(A13,  "4"), \
        PT_PIN_LABEL(D7,   "5"), \
        PT_PIN_LABEL(D4,   "6"), \
        PT_PIN_LABEL(D2,   "7"), \
        PT_PIN_LABEL(D3,   "8"), \
        PT_PIN_LABEL(C3,   "9"), \
        PT_PIN_LABEL(C4,  "10"), \
        PT_PIN_LABEL(C6,  "11"), \
        PT_PIN_LABEL(C7,  "12"), \
        PT_PIN_LABEL(C5,  "13"), \
        PT_PIN_LABEL(D1,  "14"), \
        PT_PIN_LABEL(C0,  "15"), \
        PT_PIN_LABEL(B0,  "16"), \
        PT_PIN_LABEL(B1,  "17"), \
        PT_PIN_LABEL(B3,  "18"), \
        PT_PIN_LABEL(B2,  "19"), \
        PT_PIN_LABEL(D5,  "20"), \
        PT_PIN_LABEL(D6,  "21"), \
        PT_PIN_LABEL(C1,  "22"), \
        PT_PIN_LABEL(C2,  "23"), \
        PT_PIN_LABEL(A5,  "24"), \
        PT_PIN_LABEL(B19, "25"), \
        PT_PIN_LABEL(E1,  "26"), \
        PT_PIN_LABEL(C9,  "27"), \
        PT_PIN_LABEL(C8,  "28"), \
        PT_PIN_LABEL(C10, "29"), \
        PT_PIN_LABEL(C11, "30"), \
        PT_PIN_LABEL(E0,  "31"), \
        PT_PIN_LABEL(B18, "32"), \
        PT_PIN_LABEL(A4,  "33")

// clang-format on

// Note that some pins on the Teensy 3.2 board are analog-only and do not work
// as digital GPIOs:
//   ADC0_DP0 (A10)
//   ADC0_DM0 (A11)
//   ADC0_DP3 (A12)
//   ADC0_DM3 (A13)
//   DAC0     (A14)

// The C5 pin has a LED on it, which results in an undefined logical level if
// that pin is used as an input.
#define PINTESTER_DEFAULT_IGNORE_PINS C5

// Total number of GPIO pins available for the keyboard matrix (must match the
// number of items in PINTESTER_PINS).
#define PINTESTER_PIN_COUNT 34

// ceil(PINTESTER_PIN_COUNT * PINTESTER_PIN_COUNT / MATRIX_COLS)
// Precalculated to satisfy the data-driven code.
#define MATRIX_ROWS 37
