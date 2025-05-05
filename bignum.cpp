#include "bignum.h"
#include <sstream>
void BigNum::normalize()//标准化，即处理前后导0，使1.1451400e2变成1.14514e2，0.114514e3变成1.14514e2
{
    while (!digits.empty() && digits.front() == 0)
    {//去后导0，注意存储是倒序的，所以这里是后导
        digits.pop_front();
    }
    while (!digits.empty() && digits.back() == 0)
    {//去除前导0
        digits.pop_back();
        --exponent;//去除前导0等价于把第一位往右边移，因为第一位向右边移，所以指数相对减一
    }
    if (digits.empty())
    {
        digits.push_back(0);
        exponent = 0;
        isNegative = false;
    }
}
void BigNum::alignexponent(BigNum& a,BigNum& b)//让两个数完全对齐
{
    while (a.exponent < b.exponent) {//后面添0
        a.digits.push_back(0);
        a.exponent++;
    }
    while (b.exponent < a.exponent) {
        b.digits.push_back(0);
        b.exponent++;
    }
    while (a.GetSize() < b.GetSize())//前面添0
    {
        a.digits.push_front(0);
    }
    while (b.GetSize() < a.GetSize())
    {
        b.digits.push_front(0);
    }
}
BigNum::BigNum()//默认构造为0
{
    isNegative = 0;
    exponent = 0;
    digits.clear();
    digits.resize(1);
    digits[0] = 0;
}
BigNum::BigNum(const std::string& s)
{
    //初始化正负
    size_t start = 0;
    isNegative = bool(s[0] == '-');
    if (isNegative || s[0] == '+')
        ++start;
    size_t ePos = s.find_first_of("Ee");//注意这个find_first_of是指Ee中的任意一个第一个索引

    //处理e后指数
    if (ePos != std::string::npos)//如果是形如1.14514e5
    {//注意到11.4514e4这样的输入也有可能，所以指数之后还需要加
        exponent = std::stoi(s.substr(ePos + 1, s.size() - ePos - 1));
    }
    else//否则是形如11.4514
    {
        exponent = 0;//这里只是e后初始值
    }

    //初始化数位、指数
    std::string absoluteValue = s.substr(start, ePos == std::string::npos ? s.size() - start : ePos - start);//与npos比较可以知道是否找到，去掉符号和e之后的指数
    size_t pointPos = absoluteValue.find('.');//小数点
    if (pointPos != std::string::npos)
    {
        absoluteValue.erase(pointPos, 1);
    }
    else
    {
        pointPos = absoluteValue.size();
    }
    exponent += pointPos - 1;
    digits.resize(absoluteValue.size());
    for (size_t i = 0;i < absoluteValue.size();++i)
    {
        digits[i] = absoluteValue[absoluteValue.size() - 1 - i]-'0';
    }
    normalize();//标准化
}
BigNum::BigNum(long long num)
{
    isNegative = num < 0;
    if (isNegative) num = -num;
    exponent = floor(log10(num));
    for (size_t i = 0;i <= exponent;i++)//因为要第一位，所以多循环一次
    {
        digits.push_back(num % 10);
        num /= 10;
    }
}
const int BigNum::operator[] (const size_t& i)const
{
    return digits[digits.size() - 1 - i];
}
const size_t BigNum::GetSize()const//血的教训：为什么不返回引用const size_t&？因为digits.size()返回的是右值，不是左值
{
    return digits.size();
}
const long long& BigNum::GetExponent()const
{
    return exponent;
}
const bool& BigNum::GetNegative()const
{
    return isNegative;
}
BigNum::operator std::string()
{
    std::stringstream ss;
    ss << (isNegative ? "-" : "");
    ss << (*this)[0] << '.';
    if (GetSize() != 1)
    {
        for (size_t i = 1;i < GetSize();++i)
        {
            ss << (*this)[i];
        }
    }
    else
    {
        ss << '0';
    }
    ss << "×10^" << (exponent < 0 ? "(" : "") << exponent << (exponent < 0 ? ")" : "");
    return ss.str();
}
std::strong_ordering BigNum::operator<=>(const BigNum &rhs) const//三相比较运算符(C++20)
{
    if (isNegative != rhs.isNegative)
    {
        return rhs.isNegative <=> isNegative;
    }
    else
    {
        BigNum a(*this);
        BigNum b(rhs);
        alignexponent(a, b);
        for (size_t i = 0; i < a.GetSize(); ++i)
        {
            if (a[i] != b[i])
            {
                return a[i] <=> b[i];
            }
        }
        return std::strong_ordering::equal;
    }
}
BigNum BigNum::operator+(const BigNum &rhs) const
{
    if (isNegative == rhs.isNegative)//同号逻辑
    {
        BigNum a(*this), b(rhs);//创建副本
        alignexponent(a, b);//让两个数头部对齐（通过前面补0的方式）
        BigNum result;
        result.isNegative = isNegative;
        result.exponent = a.exponent;

        int8_t carry = 0;//上一位的进位
        size_t max_len = std::max(a.digits.size(), b.digits.size());
        for (size_t i = 0; i < max_len || carry; ++i) {
            int8_t sum = carry;
            if (i < a.digits.size()) sum += a.digits[i];
            if (i < b.digits.size()) sum += b.digits[i];
            result.digits.push_back(sum % 10);
            carry = sum / 10;
            if (i >= max_len)//当进位时，需要把指数增加
            {
                ++result.exponent;
            }
        }
        result.normalize();
        return result;
    }
    else//异号逻辑
    {
        if (rhs.isNegative == true)
        {
            BigNum copy = rhs;
            copy.isNegative = false;
            return (*this) - copy;
        }
        else
        {
            BigNum copy = (*this);
            copy.isNegative = false;
            return rhs - copy;
        }
    }
}
BigNum BigNum::operator-(const BigNum& rhs)const
{
    if (isNegative != rhs.isNegative)//异号减法转换为加法
    {
        if (isNegative)
        {
            BigNum copy = (*this);
            copy.isNegative = false;
            BigNum result = rhs + copy;
            result.isNegative = true;
            return result;
        }
        else
        {
            BigNum copy = rhs;
            copy.isNegative = false;
            BigNum result = copy + (*this);
            return result;
        }
    }
    else
    {//同号减法转化为大减小，再确定符号
        BigNum maxnum,minnum;//差直接在maxnum上构建
        if ((*this) >= rhs)
        {
            maxnum = (*this);
            minnum = rhs;
            maxnum.isNegative = false;
        }
        else
        {
            minnum = (*this);
            maxnum = rhs;
            maxnum.isNegative = true;
        }
        alignexponent(maxnum, minnum);
        for (size_t i = 0;i < maxnum.digits.size();++i)
        {
            maxnum.digits[i] -= minnum.digits[i];
            if (maxnum.digits[i] < 0)
            {
                maxnum.digits[i] += 10;
                --maxnum.digits[i + 1];
            }
        }
        maxnum.normalize();
        return maxnum;
    }
}
std::string BigNum::ToFloatFormart()
{
    std::stringstream ss;
    if(isNegative) ss <<'-';//符号
    if(exponent<0)//指数小于0，需要在前面添0
    {
        ss << "0.";
        for(long long i=1;i<-exponent;++i)
        {
            ss<<'0';
        }
        for(size_t i = 0;i<GetSize();++i)
        {
            ss<<(*this)[i];
        }
    }
    else//指数大于等于0，需要在后面添0
    {
        for(long long i=0;i<exponent+1;++i)
        {
            if(i<digits.size()) ss<<(*this)[i];
            else ss<<'0';//这个是末尾为几个0的大整数，如114514000
        }
        ss<<'.';
        if(digits.size()>exponent+1)//小数部分
        {
            for(long long i=exponent+1;i<digits.size();++i)
            {
                ss<<(*this)[i];
            }
        }
        else
        {
            ss<<'0';
        }
    }
    return ss.str();
}
