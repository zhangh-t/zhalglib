#pragma once
#include <iostream>
#include <vector>
using namespace std;
class hugeInteger
{
    friend ostream& operator<<(ostream& out, hugeInteger& hi);
public:
    hugeInteger();
    explicit hugeInteger(int num);
    hugeInteger(int nCount, char nums[], bool bPos);
    hugeInteger(int nCount, int array[], bool bPos);
    hugeInteger(const hugeInteger& other);
    hugeInteger(hugeInteger&& other);
    hugeInteger& operator = (const hugeInteger& other);
    ~hugeInteger(void);
public:
    hugeInteger operator + (const hugeInteger& other);
    hugeInteger operator - (const hugeInteger& other);
    hugeInteger operator * (const hugeInteger& other);
    hugeInteger operator / (const hugeInteger& other);
public:
    hugeInteger& operator += (const hugeInteger& other);
    hugeInteger& operator -= (const hugeInteger& other);
    hugeInteger& operator *= (const hugeInteger& other);
    hugeInteger& operator /= (const hugeInteger& other);
public:
    hugeInteger operator++ (int);    //后置++
    hugeInteger& operator++ ();    //前置++
    hugeInteger operator-- (int);
    hugeInteger& operator-- ();
public:
    int relation(const hugeInteger& other);
    bool isZero() const;
    void multTen(int nPow);
    void divTen(int nPow);
private:
    void add(const hugeInteger& other);    //加并修改自身
    void sub(const hugeInteger& other);    //减并修改自身
    void mult(const hugeInteger& other);    //乘并修改自身
    void div(const hugeInteger& other);    //除并修改自身
private:
    void absoluteAdd(const hugeInteger& other);
    bool absoluteSub(const hugeInteger& other);
    hugeInteger divideAndConquer(int* data1, int nDigit1, int nLength1,
        int* data2, int nDigit2, int nLength2);
    int timesOfTen();
private:
    void free();
    void copy(const hugeInteger& other);
private:
    int* m_pData;
    int m_nDigit;   //位数
    int m_nLength;  //长度 >= 位数
    bool m_bPos;    //正负号标记
    static hugeInteger s_one;
};


ostream& operator << (ostream& out, hugeInteger& hi);
