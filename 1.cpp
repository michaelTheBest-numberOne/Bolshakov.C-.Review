#include <vector>
#include <iostream>
#include <cmath>
#include <stack>
#include <string>
#include <cassert>

std::vector<std::string> parting(std::string s) {
  std::string current = "";
  std::vector<std::string> ans = {};
  for (size_t i = 0; i < s.size(); ++i) {
    if (s[i] == ' ') {
      if(current != ""){
        ans.push_back(current);
        current = "";
      }
      continue;
    }
    current += s[i];
  }
  if (current != "") {
    ans.push_back(current);
    current = "";
  } 
  return ans;
}
float top(std::stack<float>& q) {
  assert(not(q.empty()) and "not enought elements");
  float x = q.top();
  q.pop();
  return x;
}
float calculateParted(std::vector<std::string> s) {
  std::stack<float> q{};
  for (size_t i = 0; i < s.size(); ++i) {
    if (s[i] == "+") {
      float x2 = top(q);
      float x1 = top(q);
      q.push(x1 + x2);
    } else if (s[i] == "-") {
      float x2 = top(q);
      float x1 = top(q);
      q.push(x1 - x2);
    } else if (s[i] == "*") {
      float x2 = top(q);
      float x1 = top(q);
      q.push(x1 * x2);
    } else if (s[i] == "/") {
      float x2 = top(q);
      float x1 = top(q);
      q.push(x1 / x2);
    } else if (s[i] == "atan2") {
      float x2 = top(q);
      float x1 = top(q);
      q.push(std::atan2(x1, x2));
    } else if (s[i] == "pow") {
      float x2 = top(q);
      float x1 = top(q);
      assert(x1 > 0 and "negative base");
      q.push(std::pow(x1, x2));
    } else if (s[i] == "sin") {
      float x = top(q);
      q.push(std::sin(x));
    } else if (s[i] == "cos") {
      float x = top(q);
      q.push(std::cos(x));
    } else if (s[i] == "tg") {
      float x = top(q);
      assert(std::cos(x) != 0 and "tg are inf");
      q.push(std::tan(x));
    } else if (s[i] == "ctg") {
      float x = top(q);
      assert(std::sin(x) != 0 and "ctg are inf");
      q.push(1.0 / std::tan(x));
    } else if (s[i] == "exp") {
      float x = top(q);
      q.push(std::exp(x));
    } else if (s[i] == "log") {
      float x = top(q);
      assert(x > 0 and "log(negative)");
      q.push(std::log(x));
    } else if (s[i] == "sqrt") {
      float x = top(q);
      assert(x >= 0 and "sqrt(negative)");
      q.push(std::sqrt(x));
    } else if (s[i] == "median") {
      float x3 = top(q);
      float x2 = top(q);
      float x1 = top(q);
      q.push(std::max(std::min(x1, x2), std::min(std::max(x1, x2), x3)));
    } else {
      q.push(std::stof(s[i]));
    }
  }
  float ans = top(q);
  assert(q.empty() and "not enought functions");
  return ans;
}
float calculate(std::string s) {
  return calculateParted(parting(s));
}
int main() {
  std::string s1 = "5 2 8 median 10 *";
  assert(calculate(s1) == 50);
  std::string s2 = "1 5 + 5 2 3 4 1 + + * 5 - - -";
  assert(calculate(s2) == 12);
  std::string s3 = "1 5 5 median 0 cos *";
  assert(calculate(s3) == 5);
}