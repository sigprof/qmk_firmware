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

// Always use 32 logical columns to support the maximum possible number of pins
// (89*89 bits would require 248 rows; 90*90 bits would require 254 rows, which
// won't work due to overlap with some special values).
#define MATRIX_COLS 32

/* Disable features which cannot be used by this “keyboard”. */
#define NO_ACTION_TAPPING
#define NO_ACTION_LAYER
