#include "count_down_latch.hh"

Ltalk::CountDownLatch::CountDownLatch(int count) : count_(count), mutex_(), condition_(mutex_) {

}

Ltalk::CountDownLatch::~CountDownLatch() {

}

void Ltalk::CountDownLatch::Wait() {
    MutexLockGuard mutex_lock_guard(mutex_);
    while(count_ > 0) condition_.Wait();
}

void Ltalk::CountDownLatch::CountDown() {
    MutexLockGuard mutex_lock_guard(mutex_);
    --count_;
    if(count_ <= 0)  condition_.NotifyAll();
}
