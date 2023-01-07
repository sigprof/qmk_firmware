// Copyright 2023 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "quantum.h"

struct pintester_pin_info {
    const char *name;
    const char *label;
    pin_t       pin;
};

#define PT_PIN(pin_) \
    { .pin = pin_, .name = #pin_ }
#define PT_PIN_LABEL(pin_, label_) \
    { .pin = pin_, .name = #pin_, .label = label_ }

struct pintester_pin_state {
    bool active : 1;
    bool enabled : 1;
};

extern const struct pintester_pin_info pin_info[];
extern struct pintester_pin_state      pin_state[];

bool pintester_is_pin_enabled(pin_t pin);
