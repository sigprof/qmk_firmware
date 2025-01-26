// Copyright 2025 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include_next <mcuconf.h>

#undef STM32_I2C_USE_I2C1
#define STM32_I2C_USE_I2C1 TRUE

#undef STM32_SERIAL_USE_USART2
#define STM32_SERIAL_USE_USART2 TRUE

#undef STM32_SPI_USE_SPI2
#define STM32_SPI_USE_SPI2 TRUE
