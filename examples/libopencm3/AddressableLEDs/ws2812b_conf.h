#ifndef __WS2818B_H
#define __WS2818B_H

/* System -----------------------------------------------------------------*/

/**
 * @brief The speed (Hz) that the MCU is running at.
 * Apparently (as far as I can tell) libopencm3 doesn't have an easy way to get this.
 */
#define CLOCK_SPEED 32000000


/* LEDS -------------------------------------------------------------------*/

/**
 * @brief Enable RGBW (RGB + White) LEDs
 *  - Set to 1 to use RGBW leds.
 *  - Set to 0 to use standard RGB WS2812B leds
 */
#define USE_RGBW 0

/* Data output ------------------------------------------------------------*/

/**
 * @brief The GPIO port to output the data to.
 * This needs to be the same pin that maps to the PWM output defined with the timer.
 */
#define DATA_PORT GPIOA

/**
 * @brief The GPIO pin to output the data on.
 * This needs to be the same pin that maps to the PWM output defined with the timer.
 */
#define DATA_PIN GPIO6

/**
 * @brief The alternate function number to assign to this pin to tie it to the timer output.
 */
#define DATA_PIN_AF GPIO_AF2

/**
 * @brief The RCC enable value for the GPIO port (to be sent to rcc_periph_clock_enable)
 */
#define DATA_PORT_RCC RCC_GPIOA

/* Timer ------------------------------------------------------------------*/

/**
 * @brief The timer number
 */
#define TIMER TIM3

/**
 * @brief The timer channel number
 */
#define TIMER_CHANNEL TIM_OC1

/**
 * @brief The PWM timer driver
 */
#define PWM_DRIVER TIM_OCM_PWM1

/**
 * @brief The RCC enable value for the timer (to be sent to rcc_periph_clock_enable)
 */
#define TIMER_RCC RCC_TIM3

/**
 * @brief The timer CCR register (TIM<TIMER_NUM>_CCR<CHANNEL>)
 */
#define TIMER_CCR TIM3_CCR1

/* DMA --------------------------------------------------------------------*/

/**
 * @brief The DMA controller
 */
#define DMA DMA1

/**
 * @brief The DMA channel to use.
 * (or stream for STM32Fxxx and chips which use DMA "streams")
 */
#define DMA_CHANNEL 3

/**
 * @brief The DMA request to use.
 * (or channel for STM32Fxxx and chips which DMA "streams")
 */
#define DMA_REQUEST 10

/**
 * @brief The DMA interrupt number
 */
#define DMA_IRQ NVIC_DMA1_CHANNEL2_3_IRQ

/**
 * @brief The DMA interrupt function name
 */
#define DMA_ISR_FUNC void dma1_channel2_3_isr(void)

/* LED Protocol -----------------------------------------------------------*/

/**
 * @brief The frequency communication should be set to.
 * WS2812B communicates at 800kHz
 */
#define SIGNAL_HZ 800000

/**
 * @brief The number of PWM pulses needed for the reset signal.
 * The WS2812B needs a 50us reset signal.
 * 0.00005s / (1/800kHz) = 40
 */
#define RESET_PULSES 40

/**
 * @brief The duty cycle multiplier for a binary 1.
 * i.e. .75 = 75% duty
 */
#define BINARY_1_DUTY_MULTIPLIER 0.66

/**
 * @brief The duty cycle multiplier for a binary 0.
 * i.e. .75 = 75% duty
 */
#define BINARY_0_DUTY_MULTIPLIER 0.32

#endif /* #ifndef __WS2818B_H */
