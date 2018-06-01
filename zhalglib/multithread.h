#pragma once
#include <atomic>
class multithread
{
public:
    multithread();
    ~multithread();
public:
    void execute();
};

//std�ṩ���첽����
class asyncoperation
{
public:
    asyncoperation() {}
    ~asyncoperation() {}
public:
    void execute();
};




//������
class spinmutex
{
public:
    spinmutex();
    virtual ~spinmutex();
public:
    void lock();
    void unlock();
private:
    std::atomic<bool> m_flag;
};