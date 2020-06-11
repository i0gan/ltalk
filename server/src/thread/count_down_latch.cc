#include "count_down_latch.hh"

Thread::CountDownLatch::CountDownLatch(int count) :
    count_(count), mutex_(), condition_(mutex_) {
}

Thread::CountDownLatch::~CountDownLatch() {
}

    // 等待通知
void Thread::CountDownLatch::Wait() {
    MutexLockGuard mutex_lock_guard(mutex_);
    while(count_ > 0) condition_.Wait();
}


void Thread::CountDownLatch::CountDown() {
    MutexLockGuard mutex_lock_guard(mutex_);
    --count_;
    // 减到0的时候, 就通知所有线程, 为了释放所有线程的等待机制
    if(count_ <= 0)  condition_.Broadcast();
}
