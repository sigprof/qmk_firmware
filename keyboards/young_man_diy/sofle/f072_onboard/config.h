// Copyright 2025 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// I2C on pins B6/B7 (I2C1_SCL, I2C1_SDA)
#define I2C_DRIVER I2CD1
#define I2C1_SCL_PIN B6
#define I2C1_SDA_PIN B7
#define I2C1_SCL_PAL_MODE 1
#define I2C1_SDA_PAL_MODE 1
// I2C timings for fast mode (400 kHz) with 8 MHz clock (I2C1 clocked from HSI)
#define I2C1_TIMINGR_PRESC 0x00U
#define I2C1_TIMINGR_SCLDEL 0x03U
#define I2C1_TIMINGR_SDADEL 0x01U
#define I2C1_TIMINGR_SCLH 0x03U
#define I2C1_TIMINGR_SCLL 0x09U

// Split driver: USART half duplex on pin A2 (USART2_TX)
#define SERIAL_USART_DRIVER SD2
#define SERIAL_USART_TX_PAL_MODE 1

// WS2812 SPI on pin B15 (SPI2_MOSI), B13 (SPI2_SCK); MISO pin not enabled
#define WS2812_SPI_DRIVER SPID2
#define WS2812_SPI_MOSI_PAL_MODE 0
#define WS2812_SPI_SCK_PAL_MODE 0
#define WS2812_SPI_SCK_PIN B13
#define WS2812_SPI_USE_CIRCULAR_BUFFER

// Use EEPROM to store the handedness configuration
#define EE_HANDS

// Custom bootmagic implementation to handle handedness issues
// Key [0, 0] (left half, top left) - enter the bootloader
#define BOOTMAGIC_OUTPUT_PIN_LEFT B11
#define BOOTMAGIC_INPUT_PIN_LEFT B2
// Key [5, 0] (right half, top right) - enter the bootloader
#define BOOTMAGIC_OUTPUT_PIN_RIGHT A15
#define BOOTMAGIC_INPUT_PIN_RIGHT B9
// Key [1, 0] (left half, leftmost key on the second row from top) - set as the left hand
#define HANDMAGIC_OUTPUT_PIN_LEFT B10
#define HANDMAGIC_INPUT_PIN_LEFT B2
// Key [6, 5] (right half, leftmost key on the second row from top) - set as the right hand
#define HANDMAGIC_OUTPUT_PIN_RIGHT B3
#define HANDMAGIC_INPUT_PIN_RIGHT A6
