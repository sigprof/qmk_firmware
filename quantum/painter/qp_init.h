// Copyright 2024 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "qp.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generic representation of the device initialization commands

// Parameters for qp_comms_bulk_command_sequence()
typedef struct qp_bulk_command_sequence_t {
    const uint8_t *data;
    size_t         size;
} qp_bulk_command_sequence_t;

// Same as qp_bulk_command_sequence_t above, but with a non-const data pointer
typedef struct qp_mutable_bulk_command_sequence_t {
    uint8_t *data;
    size_t   size;
} qp_mutable_bulk_command_sequence_t;

// Device initialization commands
typedef struct qp_device_generic_init_sequence_t {
    qp_bulk_command_sequence_t         init;
    qp_bulk_command_sequence_t         custom_init;
    qp_bulk_command_sequence_t         enable;
    qp_mutable_bulk_command_sequence_t configure;
} qp_device_generic_init_sequence_t;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generic device initialization functions

bool qp_device_generic_init(painter_device_t device, qp_device_generic_init_sequence_t *init_sequence);
