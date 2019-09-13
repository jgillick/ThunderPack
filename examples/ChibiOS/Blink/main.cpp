/*
  Continuously blinks the LED 1 (PA0). When the user button
  is pressed, the LED blinks faster.
*/
#include "ch.hpp"
#include "hal.h"

#define SPEED_SLOW 800
#define SPEED_FAST 200

int main(void) {
  // System initializations.
  halInit();
  chSysInit();

  // Set LED mode to digital output
  palSetLineMode(LINE_LED_1, PAL_MODE_OUTPUT_PUSHPULL);

  // Main loop
  int blinkSpeed = SPEED_SLOW;
  while (1) {
    palToggleLine(LINE_LED_1);

    // Blink faster while the button has been pressed
    unsigned int btnVal = palReadLine(LINE_USER_BTN);
    if (btnVal) {
      blinkSpeed = SPEED_FAST;
    } else {
      blinkSpeed = SPEED_SLOW;
    }
    chThdSleepMilliseconds(blinkSpeed);
  }
}
