#include "ch.hpp"
#include "hal.h"
#include "./ProgramController.h"
#include "./Memory.cpp"

// Address index to save the program at
#define SAVE_PROG_ADDR    0

ProgramController::ProgramController(int numPrograms) : numPrograms(numPrograms) {
  loadSavedProgram();
  chEvtObjectInit(&evtSrc);
}

void ProgramController::loadSavedProgram() {
  uint8_t val = memory_read(SAVE_PROG_ADDR);
  if (val >= numPrograms) {
    val = 0;
  }
  currentProgram = val;
}

void ProgramController::saveProgram() {
  memory_write(SAVE_PROG_ADDR, currentProgram);
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
