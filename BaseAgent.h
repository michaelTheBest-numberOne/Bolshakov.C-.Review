
#ifndef BASEAGENT_H
#define BASEAGENT_H

#include "Data.h"
#include <queue>
#include <unordered_map>

class BaseAgent {
public:
  BaseAgent* next{};
  virtual ~BaseAgent() = default;
  BaseAgent();
  BaseAgent(int num);
  BaseAgent(BaseAgent* prev, int num);
  virtual Data* Process(Data* recv_data) = 0;
  virtual Data* Run() = 0;
  int Num();
  void NewMessage(std::string text, int recepient);
  bool Empty();
protected:
  int num{};
  std::queue<Data*> message_queue{};
  std::unordered_map<int, std::string> recv_message_map{};
};

#endif