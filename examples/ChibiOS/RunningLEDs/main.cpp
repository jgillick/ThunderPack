/*
  Make all the LEDs "run" by turning them on in order, and then off again.
*/

#include "ch.hpp"
#include "hal.h"

#define SPEED 150
#define LED_COUNT 4

int leds[] = {
  LINE_LED_1, // LED 1
  LINE_LED_2, // LED 2
  LINE_LED_3, // LED 3
  LINE_LED_4, // LED 4
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
  }

  // Run LEDs
  uint8_t led_idx = 0;
  while (1) {
    // Toggle the current LED
    palToggleLine(leds[led_idx]);
    chThdSleepMilliseconds(SPEED);

    // Increment LED
    led_idx++;
    if (led_idx >= LED_COUNT) {
      led_idx = 0;
    }
  }
}
