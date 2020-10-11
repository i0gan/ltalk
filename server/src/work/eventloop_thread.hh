 #pragma once

#include "../thread/thread.hh"
#include "../thread/mutex_lock.hh"
#include "../thread/condition.hh"

#include "eventloop.hh"

namespace Work {
class EventLoopThread {
public:
    EventLoopThread();
    ~EventLoopThread();
    EventLoop *StartLoop();
    void Stop();
private:
    EventLoop *eventloop_;
    bool exiting_;
    Thread::Thread thread_;
    Thread::MutexLock mutex_lock_;
    Thread::Condition condition_;
    void ThreadFunc();
};
}
