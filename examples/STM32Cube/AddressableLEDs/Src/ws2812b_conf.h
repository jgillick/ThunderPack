#ifndef __WS2818B_H
#define __WS2818B_H

/* LEDS -------------------------------------------------------------------*/

/**
 * @brief Enable RGBW (RGB + White) LEDs
 *  - Set to 1 to use RGBW leds.
 *  - Set to 0 to use standard RGB WS2812B leds
 */
#define USE_RGBW 0

/**
 * @brief Automatically update the LEDs when you call one of the methods which sets a color.
 * Otherwise, you'll need to call
 */
#define AUTO_UPDATE 1

/* Data Pin config --------------------------------------------------------*/

/**
 * @brief The GPIO port to output the data to.
 * This needs to be the same pin that maps to the PWM output defined with the timer.
 */
#define DATA_PORT GPIOA

/**
 * @brief The GPIO pin to output the data on.
 * This needs to be the same pin that maps to the PWM output defined with the timer.
 */
#define DATA_PIN GPIO_PIN_7

/**
 * @brief The alternate function to assign to this pin to tie it to the timer output.
 */
#define DATA_PIN_AF GPIO_AF2_TIM3

/* Timer ------------------------------------------------------------------*/

/**
 * @brief The pwm timer instance to use
 */
#define TIMER TIM3

/**
 * @brief The timer channel
 */
#define TIMER_CHANNEL TIM_CHANNEL_2

/**
 * @brief The bit used to enable the clock for this timer.
 * Find it in the relevant stm32xxxx_hal_rcc_ex.h file (i.e. stm32l0xx_hal_rcc_ex.h)
 * and defined with one of the `__HAL_RCC_TIMx_CLK_ENABLE` functions (i.e. `__HAL_RCC_TIM3_CLK_ENABLE`).
 */
#define TIMER_RCC_BIT RCC_APB1ENR_TIM3EN


/* DMA --------------------------------------------------------------------*/

/**
 * @brief The DMA handler instance (i.e. DMA1)
 */
#define DMA DMA1

/**
 * @brief The DMA channel to use.
 */
#define DMA_CHANNEL DMA_CHANNEL_5

/**
 * @brief The DMA stream to use. (if the DMA uses "streams")
 */
#define DMA_STREAM DMA1_Stream5

/**
 * @brief The DMA request to use. (if the chip uses "requests")
 */
// #define DMA_REQUEST DMA_REQUEST_10


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

#endif