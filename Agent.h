

#ifndef AGENT_H
#define AGENT_H

#include "Data.h"
#include "BaseAgent.h"

class Agent : public BaseAgent {
public:
  ~Agent();
  Agent();
  Agent(int num);
  Agent(BaseAgent* prev, int num);
  Data* Run();
  Data* Process(Data* recv_data);
private:
};

#endif