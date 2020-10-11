#include "mutex_lock.hh"

Thread::MutexLock::MutexLock() {
    /*
     * 避免:  tpp.c:63: __pthread_tpp_change_priority:
     *  Assertion `new_prio == -1 || (new_prio >= __sched_fifo_min_prio && new_prio
     *      <=          __sched_fifo_max_prio)' failed.
     * 发现是报警模块初始化时， 调用pthread_mutex_lock卡死
     * 对锁进行初始化
     *
    */
    memset(&mutex_attr_, 0, sizeof(pthread_mutexattr_t));
    pthread_mutexattr_init(&mutex_attr_);
    pthread_mutexattr_settype(&mutex_attr_, PTHREAD_MUTEX_RECURSIVE_NP);
    pthread_mutex_init(&mutex_, &mutex_attr_);
}

Thread::MutexLock::~MutexLock() {
    // 线程调用该函数让互斥锁上锁，如果该互斥锁已被另一个线程锁定和拥有，则调用该线程将阻塞，直到该互斥锁变为可用为止
    pthread_mutex_lock(&mutex_);
    // 销毁互斥锁
    pthread_mutex_destroy(&mutex_);
}

void Thread::MutexLock::Lock() {
    pthread_mutex_lock(&mutex_);
}

void Thread::MutexLock::Unlock() {
    pthread_mutex_unlock(&mutex_);
}

pthread_mutex_t *Thread::MutexLock::get_mutex() {
    return &mutex_;
}


Thread::MutexLockGuard::MutexLockGuard(Thread::MutexLock &mutex_lock) : mutex_lock_(mutex_lock){
    mutex_lock_.Lock();
}
Thread::MutexLockGuard::~MutexLockGuard(){
    mutex_lock_.Unlock();
}
