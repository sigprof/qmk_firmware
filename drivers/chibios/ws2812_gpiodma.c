/* Copyright 2021 Sergey Vlasov <sigprof@gmail.com>
 *
 * Derived from drivers/chibios/ws2812_pwm.c, which did not have an
 * explicit copyright notice.
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
#include "ws2812.h"
#include "quantum.h"
#include <hal.h>

/* Adapted from https://github.com/joewa/WS2812-LED-Driver_ChibiOS/ */

#ifndef WS2812_GPIODMA_PWM_DRIVER
#    if defined(STM32F103xB)
#        define WS2812_GPIODMA_PWM_DRIVER PWMD4
#        define WS2812_GPIODMA_START_PWM_CHANNEL 1
#        define WS2812_GPIODMA_START_DMA_STREAM STM32_DMA1_STREAM1
//#        define WS2812_GPIODMA_START_PWM_CHANNEL 0
//#        define WS2812_GPIODMA_START_DMA_STREAM STM32_DMA1_STREAM7
#        define WS2812_GPIODMA_T0H_PWM_CHANNEL 2
#        define WS2812_GPIODMA_T0H_DMA_STREAM STM32_DMA1_STREAM4
#        define WS2812_GPIODMA_T1H_PWM_CHANNEL 3
#        define WS2812_GPIODMA_T1H_DMA_STREAM STM32_DMA1_STREAM5
#    elif defined(STM32F303xC)
#        define WS2812_GPIODMA_PWM_DRIVER PWMD4
#        define WS2812_GPIODMA_START_PWM_CHANNEL 1
#        define WS2812_GPIODMA_START_DMA_STREAM STM32_DMA1_STREAM1
#        define WS2812_GPIODMA_T0H_PWM_CHANNEL 2
#        define WS2812_GPIODMA_T0H_DMA_STREAM STM32_DMA1_STREAM4
#        define WS2812_GPIODMA_T1H_PWM_CHANNEL 3
#        define WS2812_GPIODMA_T1H_DMA_STREAM STM32_DMA1_STREAM5
#    elif defined(STM32F401xx) || defined(STM32F411xx)
#        define WS2812_GPIODMA_PWM_DRIVER PWMD1
#        define WS2812_GPIODMA_START_PWM_CHANNEL 1
#        define WS2812_GPIODMA_START_DMA_STREAM STM32_DMA2_STREAM3
#        define WS2812_GPIODMA_START_DMA_CHANNEL 6
#        define WS2812_GPIODMA_T0H_PWM_CHANNEL 2
#        define WS2812_GPIODMA_T0H_DMA_STREAM STM32_DMA2_STREAM2
#        define WS2812_GPIODMA_T0H_DMA_CHANNEL 6
#        define WS2812_GPIODMA_T1H_PWM_CHANNEL 3
#        define WS2812_GPIODMA_T1H_DMA_STREAM STM32_DMA2_STREAM6
#        define WS2812_GPIODMA_T1H_DMA_CHANNEL 6
#        define WS2812_GPIODMA_PERIPHERAL_TO_MEMORY
#    else
#        error "WS2812 GPIODMA driver configuration missing"
#    endif
#endif

// Push Pull or Open Drain Configuration
// Default Push Pull
#ifndef WS2812_EXTERNAL_PULLUP
#    define WS2812_OUTPUT_MODE PAL_MODE_OUTPUT_PUSHPULL
#else
#    define WS2812_OUTPUT_MODE PAL_MODE_OUTPUT_OPENDRAIN
#endif

#ifndef WS2812_BAUD_RATE
#    define WS2812_BAUD_RATE 800000  // 800 kbps - bit time is 1250 ns
#endif

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */

// Color byte positions inside the LED data
#if (WS2812_BYTE_ORDER == WS2812_BYTE_ORDER_GRB)
#    define GREEN_POS 0
#    define RED_POS 1
#    define BLUE_POS 2
#elif (WS2812_BYTE_ORDER == WS2812_BYTE_ORDER_RGB)
#    define RED_POS 0
#    define GREEN_POS 1
#    define BLUE_POS 2
#elif (WS2812_BYTE_ORDER == WS2812_BYTE_ORDER_BGR)
#    define BLUE_POS 0
#    define GREEN_POS 1
#    define RED_POS 2
#endif

#ifdef RGBW
#    define WHITE_POS 3
#    define WS2812_BITS_PER_LED 32
#else
#    define WS2812_BITS_PER_LED 24
#endif

#define WS2812_PWM_FREQUENCY (STM32_SYSCLK / 2)                     /**< Clock frequency of PWM, must be valid with respect to system clock! */
#define WS2812_PWM_PERIOD (WS2812_PWM_FREQUENCY / WS2812_BAUD_RATE) /**< Clock period in ticks. 1 / 800kHz = 1.25 uS (as per datasheet) */
#define WS2812_BIT_TIME_NS (1000000000 / WS2812_BAUD_RATE)

/**
 * @brief   Number of extra bit-periods at the start of transmission
 *
 * Because the timer generates the first update DMA request only after
 * the first period has passed, that first period needs to be skipped,
 * but the space for it must be reserved in the data buffer.
 */
#define WS2812_PREAMBLE_BIT_N 1

/**
 * @brief   Number of bit-periods actually used for LED data
 */
#define WS2812_COLOR_BIT_N (RGBLED_NUM * WS2812_BITS_PER_LED)

/**
 * @brief   Number of bit-periods to hold the data line low at the end of a frame
 *
 * The reset period for each frame is defined in WS2812_TRST_US, and the
 * bit time is calculated from WS2812_BAUD_RATE.
 */
#define WS2812_RESET_BIT_N (1000 * WS2812_TRST_US / WS2812_BIT_TIME_NS)

/**
 * @brief   Number of bits in the data buffer
 *
 * The data buffer needs to hold only the preamble and actual color
 * bits; during the reset period data bits are no longer sent.
 */
#define WS2812_DATA_BIT_N (WS2812_PREAMBLE_BIT_N + WS2812_COLOR_BIT_N)

/**
 * @brief   Total number of bit-periods in a frame
 */
#define WS2812_TOTAL_BIT_N (WS2812_PREAMBLE_BIT_N + WS2812_COLOR_BIT_N + WS2812_RESET_BIT_N)

/**
 * @brief   High period for a zero, in ticks
 *
 * Per the datasheet:
 * WS2812:
 * - T0H: 200 nS to 500 nS, inclusive
 * - T0L: 650 nS to 950 nS, inclusive
 * WS2812B:
 * - T0H: 200 nS to 500 nS, inclusive
 * - T0L: 750 nS to 1050 nS, inclusive
 *
 * The duty cycle is calculated for a high period of 350 nS.
 */
#define WS2812_DUTYCYCLE_0 (WS2812_PWM_FREQUENCY / (1000000000 / 350))

/**
 * @brief   High period for a one, in ticks
 *
 * Per the datasheet:
 * WS2812:
 * - T1H: 550 nS to 850 nS, inclusive
 * - T1L: 450 nS to 750 nS, inclusive
 * WS2812B:
 * - T1H: 750 nS to 1050 nS, inclusive
 * - T1L: 200 nS to 500 nS, inclusive
 *
 * The duty cycle is calculated for a high period of 800 nS.
 * This is in the middle of the specifications of the WS2812 and WS2812B.
 */
#define WS2812_DUTYCYCLE_1 (WS2812_PWM_FREQUENCY / (1000000000 / 800))

/**
 * @brief   HAL port object for the GPIO port which contains the DI pin
 */
#define WS2812_DI_PORT PAL_PORT(RGB_DI_PIN)

/**
 * @brief   Bit mask for the DI pin in the GPIO port
 */
#define WS2812_DI_BIT_MASK (1U << PAL_PAD(RGB_DI_PIN))

#ifdef USE_GPIOV1

/**
 * @brief Bit Reset Register for the GPIO port which contains the DI pin
 *
 * For GPIOv1 devices this is a 32-bit register.
 */
#    define WS2812_DI_BRR (WS2812_DI_PORT->BRR)

/**
 * @brief   DMA peripheral size flag to be used with WS2812_DI_BRR
 *
 * In GPIOv1 devices all GPIO registers, including BRR, must be accessed as 32-bit words.  Because all existing GPIOv1
 * devices also use DMAv1, specifying different sizes for peripheral and memory data is allowed, and the 16-bit data
 * from memory would be zero extended to 32 bits by the DMA controller.
 */
#    define BRR_DMA_PSIZE STM32_DMA_CR_PSIZE_WORD

#    define BRR_DMA_SIZE WORD

#else

/**
 * @brief Bit Reset Register for the GPIO port which contains the DI pin
 *
 * GPIOv2 devices do not have BRR as a separate register, but the high 16-bit word of BSRR could be used for that
 * purpose (unlike GPIOv1, these newer devices support 16-bit and even 8-bit access to GPIO registers).  The same high
 * 16-bit word of BSRR is used for GPIOv3 devices (the ChibiOS GPIO HAL does this too).
 */
#    define WS2812_DI_BRR (WS2812_DI_PORT->BSRR.H.clear)

/**
 * @brief   DMA peripheral size flag to be used with WS2812_DI_BRR
 *
 * For GPIOv2 and GPIOv3 devices the register which is used as a replacement for BRR is 16-bit, and some of these
 * devices also use DMAv2, which requires the peripheral and memory data size to be the same.
 */
#    define BRR_DMA_PSIZE STM32_DMA_CR_PSIZE_HWORD

#    define BRR_DMA_SIZE HWORD

#endif

/* --- PRIVATE MACROS ------------------------------------------------------- */

/**
 * @brief   Determine the index in @ref ws2812_frame_buffer "the frame buffer" of a given bit
 *
 * @param[in] led:                  The led index [0, @ref RGBLED_NUM)
 * @param[in] byte:                 The byte number [0, 2] (or [0, 3] for RGBW)
 * @param[in] bit:                  The bit number [0, 7]
 *
 * @return                          The bit index
 */
#define WS2812_BIT(led, byte, bit) (WS2812_PREAMBLE_BIT_N + WS2812_BITS_PER_LED * (led) + 8 * (byte) + (7 - (bit)))

/**
 * @brief   Determine the index in @ref ws2812_frame_buffer "the frame buffer" of a given red bit
 *
 * @param[in] led:                  The led index [0, @ref RGBLED_NUM)
 * @param[in] bit:                  The bit number [0, 7]
 *
 * @return                          The bit index
 */
#define WS2812_RED_BIT(led, bit) WS2812_BIT((led), RED_POS, (bit))

/**
 * @brief   Determine the index in @ref ws2812_frame_buffer "the frame buffer" of a given green bit
 *
 * @param[in] led:                  The led index [0, @ref RGBLED_NUM)
 * @param[in] bit:                  The bit number [0, 7]
 *
 * @return                          The bit index
 */
#define WS2812_GREEN_BIT(led, bit) WS2812_BIT((led), GREEN_POS, (bit))

/**
 * @brief   Determine the index in @ref ws2812_frame_buffer "the frame buffer" of a given blue bit
 *
 * @param[in] led:                  The led index [0, @ref RGBLED_NUM)
 * @param[in] bit:                  The bit index [0, 7]
 *
 * @return                          The bit index
 */
#define WS2812_BLUE_BIT(led, bit) WS2812_BIT((led), BLUE_POS, (bit))

/**
 * @brief   Determine the index in @ref ws2812_frame_buffer "the frame buffer" of a given white bit
 *
 * @param[in] led:                  The led index [0, @ref RGBLED_NUM)
 * @param[in] bit:                  The bit index [0, 7]
 *
 * @return                          The bit index
 */
#define WS2812_WHITE_BIT(led, bit) WS2812_BIT((led), WHITE_POS, (bit))

/**
 * @brief   DIER timer register bit which enables the DMA request for the compare channel
 */
#define CCxDE(channel) (TIM_DIER_CC1DE << ((channel)-1))

#if WS2812_GPIODMA_START_PWM_CHANNEL > 0

/**
 * @brief Time between the start of the WS2812 bit pulse and the start of the timer cycle in timer ticks.
 *
 * When a separate timer channel is used to generate the DMA request at the start of the WS2812 bit pulse, that channel
 * is actually set up to match on the very last timer tick in the PWM period, so that it would almost match the timer
 * update event (which happens at the first tick of the PWM period, but does not happen for the very first PWM period
 * after starting the timer).  Because the timer compare event happens earlier, the PWM duty values of other channels
 * also need to be adjusted by the same number of ticks to keep the resulting pulse timings the same.
 */
#    define WS2812_START_SHIFT 1

/**
 * @brief DIER timer register bit for the DMA request at the start of the WS2812 bit pulse
 *
 * If a separate timer channel is used to generate the DMA request at the start of the WS2812 bit pulse, the DMA request
 * is enabled by the corresponding CCxDE bits in the DIER register.
 */
#    define DIER_START_DE CCxDE(WS2812_GPIODMA_START_PWM_CHANNEL)

#else

/**
 * @brief Time between the start of the WS2812 bit pulse and the start of the timer cycle in timer ticks.
 *
 * When the timer update event is used to generate the DMA request at the start of the WS2812 bit pulse, the pulse
 * starts exactly at the start of the PWM period, therefore PWM duty values for other channels do not require additional
 * adjustment.
 */
#    define WS2812_START_SHIFT 0

/**
 * @brief DIER timer register bit for the DMA request at the start of the WS2812 bit pulse
 *
 * The timer update event could be used to generate the DMA request at the start of the WS2812 bit pulse without
 * requiring a separate compare channel.  In this case the bit which enables the DMA request generation is the UDE bit
 * in the DIER register.
 */
#    define DIER_START_DE TIM_DIER_UDE

#endif

#if (STM32_DMA_SUPPORTS_DMAMUX == TRUE)
#    define WS2812_DMA_SET_DMAMUX(name) dmaSetRequestSource(name##_DMA_STREAM, name##_DMAMUX_ID)
#else
#    define WS2812_DMA_SET_DMAMUX(name) _Static_assert(true, "")
#endif

#ifdef WS2812_GPIODMA_PERIPHERAL_TO_MEMORY
#    define WS2812_DMA_SET_MEM_ADDR(name, addr) dmaStreamSetPeripheral(name##_DMA_STREAM, (addr))
#    define WS2812_DMA_SET_GPIO_ADDR(name, addr) dmaStreamSetMemory0(name##_DMA_STREAM, (addr))
#    define WS2812_DMA_DIR STM32_DMA_CR_DIR_P2M
#    define WS2812_DMA_MEM_WIDTH(width) STM32_DMA_CR_PSIZE_##width
#    define WS2812_DMA_GPIO_WIDTH(width) STM32_DMA_CR_MSIZE_##width
#    define WS2812_DMA_MEM_INC(inc_flag) ((inc_flag) ? STM32_DMA_CR_PINC : 0)
#else
#    define WS2812_DMA_SET_MEM_ADDR(name, addr) dmaStreamSetMemory0(name##_DMA_STREAM, (addr))
#    define WS2812_DMA_SET_GPIO_ADDR(name, addr) dmaStreamSetPeripheral(name##_DMA_STREAM, (addr))
#    define WS2812_DMA_DIR STM32_DMA_CR_DIR_M2P
#    define WS2812_DMA_MEM_WIDTH(width) STM32_DMA_CR_MSIZE_##width
#    define WS2812_DMA_GPIO_WIDTH(width) STM32_DMA_CR_PSIZE_##width
#    define WS2812_DMA_MEM_INC(inc_flag) ((inc_flag) ? STM32_DMA_CR_MINC : 0)
#endif

// clang-format off
/**
 * @brief   Set up DMA for a memory to GPIO transfer
 *
 * @param[in] name:         The DMA stream base name
 * @param[in] mem_addr:     The source address in memory
 * @param[in] gpio_addr:    The destination address in GPIO port registers
 * @param[in] size:         The transaction size (number of DMA transfers)
 * @param[in] mem_width:    The memory data width [BYTE, HWORD, WORD]
 * @param[in] gpio_width:   The GPIO data width [BYTE, HWORD, WORD]
 * @param[in] mem_inc:      True if the DMA memory address needs to be incremented after each data transfer
 * @param[in] mode:         Extra DMA mode flags
 */
#define WS2812_DMA_SETUP(name, mem_addr, gpio_addr, size, mem_width, gpio_width, mem_inc, mode)     \
    do {                                                                                            \
        WS2812_DMA_SET_MEM_ADDR(name, (mem_addr));                                                  \
        WS2812_DMA_SET_GPIO_ADDR(name, (gpio_addr));                                                \
        dmaStreamSetTransactionSize(name##_DMA_STREAM, (size));                                     \
        dmaStreamSetMode(name##_DMA_STREAM,                                                         \
                         ( (mode) | STM32_DMA_CR_CHSEL(name##_DMA_CHANNEL) | STM32_DMA_CR_PL(3)     \
                           | WS2812_DMA_MEM_WIDTH(mem_width) | WS2812_DMA_GPIO_WIDTH(gpio_width)    \
                           | WS2812_DMA_DIR | WS2812_DMA_MEM_INC(mem_inc) ));                       \
        WS2812_DMA_SET_DMAMUX(name);                                                                \
    } while (0)
// clang-format on

/* --- PRIVATE VARIABLES ---------------------------------------------------- */

static uint32_t ws2812_tstart_bsrr = WS2812_DI_BIT_MASK; /**< Value to be transferred to BSRR at the start of the bit time */
static uint16_t ws2812_t1h_brr     = WS2812_DI_BIT_MASK; /**< Value to be transferred to BRR at the end of T1H */
static uint16_t ws2812_frame_buffer[WS2812_DATA_BIT_N];  /**< Buffer for a frame */

/**
 * @brief   Set if the LED data needs to be sent again, because it was changed while another send was in progress.
 *
 * This variable may be accessed only in the I-Locked or S-Locked state.
 */
static bool ws2812_update_required = false;

/**
 * @brief   Set while the LED data is being sent (the timer was started, and DMA has not completed yet).
 *
 * This variable may be accessed only in the I-Locked or S-Locked state.
 */
static bool ws2812_busy = false;

/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */

static void ws2812_t1h_dma_complete(void *context, uint32_t flags);

/**
 * @brief   Start sending the WS2812 bitstream.
 *
 * Assumes that ws2812_init() has been called previously, and the timer counter has been disabled (the PWM API provided
 * by the ChibiOS HAL does not expose the counter enable/disable operation, therefore direct register access is used).
 *
 * This function is called in the I-Locked or S-Locked state, therefore only I-Class APIs may be used.
 */
static void ws2812_start_send(void) {
    // Save the DMA/interrupt enable register state which was configured by pwmStart().
    uint32_t dier = WS2812_GPIODMA_PWM_DRIVER.tim->DIER;

    // Disable all DMA requests from the timer.  This is required to reset pending DMA requests, otherwise they will be
    // serviced immediately after enabling the DMA streams, breaking the proper order of operations.  See AN4031 Rev.3,
    // section 4.1 “Software sequence to disable DMA”; the DMA stream is already disabled at this point.
    WS2812_GPIODMA_PWM_DRIVER.tim->DIER = dier & ~(STM32_TIM_DIER_UDE | STM32_TIM_DIER_CC1DE | STM32_TIM_DIER_CC2DE | STM32_TIM_DIER_CC3DE | STM32_TIM_DIER_CC4DE);

    // Set up DMA for the start of the bit pulse.  This DMA transfer always sets the DI pin to 1, and the count is
    // limited to the number of color bits (preamble is not included, because the update event which usually triggers
    // this transfer is generated only after the first PWM period is finished).
    WS2812_DMA_SETUP(WS2812_GPIODMA_START, &ws2812_tstart_bsrr, &(WS2812_DI_PORT->BSRR), WS2812_COLOR_BIT_N, WORD, WORD, false, 0);

    // Set up DMA for T0H.  This DMA transfer sets the DI pin to 0 (creating a short pulse) if the currently transferred
    // bit should be 0.  The count includes an extra element for the preamble (it should set the DI pin to 0).  On chips
    // with DMAv1 the data sizes for memory and peripheral may not match (the memory data size is always 16 bit, and the
    // peripheral data size is required to be 32 bit for GPIOv1).
    WS2812_DMA_SETUP(WS2812_GPIODMA_T0H, ws2812_frame_buffer, &WS2812_DI_BRR, WS2812_DATA_BIT_N, HWORD, BRR_DMA_SIZE, true, 0);

    // Set up DMA for T1H.  This DMA transfer always sets the DI pin to 0, finishing the long pulse if the currently
    // transferred bit should be 1, or doing nothing if either the currently transferred bit should be 0, or during the
    // reset pulse.  The count is set according to the total number of bit times including the reset pulse.  The same
    // data sizes as for T0H are used.
    WS2812_DMA_SETUP(WS2812_GPIODMA_T1H, &ws2812_t1h_brr, &WS2812_DI_BRR, WS2812_TOTAL_BIT_N, HWORD, BRR_DMA_SIZE, false, STM32_DMA_CR_TCIE);

    // Start all DMA streams (the timer counter is assumed to be disabled at this time).
    dmaStreamEnable(WS2812_GPIODMA_START_DMA_STREAM);
    dmaStreamEnable(WS2812_GPIODMA_T0H_DMA_STREAM);
    dmaStreamEnable(WS2812_GPIODMA_T1H_DMA_STREAM);

    // Enable DMA requests from the timer again.  This must be done after the DMA streams are enabled; see AN4031 Rev.3,
    // section 4.3 “Software sequence to enable DMA”.
    WS2812_GPIODMA_PWM_DRIVER.tim->DIER = dier;

    // Finally, enable the timer counter.
    WS2812_GPIODMA_PWM_DRIVER.tim->CR1 |= STM32_TIM_CR1_CEN;
}

/**
 * @brief   DMA completion callback for T1H.
 *
 * This function is called from an interrupt handler.
 */
static void ws2812_t1h_dma_complete(void *context, uint32_t flags) {
    (void)context;
    (void)flags;

    chSysLockFromISR();

    // Disable the timer counter.
    WS2812_GPIODMA_PWM_DRIVER.tim->CR1 &= ~STM32_TIM_CR1_CEN;

    // Generate the update event to reset the timer counter to its initial value (in preparation for another send).
    WS2812_GPIODMA_PWM_DRIVER.tim->EGR = STM32_TIM_EGR_UG;

    // Stop all DMA streams.
    dmaStreamDisable(WS2812_GPIODMA_START_DMA_STREAM);
    dmaStreamDisable(WS2812_GPIODMA_T0H_DMA_STREAM);
    dmaStreamDisable(WS2812_GPIODMA_T1H_DMA_STREAM);

    // Check whether the buffer was updated while the DMA was still in progress.
    if (ws2812_update_required) {
        // The buffer was updated - need to send the data again.
        ws2812_update_required = false;
        ws2812_start_send();
    } else {
        // No need to send the data again until further updates.
        ws2812_busy = false;
    }

    chSysUnlockFromISR();
}

void ws2812_write_led(uint16_t led_number, LED_TYPE led) {
    // Write color to frame buffer
    for (uint8_t bit = 0; bit < 8; bit++) {
        ws2812_frame_buffer[WS2812_RED_BIT(led_number, bit)]   = ((led.r >> bit) & 0x01) ? 0 : WS2812_DI_BIT_MASK;
        ws2812_frame_buffer[WS2812_GREEN_BIT(led_number, bit)] = ((led.g >> bit) & 0x01) ? 0 : WS2812_DI_BIT_MASK;
        ws2812_frame_buffer[WS2812_BLUE_BIT(led_number, bit)]  = ((led.b >> bit) & 0x01) ? 0 : WS2812_DI_BIT_MASK;
#ifdef RGBW
        ws2812_frame_buffer[WS2812_WHITE_BIT(led_number, bit)] = ((led.w >> bit) & 0x01) ? 0 : WS2812_DI_BIT_MASK;
#endif
    }
}

/**
 * @brief   Initialize the WS2812 driver
 */
static void ws2812_init(void) {
    // Initialize LED frame buffer.
    uint32_t i;
    for (i = 0; i < WS2812_DATA_BIT_N; i++) {
        ws2812_frame_buffer[i] = WS2812_DI_BIT_MASK;  // All preamble and color bits are zero
    }

    // Set up the DI pin.
    palSetLineMode(RGB_DI_PIN, WS2812_OUTPUT_MODE);
    palClearLine(RGB_DI_PIN);

    // Allocate all DMA streams (this must be done only once).
    dmaStreamAlloc(WS2812_GPIODMA_START_DMA_STREAM - STM32_DMA_STREAM(0), 10, NULL, NULL);
    dmaStreamAlloc(WS2812_GPIODMA_T0H_DMA_STREAM - STM32_DMA_STREAM(0), 10, NULL, NULL);
    dmaStreamAlloc(WS2812_GPIODMA_T1H_DMA_STREAM - STM32_DMA_STREAM(0), 10, ws2812_t1h_dma_complete, NULL);

    // Configure and start the PWM timer.
    static const PWMConfig ws2812_pwm_config = {
        .frequency = WS2812_PWM_FREQUENCY,
        .period    = WS2812_PWM_PERIOD,
        .callback  = NULL,
        .channels =
            {
                // Disable all channels (PWM outputs are not actually used).
                [0 ... PWM_CHANNELS - 1] = {.mode = PWM_OUTPUT_DISABLED, .callback = NULL},
            },
        .cr2  = 0,
        .dier = DIER_START_DE | CCxDE(WS2812_GPIODMA_T0H_PWM_CHANNEL) | CCxDE(WS2812_GPIODMA_T1H_PWM_CHANNEL),
    };
    pwmStart(&WS2812_GPIODMA_PWM_DRIVER, &ws2812_pwm_config);

    // pwmStart() enables the timer counter, but does not provide an option to set the PWM duty values before doing
    // that.  Setting the duty values after starting the timer would cause them to take effect only on the next timer
    // cycle (because pwmStart() sets the OCxPE bits in CCMRx registers), and the initialization with the timer already
    // running would be racing against the timer, which just does not look right.  So it looks like the only way to
    // start the timer properly is to manipulate some bits directly behind the driver's back.

    // Disable the timer counter.
    WS2812_GPIODMA_PWM_DRIVER.tim->CR1 &= ~STM32_TIM_CR1_CEN;

    // Set the duty values for all used channels.
    pwmEnableChannel(&WS2812_GPIODMA_PWM_DRIVER, WS2812_GPIODMA_T0H_PWM_CHANNEL - 1, WS2812_DUTYCYCLE_0 - WS2812_START_SHIFT);
    pwmEnableChannel(&WS2812_GPIODMA_PWM_DRIVER, WS2812_GPIODMA_T1H_PWM_CHANNEL - 1, WS2812_DUTYCYCLE_1 - WS2812_START_SHIFT);
#if WS2812_GPIODMA_START_PWM_CHANNEL > 0
    pwmEnableChannel(&WS2812_GPIODMA_PWM_DRIVER, WS2812_GPIODMA_START_PWM_CHANNEL - 1, WS2812_PWM_PERIOD - WS2812_START_SHIFT);
#endif

    // Force the update event, so that the duty values would be applied, and the counter would be reset.
    WS2812_GPIODMA_PWM_DRIVER.tim->EGR = STM32_TIM_EGR_UG;

    // Now the timer is ready to be started, and all initialization actions that require the normal state (e.g.,
    // pwmStart()) have been done.
}

/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

/**
 * @brief   Send the LED data to the WS2812 LEDs.
 *
 * This function is a part of the public WS2812 driver API.
 */
void ws2812_setleds(LED_TYPE *ledarray, uint16_t leds) {
    // Perform driver initialization on the first call.
    static bool s_init = false;
    if (!s_init) {
        ws2812_init();
        s_init = true;
    }

    // Convert the LED data into the form which is required for sending the bitstream using DMA.
    // TODO: Currently this conversion can update the frame buffer while its contents are being sent over DMA, which
    // could potentially cause flickering.  However, solving this problem would require either a second frame buffer, or
    // a smaller LED data buffer together with a timer callback with strict latency requirements to convert the data on
    // the fly, so the potential problem is ignored for now.
    for (uint16_t i = 0; i < leds; i++) {
        ws2812_write_led(i, ledarray[i]);
    }

    // Enter the S-Locked state to use variables which are shared with the interrupt callback.
    chSysLock();

    if (ws2812_busy) {
        // The DMA transfer is currently in progress.  Set the flag which will cause the completion callback to start
        // another transfer immediately after the completion, so that the new LED state would be transferred.
        ws2812_update_required = true;
    } else {
        // No DMA transfer currently in progress - start it.
        ws2812_busy = true;
        ws2812_start_send();
    }

    // Leave the S-Locked state entered above.
    chSysUnlock();
}
