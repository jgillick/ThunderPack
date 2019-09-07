/*
  Provides a series of light/fade programs to the first
  LED. 
   - Use the button to switch between programs. 
   - The last program used will be remembered at the next start.
*/

#include "ch.hpp"
#include "hal.h"

#include "./LEDController.h"
#include "./ProgramController.h"

// PWM Config
static PWMConfig pwmcfg = {
  1000000, // 1MHz PWM clock frequency.
  10000,
  NULL,
  {
   {PWM_OUTPUT_ACTIVE_HIGH, NULL},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL}
  },
  0,
  0
};

static event_source_t programEvtSrc;
static ProgramController progCtrl(PROG_NUM);
static LEDController led(&progCtrl, &PWMD2, 0);

/*
 * Application entry point.
 */
int main(void) {
  // System initializations.
  halInit();
  chSysInit();

  // Init event source, for communicating button press to the fading thread
  chEvtObjectInit(&programEvtSrc);

  // Start PWM thread
  pwmStart(&PWMD2, &pwmcfg);

  led.start(NORMALPRIO);

  // Update the program when the button is pressed
  unsigned int lastBtnVal = 0;
  while (1) {
    unsigned int btnVal = palReadLine(LINE_USER_BTN);
    if (btnVal && btnVal != lastBtnVal) {
      progCtrl.incrementProgram();
    }
    lastBtnVal = btnVal;
    chThdSleepMilliseconds(10);
  }
}
