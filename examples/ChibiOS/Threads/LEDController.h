#include "ch.hpp"
#include "./ProgramController.h"

#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#define PROG_NUM 4

enum program_t {
  PROG_FADE_SLOW,
  PROG_FADE_FAST,
  PROG_FADE_BLINK,
  PROG_ON
};

class LEDController : public chibios_rt::BaseStaticThread<256> {
public:
  LEDController(ProgramController *progCtrl, PWMDriver *pwmp, pwmchannel_t channel);

protected:
  void main(void) override;

private:
  ProgramController *progCtrl;
  event_listener_t programChangeListener;

  uint8_t runningProgram;
  uint8_t fadeStep;
  PWMDriver *pwmDrvr;
  pwmchannel_t pwmChannel;

  void pause(unsigned int milliseconds);
  void setPwm(uint8_t step);
  void fade(uint8_t speed, uint8_t from, uint8_t to);
  bool didProgramChange();

  void fadeProgram();
  void fadeBlinkProgram();
};

#endif
