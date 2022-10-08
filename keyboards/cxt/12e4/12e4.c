// Copyright 2022 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#ifdef ENCODER_ENABLE
bool encoder_update_kb(uint8_t index, bool clockwise) {
    if (!encoder_update_user(index, clockwise)) {
        return false;
    }
    switch (index) {
        case 0:
            tap_code(clockwise ? KC_WH_R : KC_WH_L);
            break;
        case 1:
            tap_code(clockwise ? KC_WH_D : KC_WH_U);
            break;
        case 2:
            tap_code(clockwise ? KC_PGDN : KC_PGUP);
            break;
        case 3:
            tap_code_delay(clockwise ? KC_VOLU : KC_VOLD, 10);
            break;
    }
    return true;
}
#endif
