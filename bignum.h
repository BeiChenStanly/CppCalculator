#ifndef BIGNUM_H
#define BIGNUM_H
#include <compare>
#include <cstdint>
#include <deque>
#include <string>
#include <utility>
#define _USE_MATH_DEFINES
#include <cmath>
class BigNum
{
private:
    std::deque<int8_t> digits; // 各数位（倒序存储）
    long long exponent;        // 指数
    bool isNegative;           // 正负
    static void alignexponent(BigNum &a, BigNum &b);
    void normalize();
    static size_t precision;

public:
    BigNum();
    explicit BigNum(const std::string &s);
    explicit BigNum(long long num);
    explicit BigNum(size_t num);
    const int operator[](const size_t &i) const;
    explicit operator std::string();
    const bool &GetNegative() const;
    const long long &GetExponent() const;
    const size_t GetSize() const;
    static void SetPrecision(size_t new_precision);
    static size_t GetPrecision();
    std::string ToFloatFormart();
    std::strong_ordering operator<=>(const BigNum &rhs) const;
    BigNum operator+(const BigNum &rhs) const;
    BigNum operator-(const BigNum &rhs) const;
    BigNum operator*(const BigNum &rhs) const;
    BigNum operator/(const BigNum &rhs) const;
    BigNum operator%(const BigNum &rhs) const;
    static std::pair<BigNum, BigNum> Divide(const BigNum &dividend, const BigNum &divisor, size_t precision);
    static std::pair<BigNum, BigNum> Divide(const BigNum &dividend, const BigNum &divisor);
};
#endif // BIGNUM_H
