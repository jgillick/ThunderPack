/*
  Continuously blinks LED 1 (PA0). When the user button
  is pressed, the LED blinks faster.

  Uses virtual timers to blink the LED without blocking the main loop.
  http://www.chibios.org/dokuwiki/doku.php?id=chibios:book:kernel_vt
*/

#include "ch.hpp"
#include "hal.h"

#define SPEED_SLOW 800
#define SPEED_FAST 200

static virtual_timer_t blinkTimer;

/**
 * Toggle the LED value. 
 * This is invoked by the virtual timer.
 */
static void toggleLED(void *arg) {
  palToggleLine(LINE_LED_1);
}

/*
 * Application entry point.
 */
int main(void) {
  // System initializations.
  halInit();
  chSysInit();
  chVTObjectInit(&blinkTimer);

  // Set LED mode to digital output
  palSetLineMode(LINE_LED_1, PAL_MODE_OUTPUT_PUSHPULL);

  // Main loop
  int blinkSpeed = SPEED_SLOW;
  while (1) {

    // Blink faster while the button has been pressed
    unsigned int btnVal = palReadLine(LINE_USER_BTN);
    if (btnVal) {
      blinkSpeed = SPEED_FAST;
    } else {
      blinkSpeed = SPEED_SLOW;
    }

    // Restart the blink timer if it has completed
    if (!chVTIsArmed(&blinkTimer)) {
      chVTSet(&blinkTimer, TIME_MS2I(blinkSpeed), toggleLED, NULL);
    }
    
    chThdSleepMilliseconds(10);
  }
}
