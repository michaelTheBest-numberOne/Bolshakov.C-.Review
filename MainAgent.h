

#ifndef MAINAGENT_H
#define MAINAGENT_H

#include "Data.h"
#include "BaseAgent.h"
#include "Agent.h"
#include "Emulator.h"
#include <iostream>

class MainAgent : public BaseAgent {
public:
  Emulator* emulator;
  ~MainAgent();
  MainAgent();
  MainAgent(int num);
  MainAgent(BaseAgent* prev, int num);
  Data* Process(Data* recv_data);
  void Input();
  Data* Run();
private:
};

#endif