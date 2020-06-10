#include "mutex_lock.hh"

Thread::MutexLock::MutexLock() {
    pthread_mutex_init(&mutex_, nullptr);
}

Thread::MutexLock::~MutexLock() {
    pthread_mutex_lock(&mutex_);
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
