/*
  Communicates via Serial over USB (CDC) and echos back any character you send it.
  LED 1 (PA0) will light up when a USB connection has been made.
*/

#include <stdio.h>
#include <string.h>


#include "ch.h"
#include "hal.h"

#include "chprintf.h"
#include "usbcfg.h"

BaseSequentialStream* sdStream = (BaseSequentialStream*)&SDU1;

// Ask for input, then return it to the terminal
void serialComm(void) {
  chprintf(sdStream, "Type a character: ");
  int num = streamGet(&SDU1);
  chprintf(sdStream, "\r\nYou typed: %c\r\n", num);
}

int main(void) {
  // System initializations.
  halInit();
  chSysInit();

  // Initialize the serial-over-USB CDC driver.
  sduObjectInit(&SDU1);
  sduStart(&SDU1, &serusbcfg);

  // USB connection indicator
  palSetLineMode(LINE_LED_1, PAL_MODE_OUTPUT_PUSHPULL); 
  palClearLine(LINE_LED_1);

  /*
   * Activates the USB driver and then the USB bus pull-up on D+.
   * Note, a delay is inserted in order to not have to disconnect the cable
   * after a reset.
   */
  usbDisconnectBus(serusbcfg.usbp);
  chThdSleepMilliseconds(1500);
  usbStart(serusbcfg.usbp, &usbcfg);
  usbConnectBus(serusbcfg.usbp);

  uint8_t connected = 0;
  while (1) {
    // USB is connected
    if (SDU1.config->usbp->state == USB_ACTIVE) {
      palSetLine(LINE_LED_1);

      // New connection
      if (!connected) {
        chprintf(sdStream, "USB connection ready!\r\n");
      }
      serialComm();

      connected = 1;
    } else {
      palClearLine(LINE_LED_1);
      connected = 0;
    }
    chThdSleepMilliseconds(10);
  }
}
