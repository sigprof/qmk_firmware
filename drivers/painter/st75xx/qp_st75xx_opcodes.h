// Copyright 2024 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

//////////////////////////////////////////////////////////////////////////////
// Commands which are common for all ST75xx chips

#define ST75XX_DISPLAY_ON 0xAF                  // Display on
#define ST75XX_DISPLAY_OFF 0xAE                 // Display off
#define ST75XX_DISPLAY_START_LINE 0x40          // Set display start line (6 low bits of the command code)
#define ST75XX_SEG_SCAN_DIR_NORMAL 0xA0         // Set normal scan direction for SEG
#define ST75XX_SEG_SCAN_DIR_REVERSE 0xA1        // Set reverse scan direction for SEG
#define ST75XX_COM_SCAN_DIR_NORMAL 0xC0         // Set normal scan direction for COM
#define ST75XX_COM_SCAN_DIR_REVERSE 0xC8        // Set reverse scan direction for COM
#define ST75XX_ALL_ON_RESUME 0xA4               // Exit the "all pixels on" mode
#define ST75XX_ALL_ON 0xA5                      // Enter the "all pixels on" mode
#define ST75XX_NON_INVERTING_DISPLAY 0xA6       // Disable display inversion
#define ST75XX_INVERTING_DISPLAY 0xA7           // Enable display inversion
#define ST75XX_BIAS_LOW 0xA2                    // Select the lower bias ratio
#define ST75XX_BIAS_HIGH 0xA3                   // Select the higher bias ratio
#define ST75XX_POWER_CONTROL 0x28               // Enable the specified built-in power circuits (3 low bits of the command code)
#define ST75XX_POWER_CONTROL_BOOSTER 0x2C       // Enable internal booster
#define ST75XX_POWER_CONTROL_BOOSTER_VR 0x2E    // Enable internal booster and regulator
#define ST75XX_POWER_CONTROL_BOOSTER_VR_VF 0x2F // Enable internal booster, regulator and follower
#define ST75XX_POWER_CONTROL_VR_VF 0x2B         // Enable only the internal regulator and follower
#define ST75XX_POWER_CONTROL_VF 0x29            // Enable only the internal follower
#define ST75XX_POWER_CONTROL_EXTERNAL 0x28      // Disable all internal power circuits (external power supply mode)
#define ST75XX_REGULATION_RATIO 0x20            // Set regulation ratio (3 low bits of the command code)
#define ST75XX_SET_EV 0x81                      // Set electronic volume (contrast) setting (requires 1 parameter byte)
#define ST75XX_SET_BOOSTER 0xF8                 // Set booster ratio (requires 1 parameter byte)
#define ST75XX_RESET 0xE2                       // Software reset

#define ST75XX_SETCOLUMN_LSB 0x00 // Set 4 low bits of the column address (4 low bits of the command code)
#define ST75XX_SETCOLUMN_MSB 0x10 // Set 4 high bits of the column address (4 low bits of the command code)
#define ST75XX_PAGE_ADDR 0xB0     // Set page address (4 low bits of the command code)
