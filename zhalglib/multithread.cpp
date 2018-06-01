#include "stdafx.h"
#include "multithread.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <string>
#include <iostream>

std::mutex g_mutex;    //互斥量，但是不安全，异常退出无法解锁
std::condition_variable cv;
void threadProc(int threadTag) {
    for (int i = 0; i < 10000000; ++i) {
        char _out[20] = { 0 };
        sprintf(_out, "%d : %d", threadTag, i);
        std::lock_guard<std::mutex> lg(g_mutex);    //lock_guard可还行
        std::cout << std::string(_out) << std::endl;
    }
}

multithread::multithread()
{

}


multithread::~multithread()
{

}

void multithread::execute()
{
    //1. 构造了thread对象之后，线程直接就开始执行了
    //2. 如果退出之前线程还没执行完，会抛出异常
    std::thread thread1(threadProc, 1);
    std::thread thread2(threadProc, 2);
    //3. joint = wait 等待线程对象运行完
    thread1.join();
    thread2.join();
    //detach 线程proc和对象分离，也不会异常
    //thread1.detach(); thread2.detach();

    //bool的原子量
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
    flag.test_and_set();    //如果已经被置为true,返回true，否则设置为true并返回false
    flag.clear();    //置为false;

    //以下是一个原子操作
    std::atomic<int> a;
    a = 10;

    int expect, desire;
    //将a与expect比较，如果一样，将a修改为desire，并返回true， 否则将expect修改为a的值，返回false
    a.compare_exchange_strong(expect, desire);

    //花式thread
    //struct threadProc
    //{
    //    void operator () (int a) {
    //        //do sth
    //    }
    //};
    //std::thread t1(threadProc(), 1);
    //std::thread t2([](int a) {
    //     // do sth
    //}, 1);
    //std::this_thread的操作

    //std::this_thread::yield();    //让出当前线程剩下的时间片
    //std::this_thread::sleep_for();    //挂起多久
    //std::this_thread::sleep_until();    //挂起直到

}

spinmutex::spinmutex()
{
    m_flag = ATOMIC_VAR_INIT(false);
}

spinmutex::~spinmutex()
{
    m_flag.store(false);    //原子地以非原子对象替换值
}

void spinmutex::lock()
{
    //如果m_flag是false，则置为true，返回true，退出while
    //如果是true，返回false，并且expect
    bool expect = false;
    while (!m_flag.compare_exchange_strong(expect, true))
        expect = false;
}

void spinmutex::unlock()
{
    m_flag.store(false);
}

int asyncproc(int a) {
    return a + 1;
}

#include <cstringt.h>
void asyncoperation::execute()
{
    //创建线程+执行函数
    std::future<int> asyncRes = std::async(std::launch::async, asyncproc, 3);
    //等待异步对象执行结束，并返回结果
    int res = asyncRes.get();
    //future <-> promise 共享状态
    std::promise<int> pm;
    std::future<int> ft = pm.get_future();
    pm.set_value(10);
    int ten = ft.get();    //阻塞的，直到set了value了
    try {
        std::string().at(1);
    }
    catch (...) {
        try {
            pm.set_exception(std::current_exception());
        }
        catch (...) {

        }
    }
    
    try {
        int a = ft.get();
    }
    catch (...) {
        res = 3;
    }
}
