// Copyright 2023 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "config_common.h"

// Always use 32 logical columns to support the maximum possible number of pins
// (89*89 bits would require 248 rows; 90*90 bits would require 254 rows, which
// won't work due to overlap with some special values).
#define MATRIX_COLS 32

// Disable features which cannot be used by this “keyboard”.
#define NO_ACTION_TAPPING
#define NO_ACTION_LAYER
