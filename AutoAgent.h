

#ifndef AUTOAGENT_H
#define AUTOAGENT_H

#include "Data.h"
#include "BaseAgent.h"
#include <stack>

class AutoAgent : public BaseAgent {
public:
  ~AutoAgent();
  AutoAgent();
  AutoAgent(int num);
  AutoAgent(BaseAgent* prev, int num);
  Data* Run();
  Data* Process(Data* recv_data);
  std::string Reverse(std::string text);
private:
};

#endif