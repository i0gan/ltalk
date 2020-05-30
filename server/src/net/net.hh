#pragma once
#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string>
#include <errno.h>
#include <cstring>

#include "../ltalk.hh"
#include "channel.hh"
#include "eventloop_threadpool.hh"
#include "util.hh"

namespace Ltalk {
class Net final{
public:
    Net();
    Net(int port,int thread_number, EventLoop *eventloop);
    ~Net();
    int Listen();
    void Start();
    void get_eventloop();
    void set_eventloop(EventLoop *eventloop);
    void HandleNewConnection();
    void HandleThisConnection();

private:
    bool is_started = false;
    bool is_listen_ = false;
    int port_;
    int thread_number_;
    EventLoop *eventloop_;
    int listen_fd;
    SPChannel accept_channel_;
    std::unique_ptr<EventLoopThreadPool> up_eventloop_threadpool_;
};
}
