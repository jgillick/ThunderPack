/*
  Turns on the LED (PA0). When the user button (PB4) is pressed.

  Uses virtual timers to blink the LED without blocking the main loop.
  http://www.chibios.org/dokuwiki/doku.php?id=chibios:book:kernel_vt
*/

#include "ch.hpp"
#include "hal.h"

int main(void) {
  // System initializations.
  halInit();
  chSysInit();

  // Set LED mode to digital output
  palSetLineMode(LINE_LED_0, PAL_MODE_OUTPUT_PUSHPULL);

  // Main loop
  while (1) {
    // Turn LED on when button is pressed
    unsigned int btnVal = palReadLine(LINE_USER_BTN);
    if (btnVal) {
      palSetLine(LINE_LED_0);
    } else {
      palClearLine(LINE_LED_0);
    }
    
    chThdSleepMilliseconds(10);
  }
}
