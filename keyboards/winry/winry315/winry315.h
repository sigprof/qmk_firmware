// Copyright 2022 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "quantum.h"

// Supported orientations of the board.  These values must match the layout
// option values used by VIA.
enum winry315_orientation {
    WINRY315_ORIENTATION_TOP,    // Encoders at the top side (default)
    WINRY315_ORIENTATION_LEFT,   // Encoders at the left side
    WINRY315_ORIENTATION_RIGHT,  // Encoders at the right side
    WINRY315_ORIENTATION_BOTTOM  // Encoders at the bottom side
};

// Set the orientation of the board (changes the RGB Matrix effect behavior to
// match the new orientation).
void winry315_set_orientation(uint8_t orientation);

// clang-format off

/* This is a shortcut to help you visually see your layout.
 *
 * The first section contains all of the arguments representing the physical
 * layout of the board and position of the keys.
 *
 * The second converts the arguments into a two-dimensional array which
 * represents the switch matrix.
 */
#define LAYOUT_all(             \
      k17,    k15,   k16,       \
    k00, k01, k02, k03, k04,    \
    k05, k06, k07, k08, k09,    \
    k10, k11, k12, k13, k14     \
) { \
    { k00, k01, k02, k03, k04, k05, k06, k07, k08, k09, k10, k11, k12, k13, k14, k15, k16, k17, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO } \
}

// clang-format on
