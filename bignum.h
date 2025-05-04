#ifndef BIGNUM_H
#define BIGNUM_H
#define _USE_MATH_DEFINES
#include <cmath>
#include <deque>
#include <string>
#include <cstdint>
class BigNum
{
private:
    std::deque<int8_t> digits;//各数位（倒序存储）
    long long exponent;//指数
    bool isNegative;//正负
    static void alignexponent(BigNum& a,BigNum& b);
    void normalize();
public:
    BigNum();
    explicit BigNum(const std::string& s);
    explicit BigNum(long long num);
    const int operator[] (const size_t& i)const;
    explicit operator std::string();
    const bool& GetNegative()const;
    const long long& GetExponent()const;
    const size_t GetSize()const;
    std::string ToFloatFormart();
    BigNum operator+(const BigNum& rhs)const;
    BigNum operator-(const BigNum& rhs)const;
    BigNum operator*(const BigNum& rhs)const;
    BigNum operator/(const BigNum& rhs)const;
    BigNum operator%(const BigNum& rhs)const;
};

#endif // BIGNUM_H
