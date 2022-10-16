#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>
#include <list>
#include "locker.h"
#include <exception>
#include <cstdio>

//线程池类，定义成模板类，为了代码的复用，模板参数T是任务类
template <typename T>
class threadpool{
public:
    threadpool(int thread_number = 8, int max_requests = 10000);
    ~threadpool();
    //添加任务
    bool append(T * request);

private:
    //子线程的工作函数，必须是静态成员函数，因为worker只能接收
    //一个参数，但是在类里面有隐藏参数this
    static void* worker(void *arg);
    void run();

private:
    // 线程的数量
    int m_thread_number;

    //线程池数组，大小为线程的数量m_thread_number
    pthread_t * m_threads;

    //请求队列中最多允许的，等待处理的请求数量
    int m_max_requests;

    //请求队列
    std::list< T*> m_workqueue;

    //互斥锁
    locker m_queuelocker;

    //定义信号量，用来判断是否有任务需要处理
    sem m_queuestat;

    //是否结束线程
    bool m_stop;

};

template<typename T>
threadpool< T >::threadpool(int thread_number, int max_requests) : 
    m_thread_number(thread_number), m_max_requests(max_requests),
    m_stop(false), m_threads(NULL){
        //线程数量和最大请求量不能为0
        if((thread_number <= 0) || (max_requests) <= 0){
            throw std::exception();
        }
         //初始化子线程数组
        m_threads = new pthread_t[m_thread_number];
        if(!m_threads){
            throw std::exception();
        }
        //创建thread_number个线程，并将它们设置为线程分离
        for(int i = 0; i < thread_number; i++){
            printf("create the %dth thread\n", i);

            if(pthread_create(m_threads + i, NULL, worker, this) != 0){
                delete [] m_threads;
                throw std::exception();
            }

            if(pthread_detach(m_threads[i]) != 0){
                delete [] m_threads;
                throw std::exception();
            }

        }

    }

template<typename T>
threadpool<T>::~threadpool(){
    delete[] m_threads;
    m_stop = true;
}

//主线程操作请求队列（添加)
template<typename T>
bool threadpool<T>::append(T* request){
    m_queuelocker.lock();
    if(m_workqueue.size() > m_max_requests){
        m_queuelocker.unlock();
        return false;
    }
    //主线程添加请求队列，此时其他线程不能操作队列 
    //向请求队列里增加一个
    m_workqueue.push_back(request);
    m_queuelocker.unlock();
    //信号量增加//通知子线程来任务了
    m_queuestat.post();
    return true;
}

template<typename T>
void* threadpool<T>::worker(void * arg){
    threadpool * pool = (threadpool *)arg;
    pool->run();
    return pool;
}

template<typename T>
void threadpool<T>::run(){
    while(!m_stop){
        m_queuestat.wait();//工作队列为空就等待挂起，工作队列不为空才能获取任务
        m_queuelocker.lock();//获取任务时，要使用互斥锁，保证对资源的独占式访问
        if(m_workqueue.empty()){
            m_queuelocker.unlock();
            continue;
        }

        T* request = m_workqueue.front(); //获取任务
        m_workqueue.pop_front();
        m_queuelocker.unlock();

        if(!request){
            continue;
        }
        //调用任务的工作 逻辑函数
        request->process();//执行任务（工作），这里不用锁，并发执行
    }
}


#endif
