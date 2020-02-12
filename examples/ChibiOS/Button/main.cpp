/*
  Turns on the LED (PA0). When the user button (PB4) is pressed.
*/

#include "ch.hpp"
#include "hal.h"

int main(void) {
  // System initializations.
  halInit();
  chSysInit();

  // Set LED mode to digital output
  palSetLineMode(LINE_LED1, PAL_MODE_OUTPUT_PUSHPULL);

  // Main loop
  while (1) {
    // Turn LED on when button is pressed
    unsigned int btnVal = palReadLine(LINE_BUTTON);
    if (btnVal) {
      palSetLine(LINE_LED1);
    } else {
      palClearLine(LINE_LED1);
    }

    chThdSleepMilliseconds(10);
  }
}
