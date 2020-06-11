#pragma once
#include "noncopyable.hh"
#include <pthread.h>

namespace Thread {
class MutexLock : Noncopyable {
public:
    MutexLock();
    ~MutexLock();
    void Lock();
    void Unlock();
    pthread_mutex_t *get_mutex();
private:
    pthread_mutex_t mutex_ = PTHREAD_MUTEX_INITIALIZER;
    //Friend classes are not affected by access rights
    friend class Condition;
};

/*
 * MutexLockGuard 类确保线程实现同步机制
 */
class MutexLockGuard : Noncopyable {
public:
    explicit MutexLockGuard(MutexLock &mutex_lock);
    ~MutexLockGuard();
private:
    MutexLock &mutex_lock_;
};
}

