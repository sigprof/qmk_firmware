// Copyright 2023 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

void keyboard_pre_init_kb(void) {
    // Turn off the common anode RGB LED.
    setPinInputHigh(GP16);
    setPinInputHigh(GP17);
    setPinInputHigh(GP25);
}
