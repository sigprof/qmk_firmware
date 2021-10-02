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

#define PRODUCT Pin Tester Teensy++ 2.0

/*
 * List of all GPIO pins available for the keyboard matrix.  Each element of
 * the list must be one of the following macros:
 *   - PT_PIN(pin) - normal pin;
 *   - PT_PIN_LABEL(pin, label) - pin for which the label on the actual PCB is
 *     significantly different from the QMK pin name.
 */
#define PINTESTER_PINS  \
        PT_PIN(B7),     \
        PT_PIN(D0),     \
        PT_PIN(D1),     \
        PT_PIN(D2),     \
        PT_PIN(D3),     \
        PT_PIN(D4),     \
        PT_PIN(D5),     \
        PT_PIN(D6),     \
        PT_PIN(D7),     \
        PT_PIN(E0),     \
        PT_PIN(E1),     \
        PT_PIN(C0),     \
        PT_PIN(C1),     \
        PT_PIN(C2),     \
        PT_PIN(C3),     \
        PT_PIN(C4),     \
        PT_PIN(C5),     \
        PT_PIN(C6),     \
        PT_PIN(C7),     \
        PT_PIN(F7),     \
        PT_PIN(F6),     \
        PT_PIN(F5),     \
        PT_PIN(F4),     \
        PT_PIN(F3),     \
        PT_PIN(F2),     \
        PT_PIN(F1),     \
        PT_PIN(F0),     \
        PT_PIN(E6),     \
        PT_PIN(E7),     \
        PT_PIN(B0),     \
        PT_PIN(B1),     \
        PT_PIN(B2),     \
        PT_PIN(B3),     \
        PT_PIN(B4),     \
        PT_PIN(B5),     \
        PT_PIN(B6),     \
        PT_PIN(E4),     \
        PT_PIN(E5),     \
        PT_PIN(A0),     \
        PT_PIN(A1),     \
        PT_PIN(A2),     \
        PT_PIN(A3),     \
        PT_PIN(A4),     \
        PT_PIN(A5),     \
        PT_PIN(A6),     \
        PT_PIN(A7)

// The D6 pin has a LED on it, which results in an undefined logical level if
// that pin is used as an input.
#define PINTESTER_DEFAULT_IGNORE_PINS   D6

/*
 * Total number of GPIO pins available for the keyboard matrix (must match the
 * number of items in PINTESTER_PINS).
 */
#define PINTESTER_PIN_COUNT 46
