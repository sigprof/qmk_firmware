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
