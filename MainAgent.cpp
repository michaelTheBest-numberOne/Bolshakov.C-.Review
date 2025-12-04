#include "Data.h"
#include "BaseAgent.h"
#include "Agent.h"
#include "AutoAgent.h"
#include "Emulator.h"
#include "MainAgent.h"
#include <iostream>


MainAgent::~MainAgent() { next = nullptr; };
MainAgent::MainAgent() : BaseAgent() { emulator = nullptr; };
MainAgent::MainAgent(int num) : BaseAgent(num) { emulator = nullptr; };
MainAgent::MainAgent(BaseAgent* prev, int num) : BaseAgent(prev, num) { emulator = nullptr; };

Data* MainAgent::Process(Data* recv_data) {
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
int InputInt(bool& correct, int min = 0) {
  std::string s = "";
  std::cin >> s;
  for (size_t i = 0; i < s.size(); ++i) {
    if (not(s[i] == '-' || ('0' <= s[i] && s[i] <= '9'))) {
      std::cout << "It is not a integer number: " << s << std::endl;
      correct = false;
      return 0;
    }
  }
  int num = std::stoi(s);
  if (num < min) {
    std::cout << "Invalid number: " << num << std::endl;
    correct = false;
  }
  return num;
}
void MainAgent::Input() {
  while (true) {
    std::string pref = "> ";
    std::cout << pref;
    std::string command = "";
    std::cin >> command;
    if (command == "r" || command == "run") {
      return;
    }
    if (command == "a" || command == "add") {
      bool correct = true;
      int num = InputInt(correct);
      if (not(correct)) { continue; }
      emulator->AddAgent(num);
      continue;
    }
    if (command == "aa" || command == "aadd") {
      bool correct = true;
      int num = InputInt(correct);
      if (not(correct)) { continue; }
      emulator->AddAutoAgent(num);
      continue;
    }
    if (command == "d" || command == "del") {
      bool correct = true;
      int num = InputInt(correct);
      if (not(correct)) { continue; }
      emulator->DelAgent(num);
      continue;
    }
    if (command == "s" || command == "send") {
      bool correct1 = true;
      int sender = InputInt(correct1);
      bool correct2 = true;
      int recepient = InputInt(correct2, -1);
      std::string text = "";
      std::cin.ignore();
      std::getline(std::cin, text);
      if (not(correct1) || not(correct2)) { continue; }
      BaseAgent* sender_agent = emulator->Find(sender);
      if (sender_agent == nullptr) {
        std::cout << "There is no agent #" << sender << std::endl;
        continue;
      }
      if (typeid(*sender_agent) == typeid(AutoAgent)) {
        std::cout << "Agent #" << sender << " is an AutoAnseringAgent" << std::endl;
        continue;
      }
      // std::cout << "send from #" << sender_agent->Num() << std::endl;
      sender_agent->NewMessage(text, recepient);
      continue;
    }
    if (command == "p" || command == "print") {
      emulator->Print();
      continue;
    }
    if (command == "e" || command == "exit") {
      emulator->zero = nullptr;
      return;
    }
    std::cout << "Invalid command: " << command << std::endl;
  }
}
Data* MainAgent::Run() {
  if (message_queue.empty()) {
    for (BaseAgent* current = next; current != this; current = current->next) {
      if (not(current->Empty())) {
        return new Token();
      }
    }
    Input();
    return new Token();
  }
  Data* front = message_queue.front();
  message_queue.pop();
  return front;
}