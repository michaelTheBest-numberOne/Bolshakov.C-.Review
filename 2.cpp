#include <cassert>
#include <vector>
#include <string>
#include <cmath>
#include <iostream>

class BCD {
private:
  bool negative;
  std::vector<uint8_t> integer;
  std::vector<uint8_t> fractional;

  void RemoveFirstZeros() {
    while (integer.size() > 1 and integer[0] == 0) {
      integer.erase(integer.begin());
    }
  }
  void RemoveLastZeros() {
    while (fractional.size() > 0 and fractional.back() == 0) {
      fractional.pop_back();
    }
  }

  bool FractionalNotZero() const {
    for (uint8_t digit : fractional) {
      if (digit != 0) { return true; }
    }
    return false;
  }

  static std::vector<uint8_t> SumVectors(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b) {
    std::vector<uint8_t> result;
    size_t maxSize = std::max(a.size(), b.size());
    uint8_t carry = 0;

    for (size_t i = 0; (i < maxSize) || carry; ++i) {
      uint8_t digitA = 0;
      uint8_t digitB = 0;
      if (i < a.size()) {
        digitA = a[a.size() - 1 - i];
      }
      if (i < b.size()) {
        digitB = b[b.size() - 1 - i];
      }
      uint8_t sum = digitA + digitB + carry;
      result.push_back(sum % 10);
      carry = sum / 10;
    }

    std::reverse(result.begin(), result.end());
    return result;
  }
  static std::vector<uint8_t> MinusVectors(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b) {
    std::vector<uint8_t> result;
    size_t maxSize = a.size();
    uint8_t borrow = 0;

    for (size_t i = 0; i < maxSize; ++i) {
      uint8_t digitA = a[a.size() - 1 - i];
      uint8_t digitB = 0;
      if (i < b.size()) {
        digitB = b[b.size() - 1 - i];
      }
      int diff = digitA - digitB - borrow;
      if (diff < 0) {
        diff += 10;
        borrow = 1;
      } else {
        borrow = 0;
      }

      result.push_back(diff);
    }

    while (result.size() > 1 and result.back() == 0) {
      result.pop_back();
    }

    std::reverse(result.begin(), result.end());
    return result;
  }
  static int CompareVectors(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b) {
    if (a.size() != b.size()) {
      if (a.size() < b.size()) {
        return -1;
      }
      return 1;
    }
    for (size_t i = 0; i < a.size(); ++i) {
      if (a[i] != b[i]) {
        if (a[i] < b[i]) {
          return -1;
        }
        return 1;
      }
    }
    return 0;
  }

  uint64_t VectorToUint64(const std::vector<uint8_t>& digits) const {
    uint64_t result = 0;
    for (uint8_t digit : digits) {
      assert(result <= (UINT64_MAX - digit) / 10);
      result = result * 10 + digit;
    }
    return result;
  }

public:
  BCD() : negative(false), integer{0}, fractional{} {}
  BCD(int value) {
    negative = (value < 0);
    int absValue = std::abs(value);
    integer.clear();
    if (absValue == 0) {
      integer.push_back(0);
    } else {
      while (absValue > 0) {
        integer.insert(integer.begin(), absValue % 10);
        absValue /= 10;
      }
    }
    fractional.clear();
    assert(integer.size() <= 10);
  }
  BCD(int value, int fract) {
    negative = value < 0;
    int absValue = std::abs(value);
    integer.clear();
    if (absValue == 0) {
      integer.push_back(0);
    } else {
      while (absValue > 0) {
        integer.insert(integer.begin(), absValue % 10);
        absValue /= 10;
      }
    }
    fractional.clear();
    if (fract != 0) {
      int absFract = std::abs(fract);

      std::vector<uint8_t> fracDigits;
      while (absFract > 0) {
        fracDigits.insert(fracDigits.begin(), absFract % 10);
        absFract /= 10;
      }
      fractional = fracDigits;
    } else {
      fractional.push_back(0);
    }

    RemoveFirstZeros();
    RemoveLastZeros();
    assert(integer.size() <= 10);
  }
  BCD(const std::vector<uint8_t>& intPart, const std::vector<uint8_t>& fracPart, bool neg) : negative(neg), integer(intPart), fractional(fracPart)
  {
    RemoveFirstZeros();
    assert(integer.size() <= 10);
  }
  BCD(long long int N, size_t precision, [[maybe_unused]] char rudiment) {
    negative = (N < 0);
    N = std::abs(N);
    if (N == 1) {
      integer = {1};
      fractional = {};
      return;
    }
    integer = {0};
    fractional.resize(precision);
    long long int remainder = 1;
    for (size_t i = 0; i < precision; ++i) {
      remainder *= 10;
      fractional[i] = remainder / N;
      remainder %= N;
    }
    RemoveFirstZeros();
    RemoveLastZeros();
  }

  BCD(BCD&& other) : negative(other.negative), integer(std::move(other.integer)), fractional(std::move(other.fractional)) {}

  BCD& operator=(BCD&& other) {
    if (this != &other) {
      negative = other.negative;
      integer = std::move(other.integer);
      fractional = std::move(other.fractional);
    }
    return *this;
  }

  BCD(const BCD& other) = default;
  BCD& operator=(const BCD& other) = default;


  BCD ceil() const {
    if (fractional.empty() || !FractionalNotZero()) {
      return *this;
    }

    if (!negative) {
      std::vector<uint8_t> newInteger = integer;
      int carry = 1;
      for (int i = newInteger.size() - 1; i >= 0 && carry > 0; --i) {
        int sum = newInteger[i] + carry;
        newInteger[i] = sum % 10;
        carry = sum / 10;
      }
      if (carry > 0) {
        newInteger.insert(newInteger.begin(), carry);
      }
      assert(newInteger.size() <= 10);
      return BCD(newInteger, {0}, negative);
    } else {
      return BCD(integer, {0}, negative);
    }
  }
  BCD floor() const {
    if (fractional.empty() || !FractionalNotZero()) {
      return *this;
    }

    if (!negative) {
      return BCD(integer, {0}, negative);
    } else {
      std::vector<uint8_t> newInteger = integer;
      int carry = 1;
      for (int i = newInteger.size() - 1; i >= 0 && carry > 0; --i) {
        int sum = newInteger[i] + carry;
        newInteger[i] = sum % 10;
        carry = sum / 10;
      }
      if (carry > 0) {
        newInteger.insert(newInteger.begin(), carry);
      }
      assert(newInteger.size() <= 10);
      return BCD(newInteger, {0}, negative);
    }
  }
  BCD round() const {
    if (fractional.empty()) {
      return *this;
    }

    bool roundUp = fractional[0] >= 5;

    if (!roundUp) {
      if (negative) {
        return ceil();
      } else {
        return floor();
      }
    } else {
      if (negative) {
        return floor();
      } else {
        return ceil();
      }
    }
  }

  BCD operator+(const BCD& other) const {
    if (negative != other.negative) {
      return *this - (-other);
    }

    std::vector<uint8_t> frac1 = fractional;
    std::vector<uint8_t> frac2 = other.fractional;
    size_t maxFrac = std::max(frac1.size(), frac2.size());
    frac1.resize(maxFrac, 0);
    frac2.resize(maxFrac, 0);

    std::vector<uint8_t> newInteger = SumVectors(integer, other.integer);
    std::vector<uint8_t> newFractional = SumVectors(frac1, frac2);

    if (newFractional.size() > maxFrac) {
      std::vector<uint8_t> carry(newFractional.begin(), newFractional.begin() + 1);
      newFractional.erase(newFractional.begin());
      newInteger = SumVectors(newInteger, carry);
    }

    size_t newPrecision = std::min(fractional.size(), other.fractional.size());
    if (newPrecision > 0) { newPrecision--; }
    newFractional.resize(newPrecision);

    BCD Ans;
    Ans.negative = negative;
    Ans.integer = newInteger;
    Ans.fractional = newFractional;
    Ans.RemoveFirstZeros();
    assert(Ans.integer.size() <= 10);
    return Ans;
  }
  BCD operator-(const BCD& other) const {
    if (negative != other.negative) {
      return *this + (-other);
    }

    BCD absThis;
    if (other.negative) {
      absThis = -(*this);
    } else {
      absThis = *this;
    };
    BCD absOther;
    if (negative) {
      absOther = -(other);
    } else {
      absOther = other;
    };

    if (absThis < absOther) {
      BCD result = other - *this;
      result.negative = !negative;
      return result;
    }
    std::vector<uint8_t> frac1 = fractional;
    std::vector<uint8_t> frac2 = other.fractional;
    size_t maxFrac = std::max(frac1.size(), frac2.size());
    frac1.resize(maxFrac, 0);
    frac2.resize(maxFrac, 0);

    std::vector<uint8_t> newInteger = MinusVectors(integer, other.integer);
    std::vector<uint8_t> newFractional = MinusVectors(frac1, frac2);

    size_t newPrecision = std::min(fractional.size(), other.fractional.size());
    if (newPrecision > 0) { newPrecision--; }
    newFractional.resize(newPrecision);

    BCD Ans;
    Ans.negative = negative;
    Ans.integer = newInteger;
    Ans.fractional = newFractional;
    Ans.RemoveFirstZeros();
    assert(Ans.integer.size() <= 10);
    return Ans;
  }
  BCD operator*(const BCD& other) const {
    std::vector<uint8_t> digits1 = integer;
    digits1.insert(digits1.end(), fractional.begin(), fractional.end());
    std::vector<uint8_t> digits2 = other.integer;
    digits2.insert(digits2.end(), other.fractional.begin(), other.fractional.end());

    std::vector<uint8_t> result(digits1.size() + digits2.size(), 0);
    for (int i = digits1.size() - 1; i >= 0; --i) {
      for (int j = digits2.size() - 1; j >= 0; --j) {
        int product = digits1[i] * digits2[j] + result[i + j + 1];
        result[i + j + 1] = product % 10;
        result[i + j] += product / 10;
      }
    }

    size_t totalFractional = fractional.size() + other.fractional.size();
    size_t integerPartSize = result.size() - totalFractional;

    BCD absThis;
    if (negative) {
      absThis = -(*this);
    } else {
      absThis = *this;
    };
    BCD absOther;
    if (other.negative) {
      absOther = -(other);
    } else {
      absOther = other;
    };

    uint64_t ceilA = VectorToUint64(absThis.ceil().integer);
    uint64_t ceilB = VectorToUint64(absOther.ceil().integer);
    uint64_t maxInt = std::max(ceilA, ceilB);

    size_t precisionReduction = 1 + static_cast<size_t>(std::log10(maxInt));
    size_t newPrecision = 0;
    if (totalFractional > precisionReduction) {
      newPrecision = totalFractional - precisionReduction;
    }

    BCD Ans;
    Ans.negative = (negative != other.negative);
    Ans.integer.assign(result.begin(), result.begin() + integerPartSize);
    Ans.fractional.assign(result.begin() + integerPartSize, result.end());
    Ans.fractional.resize(newPrecision);
    Ans.RemoveFirstZeros();
    assert(Ans.integer.size() <= 10);
    return Ans;
  }
  void SumPositiveWithHightPrecision(const BCD& other) {
    std::vector<uint8_t> frac1 = fractional;
    std::vector<uint8_t> frac2 = other.fractional;
    size_t maxFrac = std::max(frac1.size(), frac2.size());
    frac1.resize(maxFrac, 0);
    frac2.resize(maxFrac, 0);

    std::vector<uint8_t> newInteger = SumVectors(integer, other.integer);
    std::vector<uint8_t> newFractional = SumVectors(frac1, frac2);

    if (newFractional.size() > maxFrac) {
      std::vector<uint8_t> carry(newFractional.begin(), newFractional.begin() + 1);
      newFractional.erase(newFractional.begin());
      newInteger = SumVectors(newInteger, carry);
    }

    size_t newPrecision = std::max(fractional.size(), other.fractional.size());
    newFractional.resize(newPrecision);
    integer = newInteger;
    fractional = newFractional;
    RemoveFirstZeros();
    assert(integer.size() <= 10);
  }
  void ProductPositiveWithHightPrecision(const BCD& other) {
    std::vector<uint8_t> digits1 = integer;
    digits1.insert(digits1.end(), fractional.begin(), fractional.end());
    std::vector<uint8_t> digits2 = other.integer;
    digits2.insert(digits2.end(), other.fractional.begin(), other.fractional.end());

    std::vector<uint8_t> result(digits1.size() + digits2.size(), 0);
    for (int i = digits1.size() - 1; i >= 0; --i) {
      for (int j = digits2.size() - 1; j >= 0; --j) {
        int product = digits1[i] * digits2[j] + result[i + j + 1];
        result[i + j + 1] = product % 10;
        result[i + j] += product / 10;
      }
    }

    size_t totalFractional = fractional.size() + other.fractional.size();
    size_t integerPartSize = result.size() - totalFractional;

    int newPrecision = static_cast<int>(std::max(this->fractional.size(), other.fractional.size()));

    integer.assign(result.begin(), result.begin() + integerPartSize);
    fractional.assign(result.begin() + integerPartSize, result.end());
    fractional.resize(newPrecision);
    RemoveFirstZeros();
    assert(integer.size() <= 10);
  }

  BCD operator-() const {
    BCD result = *this;
    if (!(integer.size() == 1 && integer[0] == 0 && fractional.empty())) {
      result.negative = !negative;
    }
    return result;
  }
  BCD operator+() const {
    return *this;
  }

  bool operator==(const BCD& other) const {
    return negative == other.negative and integer == other.integer and fractional == other.fractional;
  }
  bool operator<(const BCD& other) const {
    if (negative != other.negative) return negative;
    if (integer != other.integer) {
      return CompareVectors(integer, other.integer) < 0;
    }
    return CompareVectors(fractional, other.fractional) < 0;
  }
  bool operator!=(const BCD& other) const { return !(*this == other); }
  bool operator<=(const BCD& other) const { return (*this < other or *this == other); }
  bool operator>(const BCD& other) const { return !(*this <= other); }
  bool operator>=(const BCD& other) const { return !(*this < other); }

  std::string to_string() const {
    std::string result;
    if (negative) result += '-';
    for (uint8_t digit : integer) {
      assert(digit < 10);
      result += '0' + digit;
    }
    if (!fractional.empty()) {
      result += '.';
      for (uint8_t digit : fractional) {
        assert(digit < 10);
        result += '0' + digit;
      }
    }
    return result;
  }
  void show() const {
    std::cout << to_string() << std::endl;
  }
};

int main() {
  BCD a(10, 25);
  a.show();
  BCD b(3, 11);
  b.show();
  BCD c = a + b;
  c.show();
  BCD d = a * b;
  d.show();
  BCD h(1, 150, 'h');
  h.show();
  std::cout << std::endl;
  BCD factorial(1, 0);
  BCD e(1, 0);
  for (int i = 1; i < 100; i++) {
    factorial.ProductPositiveWithHightPrecision(BCD(i, 110, 'h'));
    //factorial.show();
    e.SumPositiveWithHightPrecision(factorial);
  }
  e.show();
  std::cout << "Real e = \n2,7182818284590452353602874713526624977572470936999595749669676277240766303535475945713821785251664274" << std::endl;
}
