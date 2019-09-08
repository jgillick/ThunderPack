/*
  Make all the LEDs "run" by turning them on in order, and then off again.
*/

#include "ch.hpp"
#include "hal.h"

/*
 * Application entry point.
 */
int main(void) {
  // System initializations.
  halInit();
  chSysInit();

  // Set LEDs to digital output
  palSetLineMode(LINE_LED_0, PAL_MODE_OUTPUT_PUSHPULL);
  palSetLineMode(LINE_LED_1, PAL_MODE_OUTPUT_PUSHPULL);
  palSetLineMode(LINE_LED_2, PAL_MODE_OUTPUT_PUSHPULL);
  palSetLineMode(LINE_LED_3, PAL_MODE_OUTPUT_PUSHPULL);

  while (1) {
    // Turn LEDs on
    palSetLine(LINE_LED_0);
    chThdSleepMilliseconds(SPEED);
    palSetLine(LINE_LED_1);
    chThdSleepMilliseconds(SPEED);
    palSetLine(LINE_LED_2);
    chThdSleepMilliseconds(SPEED);
    palSetLine(LINE_LED_3);

    // Turn LEDs off
    palClearLine(LINE_LED_0);
    chThdSleepMilliseconds(SPEED);
    palClearLine(LINE_LED_1);
    chThdSleepMilliseconds(SPEED);
    palClearLine(LINE_LED_2);
    chThdSleepMilliseconds(SPEED);
    palClearLine(LINE_LED_3);
  }
}
