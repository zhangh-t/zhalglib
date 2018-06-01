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
    hugeInteger operator++ (int);    //����++
    hugeInteger& operator++ ();    //ǰ��++
    hugeInteger operator-- (int);
    hugeInteger& operator-- ();
public:
    int relation(const hugeInteger& other);
    bool isZero() const;
    void multTen(int nPow);
    void divTen(int nPow);
private:
    void add(const hugeInteger& other);    //�Ӳ��޸�����
    void sub(const hugeInteger& other);    //�����޸�����
    void mult(const hugeInteger& other);    //�˲��޸�����
    void div(const hugeInteger& other);    //�����޸�����
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
    int m_nDigit;   //λ��
    int m_nLength;  //���� >= λ��
    bool m_bPos;    //�����ű��
    static hugeInteger s_one;
};


ostream& operator << (ostream& out, hugeInteger& hi);
