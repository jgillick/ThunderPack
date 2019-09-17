/*
  A simple program which demonstrates outputting serial data over USB. 
  In this case it simply outputs the state (0/1) of the button, repeatedly,
  to the serial console. 
  
  LED 1 also illuminates when the USB device has connected to the machine.
*/

#include <stdio.h>
#include <string.h>

#include "ch.h"
#include "hal.h"

#include "chprintf.h"
#include "usbcfg.h"

BaseSequentialStream* sdStream = (BaseSequentialStream*)&SDU1;

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

  uint8_t btnVal;
  while (1) {
    // USB is connected
    if (SDU1.config->usbp->state == USB_ACTIVE) {
      palSetLine(LINE_LED_1);

      // Print button status to the terminal
      btnVal = palReadLine(LINE_USER_BTN);
      chprintf(sdStream, "%d\r\n", btnVal);
    } else {
      palClearLine(LINE_LED_1);
    }
    chThdSleepMilliseconds(100);
  }
}
