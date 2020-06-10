#pragma once
#include "noncopyable.hh"
#include "condition.hh"
#include "mutex_lock.hh"
/*
 * The main function of CountDownLatch class is to ensure that the func passed
 * in the thread really starts before the start of the outer layer returns.
 */
namespace Thread {
class CountDownLatch : Noncopyable {
public:
    explicit CountDownLatch(int count);
    ~CountDownLatch();
    void Wait();
    void CountDown();
private:
    int count_;
    mutable MutexLock mutex_;
    Condition condition_;
};
}
