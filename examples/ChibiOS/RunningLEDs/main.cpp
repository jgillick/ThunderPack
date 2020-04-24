/*
  Make all the LEDs "run" by turning them on in order, and then off again.
*/

#include "ch.hpp"
#include "hal.h"

#define SPEED 150
#define LED_COUNT 4

int leds[] = {
  LINE_LED1,
  LINE_LED2,
  LINE_LED3,
  LINE_LED4,
};

/*
 * Application entry point.
 */
int main(void) {
  // System initializations.
  halInit();
  chSysInit();

  // Set LEDs to digital output
  uint8_t i;
  for (i = 0; i < LED_COUNT; i++) {
    palSetLineMode(leds[i], PAL_MODE_OUTPUT_PUSHPULL);
    palClearLine(leds[i]);
  }

  // Run LEDs
  uint8_t led_idx = 0;
  uint8_t led_state = PAL_HIGH;
  while (1) {
    palWriteLine(leds[led_idx], led_state);

    // Increment LED
    led_idx++;
    if (led_idx >= LED_COUNT) {
      led_idx = 0;
      led_state = !led_state;
    }

    chThdSleepMilliseconds(SPEED);
  }
}
