/*
 * Copyright 2021 Sergey Vlasov <sigprof@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
