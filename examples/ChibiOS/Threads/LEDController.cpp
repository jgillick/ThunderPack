/*
 Thread which controls the lighting programs for a single LED.
*/

#include <math.h>
#include "ch.h"
#include "ch.hpp"
#include "hal.h"
#include "./LEDController.h"
#include "./ProgramController.h"

#define SLOW      20
#define NORMAL    10
#define FAST      5
#define VERY_FAST 2

#define PROGRAM_CHANGE_EVT EVENT_MASK(1)

// Pre-calculated steps for smooth PWM fading
// https://diarmuid.ie/blog/pwm-exponential-led-fading-on-arduino-or-other-platforms/
static const unsigned int pwm_steps[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 4, 4,
  5, 5, 6, 7, 8, 9, 9, 11, 12, 13, 14, 16, 18, 19, 21, 24, 26,
  29, 32, 35, 38, 42, 46, 51, 56, 62, 68, 74, 82, 90, 99, 108,
  119, 130, 143, 157, 172, 189, 207, 228, 250, 274, 300, 330,
  362, 397, 435, 477, 523, 574, 629, 690, 757, 830, 911, 999,
  1095, 1201, 1317, 1444, 1583, 1736, 1904, 2088, 2289, 2510,
  2753, 3018, 3310, 3629, 3980, 4364, 4785, 5247, 5753, 6308,
  6917, 7584, 8316, 9119, 10000
};

LEDController::LEDController(ProgramController *progCtrl, PWMDriver *pwmp, pwmchannel_t channel)
: progCtrl(progCtrl),
  pwmDrvr(pwmp),
  pwmChannel(channel)
{ }

void LEDController::main(void) {
  setName("led controller");

  // Subscribe to program change event.
  chEvtRegisterMask(&progCtrl->evtSrc, &programChangeListener, PROGRAM_CHANGE_EVT);

  // Starts the PWM channel 0 using 100% duty cycle.
  pwmEnableChannel(pwmDrvr, pwmChannel, PWM_PERCENTAGE_TO_WIDTH(pwmDrvr, 10000));

  while (1) {
    runningProgram = progCtrl->currentProgram;

    if (runningProgram == PROG_FADE_SLOW) {
      fade(SLOW, 0, 100);
      fade(SLOW, 100, 0);
    } else if (runningProgram == PROG_FADE_FAST) {
      fade(NORMAL, 0, 100);
      fade(NORMAL, 100, 0);
    } else if (runningProgram == PROG_FADE_BLINK) {
      fadeBlinkProgram();
    } else {
      setPwm(100);
      pause(10);
    }
  }
}

/**
 * Pauses execution for this many milliseconds or until the program
 * change event fires.
 */
void LEDController::pause(unsigned int milliseconds) {
  chEvtWaitOneTimeout(PROGRAM_CHANGE_EVT, TIME_MS2I(milliseconds));
}

/**
 * Returns true if the program has changed since the last program has started.
 */
bool LEDController::didProgramChange() {
  return (runningProgram != progCtrl->currentProgram);
}

/**
 * Fade the LED from -> to a PWM value
 */
void LEDController::fade(uint8_t speed, uint8_t from, uint8_t to) {
  uint8_t dir = (from > to) ? -1 : 1;
  uint8_t i = from;

  while (!didProgramChange()) {
    setPwm(i);
    pause(speed);

    if (i == to) {
      break;
    }
    i += dir;
  }
}

/**
 * Set the PWM value from the fade step (0 - 100).
 */
void LEDController::setPwm(uint8_t step) {
  int pwmVal = pwm_steps[step];
  pwmEnableChannel(pwmDrvr, pwmChannel, PWM_PERCENTAGE_TO_WIDTH(pwmDrvr, pwmVal));
}

/**
 * Fades the LED up, blnks them twice and then fades them back down.
 */
void LEDController::fadeBlinkProgram() {

  // Fade up
  fade(NORMAL, 20, 100);

  // Blink
  pause(200);
  // for (uint8_t i = 0; i < 2; i++) {
    fade(VERY_FAST, 100, 50);
    fade(VERY_FAST, 50, 100);
  // }
  pause(200);

  // Fade down
  fade(NORMAL, 100, 20);
}
