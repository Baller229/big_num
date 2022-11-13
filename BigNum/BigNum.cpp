#pragma once
// here you can include whatever you want :)
#include <sstream>

#include <string>
#include <stdint.h>
#include <iostream>
#include <cassert>

using namespace std;

class MyException : public exception
{
public:
   MyException(const string& msg) : m_msg(msg)
   {
      cout << "MyException::MyException - set m_msg to: " << m_msg << endl;
   }

   ~MyException()
   {
      cout << "MyException::~MyException" << endl;
   }

   virtual const char* what() const throw ()
   {
      cout << "MyException::what " << endl;
      return m_msg.c_str();
   }

   const string m_msg;
};

#define SUPPORT_DIVISION 1 // define as 1 when you have implemented the division
#define SUPPORT_IFSTREAM 1 // define as 1 when you have implemented the input >>

// if you do not plan to implement bonus, you can delete those lines
// or just keep them as is and do not define the macro to 1
class BigNum final
{
public:
   // constructors
   BigNum();
   BigNum(int64_t n);
   explicit BigNum(const std::string& str);
   // copy
   BigNum(const BigNum& other);
   BigNum& operator=(const BigNum& rhs);
   // unary operators
   const BigNum& operator+() const;
   BigNum operator-() const;
   // binary arithmetics operators
   BigNum& operator+=(const BigNum& rhs);
   BigNum& operator-=(const BigNum& rhs);
   BigNum& operator*=(const BigNum& rhs);
#if SUPPORT_DIVISION == 1
   BigNum& operator/=(const BigNum& rhs); // bonus
   BigNum& operator%=(const BigNum& rhs); // bonus
#endif
private:
   // here you can add private data and members, but do not add stuff to 
   // public interface, also you can declare friends here if you want
   mutable std::string _bigNum;      //The big number represented as a string

   BigNum& operator++();
   size_t operator[](int index);

   BigNum negate();
   bool isNegative() const;
   BigNum trimLeadingPlus();
   BigNum trimLeadingZeros();
   BigNum setString(const std::string& newStr);
   bool equals(const BigNum& other);

   friend BigNum operator+(BigNum lhs, const BigNum& rhs);
   friend BigNum operator-(BigNum lhs, const BigNum& rhs);
   friend bool operator<(const BigNum& lhs, const BigNum& rhs);
   friend bool operator>(const BigNum& lhs, const BigNum& rhs);
   friend bool operator==(const BigNum& lhs, const BigNum& rhs);
   friend bool operator!=(const BigNum& lhs, const BigNum& rhs);
   friend BigNum operator*(BigNum lhs, const BigNum& rhs);
   friend bool operator<=(const BigNum& lhs, const BigNum& rhs);
   friend bool operator>=(const BigNum& lhs, const BigNum& rhs);

   friend std::ostream& operator<<(std::ostream& lhs, const BigNum& rhs);

#if SUPPORT_DIVISION == 1
   friend BigNum operator/(BigNum lhs, const BigNum& rhs); // bonus
   friend BigNum operator%(BigNum lhs, const BigNum& rhs); // bonus
#endif

#if SUPPORT_IFSTREAM == 1
   friend std::istream& operator>>(std::istream& lhs, BigNum& rhs); // bonus
#endif
};

bool is_number(const std::string& s)
{
   return(strspn(s.c_str(), "-+0123456789") == s.size());
}

std::string trim(const std::string& str, const std::string& whitespace = " \t")
{
   const auto strBegin = str.find_first_not_of(whitespace);
   if (strBegin == std::string::npos)
      return ""; // no content

   const auto strEnd = str.find_last_not_of(whitespace);
   const auto strRange = strEnd - strBegin + 1;

   return str.substr(strBegin, strRange);
}

BigNum::BigNum()
{
   _bigNum = "0";
}

BigNum::BigNum(int64_t n)
{
   _bigNum = std::to_string(n);
}

BigNum::BigNum(const std::string& str)
{
   //_numberString = trim(str);
   _bigNum = str;
   trimLeadingPlus();
   trimLeadingZeros();
   if (_bigNum.starts_with("-0"))
   {
      _bigNum = "0";
   }
   if (is_number(this->_bigNum) && this->_bigNum.size()>0)
   {

   }
   else
   {
      _bigNum = "0";
      stringstream exMsg;
      exMsg << "MyException thrown BigNum: " << "\"" << str << "\"";
      throw (MyException(exMsg.str()));
   }
}

BigNum::BigNum(const BigNum& other)
{
   _bigNum = other._bigNum;
}

BigNum& BigNum::operator=(const BigNum& rhs)
{
   this->_bigNum = rhs._bigNum;
   return *this;
}

const BigNum& BigNum::operator+() const
{
   return *this;
}

BigNum BigNum::operator-() const
{
   BigNum& bn = *new BigNum(this->_bigNum);
   bn.negate();
   this->_bigNum = bn._bigNum;
   delete& bn;
   return *this;
}

BigNum& BigNum::operator+=(const BigNum& rhs)
{
   BigNum b1 = rhs > *this ? rhs : *this;
   BigNum b2 = rhs > *this ? *this : rhs;
   BigNum result;

   if (b1.isNegative() || b2.isNegative())
   {
      if (b1.isNegative() && b2.isNegative())
      {
         result = b1.negate().operator+=(b2.negate()).negate();
      }
      else if (b1.isNegative() && !b2.isNegative())
      {
         result = b1.negate().operator-=(b2).negate();
      }
      else
      {
         result = b2.negate().operator-=(b1).negate();
      }
      this->_bigNum = result._bigNum;
      return *this;
   }
   std::string results;
   int carry = 0;
   int diff = int(b1._bigNum.size() - b2._bigNum.size());
   for (int i = 0; i < diff; ++i)
   {
      b2._bigNum.insert(b2._bigNum.begin(), '0');
   }
   for (int i = int(b1._bigNum.size() - 1); i >= 0; --i)
   {
      int sum = (b1._bigNum[i] - '0') + (b2._bigNum[i] - '0') + carry;
      carry = 0;
      if (sum <= 9 || i == 0)
      {
         results.insert(0, std::to_string(sum));
      }
      else
      {
         results.insert(0, std::to_string(sum % 10));
         carry = 1;
      }
   }
   this->_bigNum = results;
   return *this;
}

BigNum& BigNum::operator-=(const BigNum& rhs)
{
   BigNum b1 = *this, b2 = rhs;
   BigNum result;
   if (b1.isNegative() || b2.isNegative())
   {
      if (b1.isNegative() && b2.isNegative())
      {
         b1 = b1.negate();
         result = b1 + b2;
         result = result.negate();
      }
      else if (b1.isNegative() && !b2.isNegative())
      {
         result = b1.negate().operator+=(b2).negate();
      }
      else
      {
         result = b2.negate().operator+=(b1);
      }
      this->_bigNum = result._bigNum;
      return *this;
   }

   std::string results;
   int n = 0, p = 0;
   bool takeOffOne = false;
   bool shouldBeTen = false;

   if (b1 < b2)
   {
      std::string t = b2.operator-=(*this).negate()._bigNum;
      for (unsigned int i = 1; i < t.length(); ++i)
      {
         if (t[i] != '0') break;
         t.erase(1, 1);
      }
      this->_bigNum = t;
      return *this;
   }

   // if block here fixes case where digit difference is > 1
   // 12345 - 67 is example. Code adds 0's to prepare it, for example, 12345 - 0067
   // which allows rest of subtraction to work
   if (b1._bigNum.size() - b2._bigNum.size() > 1)
   {
      for (unsigned long i = 0; i < b1._bigNum.size() - b2._bigNum.size() - 1; ++i)
      {
         b2._bigNum.insert(b2._bigNum.begin(), '0');
      }
   }
   int i = int(b1._bigNum.size() - 1);
   for (int j = int(b2._bigNum.size() - 1); j >= 0; --j)
   {
      if (((b1._bigNum[i] - '0') < (b2._bigNum[j] - '0')) && i > 0)
      {
         n = char((b1._bigNum[i] - '0') + 10);
         takeOffOne = true;
         if (j > 0 || b1._bigNum[i - 1] != '0')
         {
            p = char((b1._bigNum[i - 1] - '0') - 1);
            if (p == -1)
            {
               p = 9;
               shouldBeTen = true;
            }
            takeOffOne = false;
         }
         if (shouldBeTen)
         {
            int index = i - 1;
            for (int a = i - 1; (b1._bigNum[a] - '0') == 0; --a)
            {
               b1._bigNum[a] = static_cast<char>(p + '0');
               --index;
            }
            int t = (b1._bigNum[index] - '0') - 1;
            b1._bigNum[index] = static_cast<char>(t + '0');
         }
         b1._bigNum[i - 1] = static_cast<char>(p + '0');
         shouldBeTen = false;
      }
      std::stringstream ss;
      if (((b1._bigNum[i] - '0') == (b2._bigNum[j] - '0')))
      {
         ss << "0";
      }
      else
      {
         if (n <= 0)
         {
            ss << ((b1._bigNum[i] - '0') - (b2._bigNum[j] - '0'));
         }
         else
         {
            ss << (n - (b2._bigNum[j] - '0'));
         }
      }

      results.insert(0, ss.str());
      --i;
      n = 0;
   }
   if (takeOffOne)
   {
      std::string number = "";
      for (int j = (int)(b1._bigNum.length() - b2._bigNum.length()) - 1; j >= 0; --j)
      {
         if (b1._bigNum[j] == '0')
         {
            number += "0";
            continue;
         }
         else
         {
            number.insert(number.begin(), b1._bigNum[j]);
            int64_t t = atoll(number.c_str());
            --t;
            b1._bigNum.replace(0, number.size(), std::to_string(t));
            break;
         }
      }
   }
   while (i >= 0)
   {
      std::stringstream ss{};
      if (i == 0)
      {
         if (b1._bigNum[i] - '0' != 0)
         {
            ss << (b1._bigNum[i] - '0');
            results.insert(0, ss.str());
         }
      }
      else
      {
         ss << (b1._bigNum[i] - '0');
         results.insert(0, ss.str());
      }

      --i;
   }
   //In the case of all 0's, we only want to return one of them
   if (results.find_first_not_of('0') == std::string::npos)
   {
      results = "0";
   }
   else if (results[0] == '0')
   {
      size_t index = results.find_first_not_of('0');
      results = results.substr(index, results.length() - 1);
   }
   this->_bigNum = results;
   return *this;
}

BigNum& BigNum::operator*=(const BigNum& other)
{
   BigNum bn1 = other > *this ? other : *this;
   BigNum bN2 = other > *this ? *this : other;
   BigNum result;
   if (bn1.isNegative() || bN2.isNegative())
   {
      if (bn1.isNegative() && bN2.isNegative())
      {
         result = bn1.negate().operator*=(bN2.negate());
      }
      else if (bn1.isNegative() && !bN2.isNegative())
      {
         result = bn1.negate().operator*=(bN2).negate();
      }
      else
      {
         result = bN2.negate().operator*=(bn1).negate();
      }
      this->_bigNum = result._bigNum;
      return *this;
   }
   if (bn1 == 0 || bN2 == 0)
   {
      this->_bigNum = "0";
   }
   int prenos = 0;
   int zeroCounter = 0;
   BigNum b = 0;

   for (unsigned int i = 0; i < bn1._bigNum.size() - bN2._bigNum.size(); ++i)
   {
      bN2._bigNum.insert(bN2._bigNum.begin(), '0');
   }
   for (long long int i = (bN2._bigNum.size() - 1); i >= 0; --i)
   {
      std::string rr;
      for (long long int j = int(bn1._bigNum.size() - 1); j >= 0; --j)
      {
         int val = ((bN2._bigNum[i] - '0') * (bn1._bigNum[j] - '0')) + prenos;
         prenos = 0;
         if (val > 9 && j != 0)
         {
            prenos = val / 10;
            rr.insert(0, std::to_string(val % 10));
         }
         else
         {
            rr.insert(0, std::to_string(val));
         }
      }
      if (zeroCounter > 0)
      {
         for (int x = 0; x < zeroCounter; ++x)
         {
            rr.append("0");
         }
      }
      ++zeroCounter;
      b += BigNum(rr);
   }
   if (b._bigNum.find_first_not_of('0') != std::string::npos)
   {
      b.setString(b._bigNum.erase(0, b._bigNum.find_first_not_of('0')));
   }
   else
   {
      //In the case of all 0's, we only want to return one of them
      b.setString("0");
   }
   this->_bigNum = b._bigNum;
   return *this;
}

BigNum& BigNum::operator/=(const BigNum& rhs) // bonus
{
   if (rhs == 0)
   {
      std::cerr << "You cannot divide by 0!" << std::endl;
   }
   BigNum b1 = *this, b2 = rhs;
   bool sign = false;
   if (b1.isNegative() && b2.isNegative())
   {
      b1.negate();
      b2.negate();
   }
   else if (b1.isNegative() && !b2.isNegative())
   {
      b1.negate();
      sign = true;
   }
   else if (!b1.isNegative() && b2.isNegative())
   {
      b2.negate();
      sign = true;
   }
   BigNum quotient = 0;
   while (b1 >= b2)
   {
      b1 -= b2;
      ++quotient;
   }
   if (sign) quotient.negate();
   this->_bigNum = quotient._bigNum;
   return *this;
}

BigNum& BigNum::operator%=(const BigNum& rhs) // bonus
{
   // Initialize result
   long long res = 0;

   // One by one process all digits of 'num'
   for (size_t i = 0; i < this->_bigNum.length(); i++)
   {
      res = (res * 10 + this->_bigNum[i] - '0') % atoll(rhs._bigNum.c_str());
   }

   _bigNum = std::to_string(res);
   return *this;
}

BigNum& BigNum::operator++()
{
   *this += BigNum("1");
   return *this;
}

BigNum operator+(BigNum lhs, const BigNum& rhs)
{
   lhs += rhs;
   return lhs;
}

BigNum operator-(BigNum lhs, const BigNum& rhs)
{
   lhs -= rhs;
   return lhs;
}

BigNum operator*(BigNum lhs, const BigNum& rhs)
{
   lhs *= rhs;
   return lhs;
}

#if SUPPORT_DIVISION == 1
BigNum operator/(BigNum lhs, const BigNum& rhs) // bonus
{
   lhs /= rhs;
   return lhs;
}
BigNum operator%(BigNum lhs, const BigNum& rhs) // bonus
{
   BigNum bn = rhs;
   bool neg = false;
   if (lhs.isNegative())
   {
      lhs = lhs.negate();
      neg = true;
   }
   if (bn.isNegative())
   {
      bn = bn.negate();
   }

   lhs %= bn;
   if (neg)
   {
      lhs = lhs.negate();
   }
   return lhs;
}
#endif

// alternatively you can implement 
// std::strong_ordering operator<=>(const BigNum& lhs, const BigNum& rhs);
// idea is, that all comparison should work, it is not important how you do it
bool operator==(const BigNum& lhs, const BigNum& rhs)
{
   BigNum bn = BigNum(lhs);
   return bn.equals(rhs);
}

bool operator!=(const BigNum& lhs, const BigNum& rhs)
{
   BigNum bn = BigNum(lhs);
   return !bn.equals(rhs);
}

bool operator<(const BigNum& lhs, const BigNum& rhs)
{
   return !(lhs == rhs) && !(lhs > rhs);
}

bool operator>(const BigNum& lhs, const BigNum& b2)
{
   BigNum b1 = BigNum(lhs);
   if (b1.isNegative() || b2.isNegative())
   {
      if (b1.isNegative() && b2.isNegative())
      {
         BigNum bt = b2;
         b1._bigNum.erase(0, 1);
         bt._bigNum.erase(0, 1);
         return b1 < bt;
      }
      else
      {
         return !(b1.isNegative() && !b2.isNegative());
      }
   }
   b1 = b1.trimLeadingZeros();
   auto c = BigNum(b2);
   c = c.trimLeadingZeros();
   if (b1 == c)
   {
      return false;
   }
   if (b1._bigNum.size() > c._bigNum.size())
   {
      return true;
   }
   else if (c._bigNum.size() > b1._bigNum.size())
   {
      return false;
   }
   else
   {
      for (unsigned int i = 0; i < b1._bigNum.size(); ++i)
      {
         if (b1[i] == static_cast<size_t>(c._bigNum[i] - '0'))
         {
            continue;
         }
         return b1[i] > static_cast<size_t>(c._bigNum[i] - '0');
      }
   }
   return false;
}

bool operator<=(const BigNum& lhs, const BigNum& rhs)
{
   return lhs < rhs || lhs == rhs;
}
bool operator>=(const BigNum& lhs, const BigNum& rhs)
{
   return lhs > rhs || lhs == rhs;
}

std::ostream& operator<<(std::ostream& lhs, const BigNum& rhs)
{
   return lhs << rhs._bigNum;
}

#if SUPPORT_IFSTREAM == 1
std::istream& operator>>(std::istream& lhs, BigNum& rhs) // bonus
{
   return lhs >> rhs._bigNum;
}
#endif


BigNum BigNum::negate()
{
   if (this->_bigNum[0] == '-')
   {
      this->_bigNum.erase(0, 1);
   }
   else
   {
      this->_bigNum.insert(this->_bigNum.begin(), '-');
   }
   return *this;
}

bool BigNum::isNegative() const
{
   return this->_bigNum[0] == '-';
}

BigNum BigNum::trimLeadingPlus()
{
   BigNum b = *this;
   if (b._bigNum.find_first_not_of('+') != std::string::npos)
   {
      b.setString(b._bigNum.erase(0, b._bigNum.find_first_not_of('+')));
   }
   this->_bigNum = b._bigNum;
   return *this;
}

BigNum BigNum::trimLeadingZeros()
{
   BigNum b = *this;
   if (b._bigNum.find_first_not_of('0') != std::string::npos)
   {
      b.setString(b._bigNum.erase(0, b._bigNum.find_first_not_of('0')));
   }
   this->_bigNum = b._bigNum;
   return *this;
}

BigNum BigNum::setString(const std::string& newStr)
{
   this->_bigNum = newStr;
   return *this;
}

size_t BigNum::operator[](int index)
{
   if (this->_bigNum[index] == '-')
   {
      std::cerr << "You cannot get the negative sign from the number" << std::endl;
   }
   return static_cast<unsigned int>(this->_bigNum[index] - '0');
}

bool BigNum::equals(const BigNum& other)
{
   return this->_bigNum == other._bigNum;
}



int test()
{
   //operator+
   assert((BigNum(50) + BigNum(32)) == BigNum("82"));
   assert((BigNum(5) + BigNum(622)) == BigNum("627"));
   assert((BigNum("-33") + BigNum("8")) == BigNum("-25"));
   assert((BigNum("15535") + BigNum("0")) == BigNum("15535"));
   assert((BigNum("126") + BigNum("39285")) == BigNum("39411"));
   assert((BigNum("0") + BigNum("0")) == BigNum("0"));
   assert(BigNum(5) + 10 == 15);
   assert(BigNum("-41") + 40 == -1);
   BigNum ad1(600);
   ad1 += 50;
   ad1 += 50;
   assert(ad1 == BigNum("700"));
   assert(ad1 == 700);

   //operator-
   assert((BigNum("50") - BigNum("32")) == BigNum("18"));
   assert((BigNum("50") - BigNum("60")) == BigNum("-10"));
   assert((BigNum("0") - BigNum("46")) == BigNum("-46"));
   assert((BigNum("50") - BigNum("50")) == BigNum("0"));
   assert((BigNum("482847") - BigNum("89787941")) == BigNum("-89305094"));
   assert((BigNum("6828") - BigNum("1")) == BigNum("6827"));
   assert((BigNum("100") - BigNum("50")) == BigNum("50"));
   assert((BigNum("42") - BigNum("49")) == BigNum("-7"));
   assert((BigNum("100") - BigNum("5")) == 95);
   BigNum sb1 = 200;
   sb1 -= 40;
   assert(sb1 == 160);
   sb1 = sb1 - 180;
   assert(sb1 == -20);
   sb1 -= 20;
   assert(sb1 == BigNum(-40));

   //operator*
   assert((BigNum("4") * BigNum("12")) == BigNum("48"));
   assert((BigNum("3002") * BigNum("1")) == BigNum("3002"));
   assert((BigNum("99") * BigNum("0")) == BigNum("0"));
   assert((BigNum("-5") * BigNum("5")) == BigNum("-25"));
   assert((BigNum("-33") * BigNum("-2")) == BigNum("66"));
   assert((BigNum("283") * BigNum("382871")) == BigNum("108352493"));
   BigNum ml1 = 4;
   ml1 *= 6;
   assert(ml1 == 24);
   ml1 = BigNum(5) * 6;
   assert(ml1 == 30);
   ml1 *= 5000;
   assert(ml1 == 150000);

   //operator/
   assert(BigNum("25") / (BigNum("5")) == 5);
   assert(BigNum("48") / (BigNum("6")) == 8);
   assert(BigNum("100") / (BigNum("5")) == 20);
   BigNum dv1 = 100;
   dv1 /= 25;
   assert(dv1 == 4);
   dv1 = dv1 / dv1;
   assert(dv1 == 1);
   dv1 /= 1;
   assert(dv1 == 1);
   dv1 = -5;
   dv1 /= 5;
   assert(dv1 == -1);
   dv1 = 3000;
   dv1 /= 300;
   assert(dv1 == 10);
   dv1 = 25485;
   dv1 /= 5;
   assert(dv1 == 5097);

   //operator==
   assert(BigNum("4") == BigNum("4"));
   assert(BigNum("-3") == BigNum("-3"));
   assert(BigNum("0") == BigNum("0"));
   assert(BigNum("938283828178273") == BigNum("938283828178273"));

   //operator>
   assert(BigNum("5") > BigNum("2"));
   assert(BigNum("30") > BigNum("-40"));
   assert(BigNum("-5") > BigNum("-10"));
   assert(BigNum("0") > BigNum("-1"));

   //operator<
   assert(BigNum("10") < BigNum("20"));
   assert(BigNum("-5") < BigNum("0"));
   assert(BigNum("30") < BigNum("30000"));

   //operator>=
   assert(BigNum("5") >= BigNum("0"));
   assert(BigNum("-5") >= BigNum("-5"));
   assert(BigNum("-5") >= BigNum("-10"));
   assert(BigNum("0") >= BigNum("0"));
   assert(BigNum("32") >= BigNum("-32"));
   assert(BigNum("2") >= BigNum("0001"));

   //operator<=
   assert(BigNum("5") <= BigNum("10"));
   assert(BigNum("0") <= BigNum("0"));
   assert(BigNum("-5") <= BigNum("0"));
   assert(BigNum("30") <= BigNum("30"));
   assert(BigNum("400") <= BigNum("392342"));

#if 0
   //Index
   assert(BigNum("423")[1] == 2);
   assert(BigNum("0")[0] == 0);
   assert(BigNum("-5")[1] == 5);

   //Even
   assert(BigNum("426").isEven());
   assert(BigNum("-20").isEven());

   //Odd
   assert(BigNum("83").isOdd());
   assert(BigNum("-27").isOdd());

   //Positive
   assert(BigNum("38").isPositive());

   //Negative
   assert(BigNum("-28382").isNegative());

   //Increment/Decrement operators
   assert(BigNum("5")-- == BigNum("5");
   assert((--BigNum("5")) == BigNum("4");
   assert(BigNum("10")++ == BigNum("10");
   assert((++BigNum("10")) == BigNum("11");

   BigNum a("10");
   a++;
   assert(a == BigNum("11");
   ++a;
   assert(a == BigNum("12");
   a--;
   assert(a == BigNum("11");
   --a;
   assert(a == BigNum("10");

   //Absolute value
   assert(BigNum("45").abs() == BigNum("45");
   assert(BigNum("-325").abs() == BigNum("325");

   //Digits
   assert(BigNum("28374765").digits() == 8);
   assert(BigNum("-3092").digits() == 4);

   //Set string
   assert(BigNum("234").setString("-45") == BigNum("-45");
#endif
   //Assignment operator
   BigNum c(10);
   c = 5;
   assert(c == 5);
   assert(c == BigNum(5));
   assert(c == BigNum("5"));
   c = 83833;
   assert(c == 83833);
   assert(c == BigNum(83833));
   assert(c == BigNum("83833"));

   //oerator== testing
   BigNum d(40);
   assert(d == 40);
   assert(d == 40);
   assert(d == BigNum("40"));
   assert(d == BigNum(40));
   d = 40;
   assert(d == 40);
   d = 40;
   assert(d == 40);

   std::cout << "BigNum ran successfully." << std::endl;
   return 0;
}

void test1()
{
   BigNum first("57435634786573863876590674847606758976036570987777765367089567342096703295867302967392048673920467389420653402673549206758394206573420675340657406857406764598686786758489567839589");
   BigNum second("698534675489367589673486067089546738906706706537687603594675409683574098376389067509836743097607667898966045645645695846758965896745896457694856874");
   BigNum plus(first + second);
   BigNum minus(first - second);
   BigNum times(first * second);
   BigNum plus_res("57435634786573863876590674847607457510712060355367438853156656888835610002573840654995643349330150963519029791741059043501491814241319641386303052553253523564583532654947262696463");
   BigNum minus_res("57435634786573863876590674847606060441361081620188091881022477795357796589160765279788453998510783815322277013606039370015296598905521709295011761161560005632790040862031872982715");
   BigNum times_res("40120782507165206523400788281606223778598018896699955752819768339268359999285800277556147439442826877027209676958001364196213027145256069838157679806151733555497659329496572330150008795634919379108433546030083129069338657506736163221609413906671985907467083201662111482928116978362229519658111400689698131863233506800745984786");
   assert(plus == plus_res);
   assert(minus == minus_res);
   assert(times == times_res);
   std::cout << "test1 passed\n";
}

void test2()
{
   BigNum first("-57435634786573863876590674847606758976036570987777765367089567342096703295867302967392048673920467389420653402673549206758394206573420675340657406857406764598686786758489567839589");
   BigNum second("-698534675489367589673486067089546738906706706537687603594675409683574098376389067509836743097607667898966045645645695846758965896745896457694856874");
   BigNum plus(first + second);
   BigNum minus(first - second);
   BigNum times(first * second);
   BigNum plus_res("-57435634786573863876590674847607457510712060355367438853156656888835610002573840654995643349330150963519029791741059043501491814241319641386303052553253523564583532654947262696463");
   BigNum minus_res("-57435634786573863876590674847606060441361081620188091881022477795357796589160765279788453998510783815322277013606039370015296598905521709295011761161560005632790040862031872982715");
   BigNum times_res("40120782507165206523400788281606223778598018896699955752819768339268359999285800277556147439442826877027209676958001364196213027145256069838157679806151733555497659329496572330150008795634919379108433546030083129069338657506736163221609413906671985907467083201662111482928116978362229519658111400689698131863233506800745984786");
   assert(plus == plus_res);
   assert(minus == minus_res);
   assert(times == times_res);
   std::cout << "test2 passed\n";

}

void test3()
{
   BigNum first("-57435634786573863876590674847606758976036570987777765367089567342096703295867302967392048673920467389420653402673549206758394206573420675340657406857406764598686786758489567839589");
   BigNum second("698534675489367589673486067089546738906706706537687603594675409683574098376389067509836743097607667898966045645645695846758965896745896457694856874");
   BigNum plus(first + second);
   BigNum minus(first - second);
   BigNum times(first * second);
   BigNum plus_res("-57435634786573863876590674847606060441361081620188091881022477795357796589160765279788453998510783815322277013606039370015296598905521709295011761161560005632790040862031872982715");
   BigNum minus_res("-57435634786573863876590674847607457510712060355367438853156656888835610002573840654995643349330150963519029791741059043501491814241319641386303052553253523564583532654947262696463");
   BigNum times_res("-40120782507165206523400788281606223778598018896699955752819768339268359999285800277556147439442826877027209676958001364196213027145256069838157679806151733555497659329496572330150008795634919379108433546030083129069338657506736163221609413906671985907467083201662111482928116978362229519658111400689698131863233506800745984786");
   assert(plus == plus_res);
   assert(minus == minus_res);
   assert(times == times_res);
   std::cout << "test3 passed\n";

}

void test4()
{
   BigNum first("57435634786573863876590674847606758976036570987777765367089567342096703295867302967392048673920467389420653402673549206758394206573420675340657406857406764598686786758489567839589");
   BigNum second("-698534675489367589673486067089546738906706706537687603594675409683574098376389067509836743097607667898966045645645695846758965896745896457694856874");
   BigNum plus(first + second);
   BigNum minus(first - second);
   BigNum times(first * second);
   BigNum plus_res("57435634786573863876590674847606060441361081620188091881022477795357796589160765279788453998510783815322277013606039370015296598905521709295011761161560005632790040862031872982715");
   BigNum minus_res("57435634786573863876590674847607457510712060355367438853156656888835610002573840654995643349330150963519029791741059043501491814241319641386303052553253523564583532654947262696463");
   BigNum times_res("-40120782507165206523400788281606223778598018896699955752819768339268359999285800277556147439442826877027209676958001364196213027145256069838157679806151733555497659329496572330150008795634919379108433546030083129069338657506736163221609413906671985907467083201662111482928116978362229519658111400689698131863233506800745984786");
   assert(plus == plus_res);
   assert(minus == minus_res);
   assert(times == times_res);
   std::cout << "test4 passed\n";

}

int main(int /*argc*/, char** /*argv*/)
{
   //cout << argc << endl;
   //for (int i = 0; i < argc; i++)
   //{
   //   cout << argv[i] << endl;
   //}

   //std::cout << BigNum(" ") << endl; // err
   //std::cout << BigNum(" 123 ") << endl; // err
   //std::cout << BigNum("") << endl; // err

   std::cout << "(BigNum(\"-0\") == BigNum(\"+0\")) :" << (BigNum("-0") == BigNum("+0")) << endl;
   std::cout << "(BigNum(\"123\") == BigNum(\"00123\")) :" << (BigNum("123") == BigNum("00123")) << endl;
   std::cout << "(BigNum(\"0123\") == BigNum(\"+00123\")) :" << (BigNum("0123") == BigNum("+00123")) << endl;

   //std::cout << "(BigNum(\" 0123\") == BigNum(\" +00123\")) :" << (BigNum(" 0123") == BigNum(" +00123")) << endl; // exc

   //try
   //{
   //   std::cout << "(BigNum(\" 0123\") == BigNum(\" +00123\")) :" << (BigNum(" 0123") == BigNum(" +00123")) << endl; // exc
   //}
   //catch (const MyException& e)
   //{
   //   cout << "caught an MyException, e.what:: " << e.what() << endl;
   //}

   //try
   //{
   //   std::cout << "(BigNum(\"0123 \") == BigNum(\"+00123 \")) :" << (BigNum("0123 ") == BigNum("+00123 ")) << endl; // exc
   //}
   //catch (const MyException& e)
   //{
   //   cout << "caught an MyException, e.what:: " << e.what() << endl;
   //}

   std::cout << "BigNum( \"-0\")" << BigNum("-0") << endl;
   std::cout << "BigNum( \"+0\")" << BigNum("+0") << endl;

   std::cout << "+BigNum( \"6\")" << +BigNum("6") << endl;

   std::cout << "+BigNum( \"6\")" << +BigNum("6") << endl;
   std::cout << "-BigNum( \"7\")" << -BigNum("7") << endl;
   std::cout << "+BigNum(\"-8\")" << +BigNum("-8") << endl;
   std::cout << "-BigNum(\"-9\")" << -BigNum("-9") << endl;
   std::cout << BigNum() << endl;
   std::cout << BigNum("100") << endl;
   std::cout << BigNum("-100") << endl;
   std::cout << BigNum("+100") << endl;
   //std::cout << BigNum("X100456XY") << endl; // err

   std::cout << "BigNum(-100) - BigNum(-20) :" << (-100 - -20) << BigNum(-100) - BigNum(-20) << endl;
   std::cout << "BigNum(-100) - BigNum(20) :" << (-100 - 20) << BigNum(-100) - BigNum(20) << endl;

   test();
   test1();
   test2();
   test3();
   test4();

   std::cout << BigNum(+100) / BigNum(-20) << endl;
   std::cout << BigNum(100) / BigNum(20) << endl;
   std::cout << BigNum(5) * BigNum(20) << endl;
   std::cout << BigNum(100) + BigNum(20) << endl;
   std::cout << BigNum(100) - BigNum(20) << endl;
   std::cout << BigNum(17) % BigNum(9) << endl;

   std::cout << "( 17 %  9) :" << (17 % 9) << endl; // 8
   std::cout << "( 17 % -9) :" << (17 % -9) << endl; // 8
   std::cout << "(-17 %  9) :" << (-17 % 9) << endl; // -8
   std::cout << "(-17 % -9) :" << (-17 % -9) << endl; // -8

   std::cout << "BigNum(17) % BigNum(9) :" << BigNum(17) % BigNum(9) << endl;
   std::cout << "BigNum(17) % BigNum(-9) :" << BigNum(17) % BigNum(-9) << endl;
   std::cout << "BigNum(-17) % BigNum(9) :" << BigNum(-17) % BigNum(9) << endl;
   std::cout << "BigNum(-17) % BigNum(-9) :" << BigNum(-17) % BigNum(-9) << endl;

   BigNum& bigNum = *new BigNum(76543210987654321);
   cout << bigNum << endl;

   BigNum& bigNum1 = *new BigNum("120");
   cout << bigNum1 << endl;
   BigNum& bigNum2 = *new BigNum("-2");
   cout << bigNum2 << endl;

   bigNum1 += bigNum2;
   cout << bigNum1 << endl;

   bigNum1 = bigNum1 + bigNum2;
   cout << bigNum1 << endl;

   bigNum1 = bigNum1 + bigNum2;
   cout << bigNum1 << endl;

   bigNum1 = +bigNum2;
   cout << bigNum1 << endl;

   bigNum1 = -bigNum2;
   cout << bigNum1 << endl;

   bigNum1 += bigNum;
   cout << bigNum1 << endl;

   BigNum bigNum3 = BigNum(3);
   BigNum bigNum4 = BigNum(4);

   bigNum3 *= bigNum4;
   cout << bigNum3 << endl;
   bigNum3 = bigNum3 * bigNum4;
   cout << bigNum3 << endl;

   bigNum3 = BigNum(3);
   bigNum4 = BigNum(-4);

   bigNum3 *= bigNum4;
   cout << bigNum3 << endl;

   bigNum3 = BigNum(-3);
   bigNum4 = BigNum(-4);

   bigNum3 *= bigNum4;
   cout << bigNum3 << endl;

   bigNum3 = BigNum(12);
   bigNum4 = BigNum(4);

   bigNum3 /= bigNum4;
   cout << bigNum3 << endl;

   bigNum3 = bigNum3 / bigNum4;
   cout << bigNum3 << endl;


   bigNum3 = BigNum(19);
   bigNum4 = BigNum(7);

   bigNum3 %= bigNum4;
   cout << bigNum3 << endl;

   test();

   delete& bigNum1;
   delete& bigNum2;
   return 0;
}

