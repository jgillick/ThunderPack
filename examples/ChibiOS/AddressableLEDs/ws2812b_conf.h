#ifndef __WS2818B_H
#define __WS2818B_H


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
#define DATA_PIN 6

/**
 * @brief The alternate function number to assign to this pin to tie it to the timer output.
 */
#define DATA_PIN_AF_NUM 2

/* Timer ------------------------------------------------------------------*/

/**
 * @brief The ChibiOS PWM timer driver (i.e. PWMD3 for TIM3)
 */
#define PWM_DRIVER PWMD3

/**
 * @brief The timer channel number, range: 0 - 3.
 */
#define TIMER_CHANNEL 0

/* DMA --------------------------------------------------------------------*/

/**
 * @brief The DMA controller unit number
 */
#define DMA_NUM 1

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