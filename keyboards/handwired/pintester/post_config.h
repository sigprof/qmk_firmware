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

// For the logical matrix layout use the maximum possible number of columns
// (32) and calculate the number of rows that would be needed to fit
// PINTESTER_PIN_COUNT*PINTESTER_PIN_COUNT bits.
#define MATRIX_COLS 32
#define MATRIX_ROWS ((PINTESTER_PIN_COUNT * PINTESTER_PIN_COUNT + 31) / 32)
