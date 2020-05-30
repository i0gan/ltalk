#pragma once

#include "../ltalk.hh"
#include "eventloop.hh"
namespace Ltalk {

class EventLoopThreadPool {
public:
    EventLoopThreadPool();
    ~EventLoopThreadPool();
    void Start();
    EventLoop *GetNextLoop();
private:
    EventLoop eventloop_;
    bool started_;
    int number_of_thread;
    int next_;
    //std::vector<SPE
    std::vector<EventLoop *> v_eventloops_;
};
}
