
#include "Data.h"
#include "BaseAgent.h"
#include "Agent.h"
#include "AutoAgent.h"
#include "Emulator.h"
#include <iostream>

Emulator::Emulator(BaseAgent* zero) : zero(zero) {};

void Emulator::AddAgent(BaseAgent* agent) {
  BaseAgent* agent_ = Find(agent->Num());
  if (agent_ != nullptr) {
    std::cout << "There is already an Agent #" << agent->Num() << std::endl;
    return;
  }
  agent->next = zero->next;
  zero->next = agent;
};
void Emulator::AddAgent(int num_agent) {
  Agent* new_agent = new Agent(num_agent);
  AddAgent(new_agent);
};
void Emulator::AddAutoAgent(int num_agent) {
  AutoAgent* new_agent = new AutoAgent(num_agent);
  AddAgent(new_agent);
};
void Emulator::DelAgent(int num_agent) {
  if (num_agent == 0) {
    std::cout << "You can`t delete MainAgent" << std::endl;
    return;
  }
  BaseAgent* prev_del_agent = nullptr;
  for (BaseAgent* current = zero->next; current != zero; current = current->next) {
    if (current->next->Num() == num_agent) {
      prev_del_agent = current;
      break;
    }
  }
  if (zero->next->Num() == num_agent) {
    prev_del_agent = zero;
  }
  if (prev_del_agent == nullptr) {
    std::cout << "There is no agent #" << num_agent << std::endl;
    return;
  }
  BaseAgent* del_agent = prev_del_agent->next;
  prev_del_agent->next = del_agent->next;
  del_agent->next = nullptr;
  delete del_agent;
};
BaseAgent* Emulator::Find(int num_agent) {
  if (num_agent == 0) {
    return zero;
  }
  for (BaseAgent* current = zero->next; current != zero; current = current->next) {
    if (current->Num() == num_agent) {
      return current;
    }
  }
  return nullptr;
};
void Emulator::Run() {
  BaseAgent* current = zero;
  Data* data = new Token();
  while (zero != nullptr) {
    // std::cout << "current: " << current->Num() << std::endl;
    data = current->Process(data);
    current = current->next;
  }
};
void Emulator::Print() {
  std::cout << zero->Num() << " -> ";
  for (BaseAgent* current = zero->next; current != zero; current = current->next) {
    std::cout << current->Num();
    if (typeid(*current) == typeid(AutoAgent)) {
      std::cout << "*";
    }
    std::cout << " -> ";
  }
  std::cout << "0 " << std::endl;
}