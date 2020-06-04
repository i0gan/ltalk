#include "mutex_lock.hh"

Ltalk::MutexLock::MutexLock() {
    pthread_mutex_init(&mutex_, nullptr);
}

Ltalk::MutexLock::~MutexLock() {
    pthread_mutex_lock(&mutex_);
    pthread_mutex_destroy(&mutex_);
}

void Ltalk::MutexLock::Lock() {
    pthread_mutex_lock(&mutex_);
}

void Ltalk::MutexLock::Unlock() {
    pthread_mutex_unlock(&mutex_);
}

pthread_mutex_t *Ltalk::MutexLock::get_mutex() {
    return &mutex_;
}

Ltalk::MutexLockGuard::MutexLockGuard(MutexLock &mutex_lock) : mutex_lock_(mutex_lock){
    mutex_lock_.Lock();
}
Ltalk::MutexLockGuard::~MutexLockGuard(){
    mutex_lock_.Unlock();
}
