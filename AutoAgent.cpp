#include "Data.h"
#include "BaseAgent.h"
#include "AutoAgent.h"
#include <stack>


AutoAgent::~AutoAgent() { next = nullptr; };
AutoAgent::AutoAgent() : BaseAgent() {};
AutoAgent::AutoAgent(int num) : BaseAgent(num) {};
AutoAgent::AutoAgent(BaseAgent* prev, int num) : BaseAgent(prev, num) {};

std::string AutoAgent::Reverse(std::string text) {
  std::stack<std::string> word_stack;
  std::string current = "";
  for (size_t i = 0; i <= text.length(); i++) {
    if (i < text.length() && text[i] != ' ') {
      current += text[i];
    }
    else{
      word_stack.push(current);
      current = "";
    }
  }
  std::string result = "";
  while (!word_stack.empty()) {
    result += word_stack.top();
    word_stack.pop();
    if (!word_stack.empty()) {
      result += " ";
    }
  }
  return result;
};
Data* AutoAgent::Run() {
  if (message_queue.empty()) {
    return new Token();
  }
  Data* front = message_queue.front();
  message_queue.pop();
  return front;
};
Data* AutoAgent::Process(Data* recv_data) {
  if (recv_data->data_type == DataType::kToken) {
    delete recv_data;
    return Run();
  }
  if (recv_data->data_type == DataType::kMessage) {
    Message* cur_data = dynamic_cast<Message*>(recv_data);
    if (cur_data->GetSender() == Num()) {
      delete recv_data;
      return new Token();
    } else if (cur_data->GetRecipient() == Num() || cur_data->GetRecipient() == -1) {
      std::string text = cur_data->GetText();
      int key = cur_data->GetSender();
      recv_message_map[key] += text;
      if (text[text.size() - 1] == '\0') {
        std::cout << "#" << key << " recv #" << Num() << ": " << recv_message_map[key] << std::endl;
        recv_message_map[key].pop_back();
        NewMessage(Reverse(recv_message_map[key]), key);
        recv_message_map[key] = "";
      }
    }
    return recv_data;
  }
  throw "invalid data received";
};