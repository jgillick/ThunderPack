#define USE_HAL_DRIVER

#include "ch.hpp"
#include "hal.h"
#include <stm32l0xx.h>
#include "./ProgramController.h"

#define FLASH_USER_START_ADDR   0x08080000
#define SAVE_PROG_ADDR          FLASH_USER_START_ADDR

ProgramController::ProgramController(int numPrograms) : numPrograms(numPrograms) {
  loadSavedProgram();
  chEvtObjectInit(&evtSrc);
}

void ProgramController::loadSavedProgram() {
  uint8_t val = *(__IO uint8_t*) SAVE_PROG_ADDR;
  if (val >= numPrograms) {
    val = 0;
  }
  currentProgram = val;
}

void ProgramController::saveProgram() {
  HAL_FLASH_Unlock();
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, SAVE_PROG_ADDR, currentProgram);
  HAL_FLASH_Lock();
}

int ProgramController::setProgram(int program) {
  if (program >= 0 && program < numPrograms) {
    currentProgram = program;
    chEvtBroadcast(&evtSrc);
  }
  saveProgram();
  return currentProgram;
}

int ProgramController::incrementProgram() {
  int prog = currentProgram;
  prog++;
  if (prog >= numPrograms) {
    prog = 0;
  }
  return setProgram(prog);
}
