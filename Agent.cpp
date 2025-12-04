#include "Data.h"
#include "BaseAgent.h"
#include "Agent.h"

Agent::~Agent() { next = nullptr; };
Agent::Agent() : BaseAgent() {};
Agent::Agent(int num) : BaseAgent(num) {};
Agent::Agent(BaseAgent* prev, int num) : BaseAgent(prev, num) {};

Data* Agent::Run() {
  if (message_queue.empty()) {
    return new Token();
  }
  Data* front = message_queue.front();
  message_queue.pop();
  return front;
};
Data* Agent::Process(Data* recv_data) {
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
        recv_message_map[key] = "";
      }
    }
    return recv_data;
  }
  throw "invalid data received";
};