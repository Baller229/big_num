#pragma once
// here you can include whatever you want :)
#include <string>
#include <stdint.h>
#include <iostream>

using namespace std;

#define SUPPORT_DIVISION 0 // define as 1 when you have implemented the division
#define SUPPORT_IFSTREAM 0 // define as 1 when you have implemented the input >>

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
};
BigNum operator+(BigNum lhs, const BigNum& rhs);
BigNum operator-(BigNum lhs, const BigNum& rhs);
BigNum operator*(BigNum lhs, const BigNum& rhs);
#if SUPPORT_DIVISION == 1
BigNum operator/(BigNum lhs, const BigNum& rhs); // bonus
BigNum operator%(BigNum lhs, const BigNum& rhs); // bonus
#endif

// alternatively you can implement 
// std::strong_ordering operator<=>(const BigNum& lhs, const BigNum& rhs);
// idea is, that all comparison should work, it is not important how you do it
bool operator==(const BigNum& lhs, const BigNum& rhs);
bool operator!=(const BigNum& lhs, const BigNum& rhs);
bool operator<(const BigNum& lhs, const BigNum& rhs);
bool operator>(const BigNum& lhs, const BigNum& rhs);
bool operator<=(const BigNum& lhs, const BigNum& rhs);
bool operator>=(const BigNum& lhs, const BigNum& rhs);

std::ostream& operator<<(std::ostream& lhs, const BigNum& rhs);

#if SUPPORT_IFSTREAM == 1
std::istream& operator>>(std::istream& lhs, BigNum& rhs); // bonus
#endif

int main(int argc, char** argv)
{
    cout << argc << endl;
    for (int i = 0; i < argc; i++)
    {
        cout << argv[i] << endl;
    }

    BigNum& bigNum1 = *new BigNum("123");
    BigNum& bigNum2 = *new BigNum("124");

    bigNum1 += bigNum2;

    bigNum1 = bigNum1 + bigNum2;

    delete& bigNum1;
    delete& bigNum2;
    return 0;
}

BigNum::BigNum()
{
}

BigNum::BigNum(int64_t /*n*/)
{
}

BigNum::BigNum(const std::string& str)
{
    string b = str;
}

BigNum& BigNum::operator=(const BigNum& rhs)
{
    // // O: insert return statement here
    BigNum& ret = *new BigNum("123");
    return ret;
}

BigNum& BigNum::operator+=(const BigNum& rhs)
{
    // // O: insert return statement here
    BigNum& ret = *new BigNum(rhs);
    return ret;
}


BigNum operator+(BigNum lhs, const BigNum& rhs)
{
    BigNum& ret = *new BigNum(rhs);
    ret += lhs;
    return ret;
}
