#pragma once

#include "../ltalk.hh"
#include "eventloop.hh"
#include "../thread/thread.hh"
#include "../thread/mutex_lock.hh"
#include "../thread/condition.hh"

namespace Ltalk {

class EventLoopThread {
public:
     explicit EventLoopThread();
    ~EventLoopThread();
    EventLoop *StartLoop();

private:
    EventLoop *eventloop_;
    bool exiting_;
    Thread thread_;
    MutexLock mutex_lock_;
    Condition condition_;
    void ThreadFunc();
};
}
