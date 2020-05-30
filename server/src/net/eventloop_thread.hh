#pragma once

#include "../ltalk.hh"
#include "eventloop.hh"

namespace Ltalk {
class EventLoopThread {
public:
    EventLoopThread();
    ~EventLoopThread();
    EventLoop *StartLoop();
private:
    void ThreadFunc();
    EventLoop *eventloop_;
    bool exiting_;

};
}
