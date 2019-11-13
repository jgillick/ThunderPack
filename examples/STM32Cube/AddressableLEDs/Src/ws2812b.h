/**
 * Implments the WS2812B protocol for STM32 chips using a Timer PWM and the DMA controller.
 * Everything can be configured from the `ws2812b_conf.h` file. This was built & tested with
 * the STM32L072 chip but, in theory, can be configured for any chips in the STM32 family.
 *
 * The implementation is based on this excellent example:
 * http://stm32f4-discovery.net/2018/06/tutorial-control-ws2812b-leds-stm32/
 *
 * Using this library
 * ==================
 * See the `main.c` file for a simple example.
 *
 *  - In your main script, setup the `DMA1_Channel2_3_IRQn` interrupt handler (see below).
 *    If this is not setup, no data will be sent to the LEDs.
 *  - Call the `ws2812b_init` function.
 *  - Call any of the `ws2812b_led*` functions defined in this file to set LED colors.
 *
 * DMA Interrupt
 * =============
 * An interrupt function can only be defined once in a program. Since the DMA controller
 * interrupt functions can be used for multiple DMA channels, it will not be defined by this
 * library.
 *
 * Instead, you'll setup the interrupt in your code with the following few lines:
 *
 * Init:
 *      HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0, 0);
 *      HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
 *
 * Function definition:
 *      void DMA1_Channel2_3_IRQHandler(void) {
 *        ws2812b_interrupt_handler();
 *      }
 */
#include "ws2812b_conf.h"

#ifndef WS2812B_H
#define WS2812B_H

/**
 * @brief  Initialize the ws2823b library and peripherals.
 * @param  num_leds  The number of LEDs in the LED strip.
 */
void ws2812b_init(size_t num_leds);

/**
 * @brief This needs to be called from the correct DMA interrupt handler.
 * By default, your code will need to define the DMA1_Channel2_3_IRQHandler
 * interrupt handler and call this function inside of it.
 */
void ws2812b_interrupt_handler(void);

/**
 * @brief  Set R,G,B color values for an LED
 * @param  index  LED index in array, starting from `0`
 * @param  r      Red color value
 * @param  g      Green color value
 * @param  b      Blue color value
 * @param  [w]    White color value (if USE_RGBW is set to 1)
 */
#if USE_RGBW
void ws2812b_led(size_t index, uint8_t r, uint8_t g, uint8_t b, uint8_t w);
#else
void ws2812b_led(size_t index, uint8_t r, uint8_t g, uint8_t b);
#endif


/**
 * @brief  Same as `ws2812b_led`, but sets this color for ALL LEDs.
 * @param  r      Red color value
 * @param  g      Green color value
 * @param  b      Blue color value
 * @param  [w]    White color value (if USE_RGBW is set to 1)
 */
#if USE_RGBW
void ws2812b_led_all(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
#else
void ws2812b_led_all(uint8_t r, uint8_t g, uint8_t b);
#endif

/**
 * @brief Set the RGB color of 1 LED.
 *
 * To set LED 2 to Red=255, Green=0, Blue=255:
 *   ws2812b_led_rgb(2, 0xFF00FF)
 *
 * @param  index  LED index in array, starting from `0`
 * @param  rgbw   Color value (0x00000000 - 0xRRGGBB[WW])
 */
void ws2812b_led_rgb(size_t index, uint32_t rgb);


/**
 * @brief  Same as `ws2812b_led_rgb`, but sets this color for ALL LEDs.
 * @param  rgbw   Color value (0x00000000 - 0xRRGGBB[WW])
 */
void ws2812b_led_all_rgb(uint32_t rgbw);

#endif