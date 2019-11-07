
#ifndef PROGRAM_CONTROLLER_H
#define PROGRAM_CONTROLLER_H

class ProgramController {
public:
  uint8_t currentProgram = 0;
  uint8_t numPrograms = 0;
  event_source_t evtSrc;
  event_listener_t programChangeListener;

  ProgramController(int numPrograms);
  int incrementProgram();
  int setProgram(int program);
  void saveProgram();
  void loadSavedProgram();
};

#endif