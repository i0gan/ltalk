#include "count_down_latch.hh"

Ltalk::CountDownLtach::CountDownLtach(int count) : count_(count), mutex_(), condition_(mutex_) {

}

Ltalk::CountDownLtach::~CountDownLtach() {

}

void Ltalk::CountDownLtach::Wait() {
    MutexLockGuard mutex_lock_guard(mutex_);
    while(count_ > 0) condition_.Wait();
}

void Ltalk::CountDownLtach::CountDown() {
    MutexLockGuard mutex_lock_guard(mutex_);
    --count_;
    if(count_ <= 0)  condition_.NotifyAll();
}
