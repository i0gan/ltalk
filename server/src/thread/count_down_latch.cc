#include "count_down_latch.hh"

Thread::CountDownLatch::CountDownLatch(int count) :
    count_(count), mutex_(), condition_(mutex_) {

}

Thread::CountDownLatch::~CountDownLatch() {

}

void Thread::CountDownLatch::Wait() {
    MutexLockGuard mutex_lock_guard(mutex_);
    while(count_ > 0) condition_.Wait();
}

/*
 * Decrements the number of latches. When the number reaches zero,
 * all waiting threads are released.
 * When the count is greater than zero, reduce the count
*/
void Thread::CountDownLatch::CountDown() {
    MutexLockGuard mutex_lock_guard(mutex_);
    --count_;
    if(count_ <= 0)  condition_.Broadcast();
}
