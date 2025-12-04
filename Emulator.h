
#ifndef EMULATOR_H
#define EMULATOR_H

#include "Data.h"
#include "BaseAgent.h"
#include "Agent.h"
#include <iostream>

class Emulator {
public:
  BaseAgent* zero;
  Emulator(BaseAgent* zero);
  void AddAgent(BaseAgent* agent);
  void AddAgent(int num_agent);
  void AddAutoAgent(int num_agent);
  void DelAgent(int num_agent);
  BaseAgent* Find(int num_agent);
  void Run();
  void Print();
};

#endif