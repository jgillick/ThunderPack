/**
 * Changes the LED strip from red to blue to green, by replacing each LED
 * sequentially down the LED strip.
 *
 * Set LED_COUNT to the number of LEDs which are in your strip.
 */

#include "ch.h"
#include "hal.h"

#include "ws2812b_conf.h"
#include "ws2812b.h"

#define LED_COUNT 16

/**
 * @brief Wipe a color across the entire LED string.
 */
void colorWipe(uint8_t r, uint8_t g, uint8_t b) {
  size_t i;
  for (i = 0; i < LED_COUNT; i++) {
    ws2812b_led(i, r, g, b);
    chThdSleepMilliseconds(50);
  }
}


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void) {
  // Initialize the system
  halInit();
  chSysInit();

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
