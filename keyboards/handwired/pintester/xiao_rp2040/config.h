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
#define PINTESTER_PINS           \
	PT_PIN_LABEL(GP26, "0"), \
	PT_PIN_LABEL(GP27, "1"), \
	PT_PIN_LABEL(GP28, "2"), \
	PT_PIN_LABEL(GP29, "3"), \
	PT_PIN_LABEL(GP6,  "4"), \
	PT_PIN_LABEL(GP7,  "5"), \
	PT_PIN_LABEL(GP0,  "6"), \
	PT_PIN_LABEL(GP1,  "7"), \
	PT_PIN_LABEL(GP2,  "8"), \
	PT_PIN_LABEL(GP4,  "9"), \
	PT_PIN_LABEL(GP3, "10")

// clang-format on

// Total number of GPIO pins available for the keyboard matrix (must match the
// number of items in PINTESTER_PINS).
#define PINTESTER_PIN_COUNT 11

// ceil(PINTESTER_PIN_COUNT * PINTESTER_PIN_COUNT / MATRIX_COLS)
// Precalculated to satisfy the data-driven code.
#define MATRIX_ROWS 4
