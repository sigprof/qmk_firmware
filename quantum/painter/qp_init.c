// Copyright 2024 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "qp.h"
#include "qp_init.h"
#include "qp_comms.h"

static void send_command_sequence(painter_device_t device, const qp_bulk_command_sequence_t *sequence) {
    if (sequence->data && sequence->size) {
        qp_comms_bulk_command_sequence(device, sequence->data, sequence->size);
    }
}

bool qp_device_generic_init(painter_device_t device, qp_device_generic_init_sequence_t *init_sequence) {
    send_command_sequence(device, &init_sequence->init);
    send_command_sequence(device, &init_sequence->custom_init);
    send_command_sequence(device, &init_sequence->enable);
    send_command_sequence(device, (const qp_bulk_command_sequence_t *)&init_sequence->configure);
    return true;
}
