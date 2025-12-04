#include "Data.h"


Data::Data(DataType dt) : data_type(dt) {}
Data::~Data() {};

Token::Token() : Data(DataType::kToken) {}

Message::Message(int sender, int recipient, std::string text)
  : Data(DataType::kMessage), sender(sender), recipient(recipient), text(text) {
}
void Message::print(int recv_num) {
  std::cout << "#" << sender << " recv #" << recv_num << ": " << text << std::endl;
}
int Message::GetSender() { return sender; }
int Message::GetRecipient() { return recipient; }
std::string Message::GetText() { return text; }