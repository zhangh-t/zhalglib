#include "stdafx.h"
#include "multithread.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <string>
#include <iostream>

std::mutex g_mutex;    //�����������ǲ���ȫ���쳣�˳��޷�����
std::condition_variable cv;
void threadProc(int threadTag) {
    for (int i = 0; i < 10000000; ++i) {
        char _out[20] = { 0 };
        sprintf(_out, "%d : %d", threadTag, i);
        std::lock_guard<std::mutex> lg(g_mutex);    //lock_guard�ɻ���
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
    //1. ������thread����֮���߳�ֱ�ӾͿ�ʼִ����
    //2. ����˳�֮ǰ�̻߳�ûִ���꣬���׳��쳣
    std::thread thread1(threadProc, 1);
    std::thread thread2(threadProc, 2);
    //3. joint = wait �ȴ��̶߳���������
    thread1.join();
    thread2.join();
    //detach �߳�proc�Ͷ�����룬Ҳ�����쳣
    //thread1.detach(); thread2.detach();

    //bool��ԭ����
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
    flag.test_and_set();    //����Ѿ�����Ϊtrue,����true����������Ϊtrue������false
    flag.clear();    //��Ϊfalse;

    //������һ��ԭ�Ӳ���
    std::atomic<int> a;
    a = 10;

    int expect, desire;
    //��a��expect�Ƚϣ����һ������a�޸�Ϊdesire��������true�� ����expect�޸�Ϊa��ֵ������false
    a.compare_exchange_strong(expect, desire);

    //��ʽthread
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
    //std::this_thread�Ĳ���

    //std::this_thread::yield();    //�ó���ǰ�߳�ʣ�µ�ʱ��Ƭ
    //std::this_thread::sleep_for();    //������
    //std::this_thread::sleep_until();    //����ֱ��

}

spinmutex::spinmutex()
{
    m_flag = ATOMIC_VAR_INIT(false);
}

spinmutex::~spinmutex()
{
    m_flag.store(false);    //ԭ�ӵ��Է�ԭ�Ӷ����滻ֵ
}

void spinmutex::lock()
{
    //���m_flag��false������Ϊtrue������true���˳�while
    //�����true������false������expect
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
    //�����߳�+ִ�к���
    std::future<int> asyncRes = std::async(std::launch::async, asyncproc, 3);
    //�ȴ��첽����ִ�н����������ؽ��
    int res = asyncRes.get();
    //future <-> promise ����״̬
    std::promise<int> pm;
    std::future<int> ft = pm.get_future();
    pm.set_value(10);
    int ten = ft.get();    //�����ģ�ֱ��set��value��
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
