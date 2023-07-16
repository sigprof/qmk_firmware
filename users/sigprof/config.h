// Copyright 2023 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// This must match `sizeof(custom_eeconfig_user_t)` (but the actual `sizeof`
// cannot be used in `config.h`, because the type definition is not available
// at the point where this size is used).
#define CUSTOM_EECONFIG_USER_SIZE 1

// `CUSTOM_EECONFIG_KEYMAP_SIZE` must match `sizeof(custom_eeconfig_keymap_t)`
// if the keymap needs to store some data in the EEPROM.
#ifdef CUSTOM_EECONFIG_KEYMAP_SIZE
#    define EECONFIG_USER_DATA_SIZE ((CUSTOM_EECONFIG_USER_SIZE) + (CUSTOM_EECONFIG_KEYMAP_SIZE))
#else
#    define EECONFIG_USER_DATA_SIZE CUSTOM_EECONFIG_USER_SIZE
#endif
