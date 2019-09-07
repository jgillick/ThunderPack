/*
  Continuously blinks the first LED (PA0). When the user button
  is pressed, the LED blinks faster.

  Uses virtual timers to blink the LED without blocking the main loop.
  http://www.chibios.org/dokuwiki/doku.php?id=chibios:book:kernel_vt
*/

#include "ch.hpp"
#include "hal.h"

#define MAX_SPEED 1000
#define MIN_SPEED 200

static virtual_timer_t blinkTimer;

/**
 * Toggle the LED value. 
 * This is invoked by the virtual timer.
 */
static void toggleLED(void *arg) {
  palToggleLine(LINE_LED_0);
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
  palSetLineMode(LINE_LED_0, PAL_MODE_OUTPUT_PUSHPULL);

  // Main loop
  int blinkSpeed = MAX_SPEED;
  while (1) {

    // Blink faster while the button has been pressed
    unsigned int btnVal = palReadLine(LINE_USER_BTN);
    if (btnVal) {
      blinkSpeed = MIN_SPEED;
    } else {
      blinkSpeed = MAX_SPEED;
    }

    // Restart the blink timer if it has completed
    if (!chVTIsArmed(&blinkTimer)) {
      chVTSet(&blinkTimer, TIME_MS2I(blinkSpeed), toggleLED, NULL);
    }
    
    chThdSleepMilliseconds(10);
  }
}
