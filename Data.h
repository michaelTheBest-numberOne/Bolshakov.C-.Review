
#ifndef DATA_H
#define DATA_H

#include <string>
#include <iostream>

enum class DataType {
  kToken,
  kMessage
};
class Data {
public:
  const DataType data_type;
  virtual ~Data();
protected:
  explicit Data(DataType dt);
};
class Token : public Data {
public:
  Token();
};

class Message : public Data {
public:
  Message(int sender, int recipient, std::string text);
  void print(int recv_int);
  int GetSender();
  int GetRecipient();
  std::string GetText();

private:
  int sender;
  int recipient;
  std::string text;
};

#endif