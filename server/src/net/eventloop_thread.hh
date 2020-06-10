#pragma once

#include "../ltalk.hh"
#include "eventloop.hh"
#include "../thread/thread.hh"
#include "../thread/mutex_lock.hh"
#include "../thread/condition.hh"

namespace Net {

class EventLoopThread {
public:
     explicit EventLoopThread();
    ~EventLoopThread();
    EventLoop *StartLoop();

private:
    EventLoop *eventloop_;
    bool exiting_;
    Thread::Thread thread_;
    Thread::MutexLock mutex_lock_;
    Thread::Condition condition_;
    void ThreadFunc();
};
}
