// Copyright 2024 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

//////////////////////////////////////////////////////////////////////////////
// ST7565A additional command opcodes

// Normal commands
#define ST7565A_SET_N_LINE 0x85            // Set N-Line inversion (requires 1 parameter byte)
#define ST7565A_RELEASE_N_LINE 0x84        // Exit N-Line inversion
#define ST7565A_ENTER_EXT_COMMAND_SET 0xFF // Enter extension command table (ST7565A_EXT_*)
#define ST7565A_EXIT_EXT_COMMAND_SET 0xFE  // Exit extension command table

// Extension commands which can be used after ST7565A_ENTER_EXT_COMMAND_SET
#define ST7565A_EXT_HIGH_POWER_MODE_ON 0x6B       // Enter high power mode
#define ST7565A_EXT_HIGH_POWER_MODE_OFF 0x64      // Exit high power mode
#define ST7565A_EXT_DISPLAY_SETTING_MODE_ON 0x72  // Enter display setting (requires 3 parameter bytes)
#define ST7565A_EXT_DISPLAY_SETTING_MODE_OFF 0x70 // Exit display setting
