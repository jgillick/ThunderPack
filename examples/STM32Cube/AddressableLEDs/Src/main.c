/**
 * Changes the LED strip from red to blue to green, by replacing each LED
 * sequentially down the LED strip.
 *
 * Set LED_COUNT to the number of LEDs which are in your strip.
 */
#include <stdio.h>
#include "stm32f4xx_hal.h"

#include "ws2812b_conf.h"
#include "ws2812b.h"
#include "thunderpack.h"

#define LED_COUNT 20

/**
 * @brief Wipe a color across the entire LED string.
 */
void colorWipe(uint8_t r, uint8_t g, uint8_t b) {
  size_t i;
  for (i = 0; i < LED_COUNT; i++) {
    ws2812b_led(i, r, g, b);
    HAL_Delay(50);
  }
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void) {
  HAL_Init();
  thunderpack_clock_init();

  // Setup DMA interrupt for ws2812b
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

  // Initialize the ws2812b lib
  ws2812b_init(LED_COUNT);

  // Start making colors
  while (1) {
    colorWipe(0xFF, 0x00, 0x00); // Red
    colorWipe(0x00, 0xFF, 0x00); // Green
    colorWipe(0x00, 0x00, 0xFF); // Blue
  }

  return 0;
}

/**
 * @brief DMA1 global interrupt
 */
void DMA1_Stream5_IRQHandler(void) {
  ws2812b_interrupt_handler();
}

/* ----------------------------------------------------------*/
/* STM32 Chip System Setup ----------------------------------*/
/* ----------------------------------------------------------*/

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void){
  HAL_IncTick();
}