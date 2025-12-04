#include "Data.h"
#include "BaseAgent.h"
#include <unordered_map>


BaseAgent::BaseAgent() {
  num = 0;
  next = this;
}
BaseAgent::BaseAgent(int num) : next(nullptr), num(num) {};
BaseAgent::BaseAgent(BaseAgent* prev, int num) : num(num) {
  next = prev->next;
  prev->next = this;
};

bool BaseAgent::Empty() { return message_queue.empty(); }
int BaseAgent::Num() { return num; };
void BaseAgent::NewMessage(std::string text, int recepient) {
  size_t max_len = 32;
  for (size_t i = 0; i < text.size(); ++i) {
    if (text[i] == '\0') {
      std::cout << "Invalid message with zero symbol" << std::endl;
      return;
    }
  }
  text.push_back('\0');
  std::string current_s = "";
  for (size_t i = 0; i < text.size(); ++i) {
    char c = text[i];
    current_s.push_back(c);
    if (current_s.size() == max_len || c == '\0') {
      message_queue.push(new Message(num, recepient, current_s));
      current_s = "";
    }
  }
}