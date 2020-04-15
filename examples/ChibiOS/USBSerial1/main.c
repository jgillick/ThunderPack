/*
  Demonstrates writing to and reading from the USB serial interface (USB CDC).
  Connect Thunderpack to your computer and then connect a serial console to it.
  Then:
    + Press the thunderpack user button and you should see the console output the button value.
    + Type a number 1 - 4 into the console and you should see that LED toggle either on or off.
*/

#include <stdio.h>
#include <string.h>

#include "ch.h"
#include "hal.h"

#include "chprintf.h"
#include "usbcfg.h"

#define LED_COUNT 4

BaseSequentialStream* sdStream = (BaseSequentialStream*)&SDU1;

int leds[] = {
  LINE_LED1,
  LINE_LED2,
  LINE_LED3,
  LINE_LED4,
};

static THD_WORKING_AREA(usbRxWorkingArea, 128);

/**
 * Handle bytes recieved over USB.
 * This uses Chibios Threads: http://www.chibios.org/dokuwiki/doku.php?id=chibios:howtos:createthread
 */
static THD_FUNCTION(usbRxThread, arg) {
  uint8_t charReceived;
  uint8_t ledNum;
  while (true) {
    // USB is connected
    if (SDU1.config->usbp->state == USB_ACTIVE) {
      // Check for bytes received, and light LED.
      if (streamRead(&SDU1, (uint8_t *)&charReceived, 1) != 0) {
        // convert char to int and light LED
        ledNum = charReceived - '0';
        if (ledNum >= 1 && ledNum <= LED_COUNT) {
          palToggleLine(leds[ledNum - 1]);
          chprintf(sdStream, "Toggle LED: %d\r\n", ledNum);
        } else {
          chprintf(sdStream, "Enter 1 - %d\r\n", LED_COUNT);
        }
      }
    }
  }
}

int main(void) {
  // System initializations.
  halInit();
  chSysInit();

  // Initialize the serial-over-USB CDC driver.
  sduObjectInit(&SDU1);
  sduStart(&SDU1, &serusbcfg);

  // Setup GPIO
  for (uint8_t i = 0; i < LED_COUNT; i++) {
    palSetLineMode(leds[i], PAL_MODE_OUTPUT_PUSHPULL);
    palClearLine(leds[i]);
  }

  // Activates the USB driver and then the USB bus pull-up on D+.
  // Note, a delay is inserted in order to not have to disconnect the cable after a reset.
  usbDisconnectBus(serusbcfg.usbp);
  chThdSleepMilliseconds(1500);
  usbStart(serusbcfg.usbp, &usbcfg);
  usbConnectBus(serusbcfg.usbp);

  // Listen for recieved characters
  (void)chThdCreateStatic(usbRxWorkingArea, sizeof(usbRxWorkingArea), NORMALPRIO, usbRxThread, NULL);

  // Output the button press state
  uint8_t btnVal;
  uint8_t lastBtnVal = 0;
  while (1) {
    // USB is connected
    if (SDU1.config->usbp->state == USB_ACTIVE) {

      // Print button status to the terminal
      btnVal = palReadLine(LINE_BUTTON);
      if (btnVal != lastBtnVal) {
        chprintf(sdStream, "Button value: %d\r\n", btnVal);
      }
      lastBtnVal = btnVal;
    }
    chThdSleepMilliseconds(100);
  }
}
