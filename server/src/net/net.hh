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

#include "../process/http.hh"

namespace Ltalk {
class Net final{
public:
    Net(int port,int thread_number, EventLoop *eventloop);
    ~Net();
    void Start();
    EventLoop * get_eventloop();
    void set_eventloop(EventLoop *eventloop);
    void HandleNewConnection();
    void HandleThisConnection();

private:
    int Listen();
    bool started_;
    bool listened_;
    int port_;
    int nunber_of_thread_;
    EventLoop *eventloop_;
    int listen_fd;
    SPChannel accept_channel_;
    std::unique_ptr<EventLoopThreadPool> up_eventloop_threadpool_;
    int accept_fd_sum = 0;
};
}
