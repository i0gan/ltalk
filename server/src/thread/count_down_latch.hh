#pragma once
#include "noncopyable.hh"
#include "condition.hh"
#include "mutex_lock.hh"
/*
 * 这个类的主要功能是确保在主线程运行之前确保创建的线程启动成功
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
