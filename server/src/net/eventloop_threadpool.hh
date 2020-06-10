#pragma once

#include "../ltalk.hh"
#include "eventloop.hh"
#include "eventloop_thread.hh"

namespace Net {
class EventLoopThreadPool {
public:
    EventLoopThreadPool(EventLoop *base_eventloop, int number_of_thread);
    ~EventLoopThreadPool();
    void Start();
    EventLoop *get_next_eventloop();
private:
    bool started_;
    EventLoop *base_eventloop_;
    int number_of_thread_;
    int next_thread_indx_;
    std::vector<EventLoop *> v_eventloops_;
    std::vector<SPEventLoopThread> v_sp_eventloop_threads_;
};
}
