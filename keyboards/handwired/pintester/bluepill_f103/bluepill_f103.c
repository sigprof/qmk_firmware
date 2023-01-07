// Copyright 2023 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later

#include <pintester.h>

void keyboard_pre_init_kb(void) {
    if (pintester_is_pin_enabled(A13) || pintester_is_pin_enabled(A14)) {
        // Disable JTAG and SWD to release all pins as GPIOs.
        AFIO->MAPR = (AFIO->MAPR & ~AFIO_MAPR_SWJ_CFG_Msk) | AFIO_MAPR_SWJ_CFG_DISABLE;
    }
}
