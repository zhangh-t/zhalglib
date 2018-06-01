#include "StdAfx.h"
#include "hugeInteger.h"
#include <string>

hugeInteger hugeInteger::s_one = hugeInteger(1);
hugeInteger::hugeInteger(void)
{
    m_nDigit = 0;
    m_pData = new int(0);
    m_bPos = false;
    m_nLength = 1;
}

hugeInteger::hugeInteger(int nCount, int array[], bool bPos)
{
    m_pData = new int[nCount];
    for (int i = 0 ; i < nCount; ++i)
    {
        m_pData[i] = array[i] % 10;
    }
    m_nDigit = nCount;
    m_nLength = m_nDigit;
    m_bPos = bPos;
}

hugeInteger::hugeInteger(const hugeInteger& other)
{
    copy(other);
}

hugeInteger::hugeInteger(hugeInteger&& other)
{
    m_pData = other.m_pData;
    m_nDigit = other.m_nDigit;
    m_bPos = other.m_bPos;
    m_nLength = other.m_nLength;
    other.m_pData = nullptr;
    other.free();
}

hugeInteger::hugeInteger(int num)
{
    m_bPos = num > 0;
    m_pData = new int[20];
    m_nLength = 20;
    m_nDigit = 0;
    while (num != 0) {
        m_pData[m_nDigit] = num % 10;
        num /= 10;
        ++m_nDigit;
        if (m_nDigit >= m_nLength)
        {
            int* pTmp = new int[m_nLength * 2];
            memset(pTmp, 0, sizeof(int) * (m_nLength * 2));
            memcpy(pTmp, m_pData, m_nLength * sizeof(int));
            delete []m_pData;
            m_pData = pTmp;
            m_nLength *= 2;
        }
    }
}

hugeInteger::hugeInteger(int nCount, char nums[], bool bPos)
{
    m_pData = new int[nCount];
    m_nDigit = m_nLength = nCount;
    for (int i = 0 ; i < nCount; ++i)
    {
        m_pData[i] = (nums[i] - '0') % 10;
    }
    m_bPos = bPos;
}

hugeInteger::~hugeInteger(void)
{
    free();
}

void hugeInteger::free()
{
    if (m_pData)
    {
        delete []m_pData;
        m_pData = nullptr;
    }
    m_nDigit = 0;
    m_nLength = 0;
}

hugeInteger& hugeInteger::operator=(const hugeInteger& other)
{
    free();
    copy(other);
    return *this;
}

void hugeInteger::copy(const hugeInteger& other)
{
    m_pData = new int[other.m_nDigit];
    memcpy(m_pData, other.m_pData, other.m_nDigit * sizeof(int));
    m_nDigit = other.m_nDigit;
    m_bPos = other.m_bPos;
    m_nLength = other.m_nLength;
}

hugeInteger hugeInteger::operator+(const hugeInteger& other)
{
    hugeInteger res(*this);
    res.add(other);
    return res;
}

hugeInteger hugeInteger::operator-(const hugeInteger& other)
{
    hugeInteger res(*this);
    res.sub(other);
    return res;
}

hugeInteger hugeInteger::operator*(const hugeInteger& other)
{
    hugeInteger res(*this);
    res.mult(other);
    return res;
}

hugeInteger hugeInteger::operator/(const hugeInteger& other)
{
    hugeInteger res(*this);
    res.div(other);
    return res;
}

hugeInteger& hugeInteger::operator+=(const hugeInteger& other)
{
    this->add(other);
    return *this;
}

hugeInteger& hugeInteger::operator-=(const hugeInteger& other)
{
    this->sub(other);
    return *this;
}

hugeInteger& hugeInteger::operator*=(const hugeInteger& other)
{
    this->mult(other);
    return *this;
}

hugeInteger& hugeInteger::operator/=(const hugeInteger& other)
{
    this->div(other);
    return *this;
}

hugeInteger hugeInteger::operator--(int)
{
    hugeInteger res(*this);
    this->sub(s_one);
    return res;
}

hugeInteger& hugeInteger::operator--()
{
    this->sub(s_one);
    return *this;
}

hugeInteger hugeInteger::operator++(int)
{
    hugeInteger res(*this);
    this->add(s_one);
    return res;
}

hugeInteger& hugeInteger::operator++()
{
    this->add(s_one);
    return *this;
}

void hugeInteger::add(const hugeInteger& other)
{
    if (isZero())
    {
        *this = other;
    }
    else if (other.isZero())
    {
        return;
    }
    // 正 + 正
    // 正 + 负
    // 负 + 正
    // 负 + 负
    if (m_bPos ^ other.m_bPos)
    {
        if (absoluteSub(other))
        {
            m_bPos = !m_bPos;
        }
    }
    else 
    {
        absoluteAdd(other);
    }
}

void hugeInteger::sub(const hugeInteger& other)
{
    if (isZero())
    {
        *this = other;
        this->m_bPos = !this->m_bPos;
    }
    else if (other.isZero())
    {
        return;
    }
    // 正 - 正
    // 正 - 负
    // 负 - 正
    // 负 - 负
    if (m_bPos ^ other.m_bPos)
    {
        absoluteAdd(other);
    }
    else 
    {
        if (absoluteSub(other))
        {
            m_bPos = !m_bPos;
        }
    }
}

void hugeInteger::mult(const hugeInteger& other)
{
    if (isZero())
    {
        return;
    }
    else if (other.isZero())
    {
        free();
        *this = hugeInteger();
    }
    //d & c
    hugeInteger otherCopy(other);
    int nTimes1 = this->timesOfTen();
    int nTimes2 = otherCopy.timesOfTen();
    this->divTen(nTimes1);
    otherCopy.divTen(nTimes2);
    *this = divideAndConquer(m_pData, m_nDigit, m_nLength, otherCopy.m_pData, otherCopy.m_nDigit, otherCopy.m_nLength);
    this->multTen(nTimes1 + nTimes2);
    m_bPos = !(m_bPos ^ other.m_bPos);
}

void hugeInteger::div(const hugeInteger& other)
{
    if (isZero())
    {
        return;
    }
    else if (other.isZero())
    {
        throw "division by zero";
    }
    int nRelation = relation(other);
    if (-1 == nRelation)
    {
        //比除数小
        memset(m_pData, 0, sizeof(int) * m_nLength);
        m_nDigit = 0;
        m_bPos = false;
    }
    else if (0 == nRelation)
    {
        *this = s_one;
    }
    else
    {
        //给除数乘以10的倍数
        hugeInteger copyOfOther(other);
        int nDigitDis = this->m_nDigit - copyOfOther.m_nDigit;
        if (nDigitDis > 0)
        {
            copyOfOther.multTen(nDigitDis);
        }
        nRelation = relation(copyOfOther);
        if (0 == nRelation)
        {
            //10 的整数倍
            free();
            m_pData = new int[nDigitDis + 1];
            memset(m_pData, 0, sizeof(int) * (nDigitDis + 1));
            m_pData[nDigitDis] = 1;
            m_nLength = nDigitDis + 1;
            m_nDigit = m_nLength;
            return;
        }
        else if (-1 == nRelation)
        {
            nDigitDis -= 1;
            copyOfOther.divTen(1);
        }
        hugeInteger thisCopy(*this);
        int *pRes = new int[nDigitDis + 1];
        m_nLength = nDigitDis + 1;
        m_nDigit = m_nLength;
        memset(pRes, 0, sizeof(int) * (nDigitDis + 1));
        while (nDigitDis >= 0) {
            int nRela = thisCopy.relation(copyOfOther);
            if (nRela != -1)
            {
                thisCopy -= copyOfOther;
                pRes[nDigitDis]++;
                if (nRela == 0)
                {
                    break;
                }
            }
            else
            {
                --nDigitDis;
                copyOfOther.divTen(1);
            }
        }
        m_pData = pRes;

    }
    m_bPos = !(m_bPos ^ other.m_bPos);
}

void hugeInteger::absoluteAdd(const hugeInteger& other)
{
    int nLength = m_nDigit > other.m_nDigit ? m_nDigit : other.m_nDigit;
    int* pRes = new int[nLength + 1];
    memset(pRes, 0, sizeof(int) * (nLength + 1));
    for (int i = 0 ; i < nLength ; ++i)
    {
        if (i < m_nDigit)
        {
            pRes[i] += m_pData[i];
        }
        if (i < other.m_nDigit)
        {
            pRes[i] += other.m_pData[i];
        }
        if (pRes[i] >= 10)
        {
            int nIncreasement = pRes[i] / 10;
            pRes[i] = pRes[i] % 10;
            pRes[i + 1] += nIncreasement;
        }
    }
    free();
    m_pData = pRes;
    m_nDigit = m_pData[nLength] == 0 ? nLength : nLength + 1;
    m_nLength = nLength + 1;
}

bool hugeInteger::absoluteSub(const hugeInteger& other)
{
    //绝对相减法，总是大的减小的
    const hugeInteger* pBig = nullptr;
    const hugeInteger* pSmall = nullptr;
    int nRelation = relation(other);
    if (nRelation == 0)
    {
        *this = hugeInteger();
        m_bPos = false;
        return false;
    }
    bool nChangeSign = nRelation == -1;
    if (nChangeSign)
    {
        pBig = &other;
        pSmall = this; 
    }
    else
    {
        pBig = this;
        pSmall = &other;
    }

    int* pRes = new int[pBig->m_nDigit];
    memset(pRes, 0, sizeof(int) * pBig->m_nDigit);
    int nCarry = 0;
    for (int i = 0 ; i < pBig->m_nDigit; ++i)
    {
        int nSub = pBig->m_pData[i];
        nSub -= nCarry;
        if (i < pSmall->m_nDigit)
        {
            nSub -= pSmall->m_pData[i];
        }
        if (nSub < 0)
        {
            nCarry = 1;
            nSub += 10;
        }
        else
        {
            nCarry = 0;
        }
        pRes[i] = nSub;
    }
    int i = pBig->m_nDigit -1;
    for (; i >=0 ;--i)
    {
        if (pRes[i] != 0)
        {
            break;
        }
    }
    free();
    m_nDigit = i + 1;
    m_nLength = pBig->m_nDigit;
    m_pData = pRes;
    return nChangeSign;
}

int hugeInteger::relation(const hugeInteger& other)
{
    if (m_nDigit > other.m_nDigit)
    {
        return 1;
    }
    else if (m_nDigit == other.m_nDigit)
    {
        //从大往小比
        for (int i = 0 ; i < m_nDigit; ++i)
        {
            int nSub = m_pData[m_nDigit - 1 - i] - other.m_pData[m_nDigit - 1 - i];
            if (nSub != 0)
            {
                return nSub > 0 ? 1 : -1;
            }
        }
        return 0;
    }
    else
    {
        return -1;
    }
}

bool hugeInteger::isZero() const
{
    return m_nDigit == 0;
}

void hugeInteger::multTen(int nPow)
{
    if (nPow > 0 && !isZero())
    {
        //往右移动nPow位
        m_nLength += nPow;
        int* pTmp = new int[m_nLength];
        memset(pTmp, 0, sizeof(int) * m_nLength);
        memcpy(pTmp + nPow, m_pData, sizeof(int) * m_nDigit);
        delete []m_pData;
        m_pData = pTmp;
        m_nDigit += nPow;
    }
}

void hugeInteger::divTen(int nPow)
{
    if (nPow > 0 && !isZero())
    {
        if (nPow >= m_nDigit)
        {
            *this = hugeInteger();
        }
        else
        {
            m_nLength -= nPow;
            m_nDigit -= nPow;
            int* pTmp = new int[m_nLength];
            memset(pTmp, 0, sizeof(int) * m_nLength);
            memcpy(pTmp, m_pData + nPow, sizeof(int) * m_nDigit);
            delete []m_pData;
            m_pData = pTmp;
        }
    }
}

hugeInteger hugeInteger::divideAndConquer(int* data1, 
    int nDigit1, 
    int nLength1,
    int* data2,
    int nDigit2, 
    int nLength2)
{
    //两个非0结尾的整数乘法
    hugeInteger res;
    if ((data1 && data2)
        && (nDigit1 > 0 && nDigit2 > 0)
        && (nLength1 >= nDigit1
        && nLength2 >= nDigit2))
    {
        if (nDigit1 == 1 && nDigit2 == 1)
        {
            //两个个位数乘法
            int res = data1[0] * data2[0];
            int nDigit, nLength;
            nDigit = nLength = res > 10 ? 2 : 1;
            int* pRes = new int[nLength];
            if (res < 10)
            {
                pRes[0] = res;
            }
            else
            {
                pRes[0] = res %10;
                pRes[1] = res / 10;
            }
            return hugeInteger(nLength, pRes, true);
        }
        else
        {
            //分治法
            // X * Y = (A * 10 ^(x) + B) * (C * 10^(y) + D)
            //       = AC * 10^(x+y) + A * 10 ^(y) * D + B * C * 10^(x) + BD
            //       = AC * t1 + AD * t2 + BC * t3 + BD
            //拆成4份
            int* pPart11 = nullptr;   //B
            int * pPart12 = nullptr;  //A
            bool bFree1 = false;
            int nDigit11 = 0;    //x
            int nDigit12 = 0;
            int nLength11 = 0;
            int nLength12 = 0;
            if (nDigit1 > 1)
            {
                nDigit11 = nDigit1 / 2;
                nLength11 = nDigit11;
                nDigit12 = nDigit1 - nDigit11;
                nLength12 = nDigit12;
                pPart11 = new int[nDigit11];
                pPart12 = new int[nDigit12];
                memcpy(pPart11, data1, sizeof(int)* (nDigit11));
                memcpy(pPart12, data1 + nDigit11, sizeof(int) * nDigit12);
                bFree1 = true;
            }
            else
            {
                pPart12 = data1;
                nDigit12 = 1;
                nLength12 = 1;
            }
            int* pPart21 = nullptr;    //D
            int * pPart22 = nullptr;    //C
            int nDigit21 = 0;    //y
            int nDigit22 = 0;
            int nLength21 = 0;
            int nLength22 = 0;
            bool bFree2 = false;
            if (nDigit2 > 1)
            {
                bFree2 = true;
                nDigit21 = nDigit2 / 2;
                nLength21 = nDigit21;
                nDigit22 = nDigit2 - nDigit21;
                nLength22 = nDigit22;
                pPart21 = new int[nDigit21];
                pPart22 = new int[nDigit22];
                memcpy(pPart21, data2, sizeof(int)* (nDigit21));
                memcpy(pPart22, data2 + nDigit21, sizeof(int) * nDigit22);
            }
            else
            {
                pPart22 = data2;
                nDigit22 = 1;
                nLength22 = 1;
            }
            hugeInteger AC = divideAndConquer(pPart12, nDigit12, nLength12, pPart22, nDigit22, nLength22);
            int t1 = nDigit11 + nDigit21;
            hugeInteger AD = pPart21 == nullptr ? hugeInteger() : divideAndConquer(pPart12, nDigit12, nLength12, pPart21, nDigit21, nLength21);
            int t2 = nDigit11;
            hugeInteger BC = pPart11 == nullptr ? hugeInteger() : divideAndConquer(pPart22, nDigit22, nLength22, pPart11, nDigit11, nLength11);
            int t3 = nDigit21;
            hugeInteger BD = pPart11 == nullptr || pPart21 == nullptr ? hugeInteger() : divideAndConquer(pPart11, nDigit11, nLength11, pPart21, nDigit21, nLength21);
            AC.multTen(t1);
            AD.multTen(t2);
            BC.multTen(t3);
            if (bFree1)
            {
                delete []pPart11;
                delete []pPart12;
            }
            if (bFree2)
            {
                delete []pPart21;
                delete []pPart22;
            }
            return AC + AD + BC + BD;
        }
    }
    else
    {
        return hugeInteger();
    }
}

int hugeInteger::timesOfTen()
{
    int nRes = 0;
    for (int i = 0 ; i < m_nDigit; ++i)
    {
        if (m_pData[i] == 0)
        {
            ++nRes;
        }
        else
        {
            break;;
        }
    }
    return nRes;
}

ostream& operator<<(ostream& out, hugeInteger& hi)
{
    if (hi.isZero())
    {
        cout << 0;
    }
    else
    {
        if (hi.m_pData && hi.m_nLength > 0)
        {
            std::string str;
            if (!hi.isZero() && !hi.m_bPos)
            {
                out << "-";
            }
            bool bSkip = true;
            for (int i = hi.m_nDigit - 1 ; i >= 0; --i)
            {
                if (bSkip && hi.m_pData[i] == 0)
                {
                    continue;
                }
                bSkip = false;
                str += '0' + hi.m_pData[i];
            }
            out << str;
        }
    }

    return out;
}
