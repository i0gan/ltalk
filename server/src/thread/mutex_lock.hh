#pragma once
#include "noncopyable.hh"
#include <pthread.h>

namespace Thread {

// 互斥锁机制: 用来保证在任一时刻，只能有一个线程访问该对象

class MutexLock : Noncopyable {
public:
    MutexLock();
    ~MutexLock();
    void Lock(); // 给该
    void Unlock();
    pthread_mutex_t *get_mutex();
private:
    pthread_mutex_t mutex_ = PTHREAD_MUTEX_INITIALIZER;
    //Friend classes are not affected by access rights
    friend class Condition;
};

/*
 * MutexLockGuard 封装了 MutexLock, 使用更加方便
 *
 */
class MutexLockGuard : Noncopyable {
public:
    explicit MutexLockGuard(MutexLock &mutex_lock);
    ~MutexLockGuard();
private:
    MutexLock &mutex_lock_;
};
}

