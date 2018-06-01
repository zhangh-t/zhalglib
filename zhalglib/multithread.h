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

//std提供的异步操作
class asyncoperation
{
public:
    asyncoperation() {}
    ~asyncoperation() {}
public:
    void execute();
};




//自旋锁
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