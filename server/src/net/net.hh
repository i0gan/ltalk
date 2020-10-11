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
#include "http.hh"


namespace Net {
class Net final{
public:
    Net();
    ~Net();
    void Start(int port, int number_of_thread);
    void HandleNewConnection();
    void HandleConnected();
    bool Stop();
private:
    int Listen();    // Bind port_ and listen
    bool started_;   // Store state of net if started
    bool listened_;  // Store state of net if listend
    int port_;       // Listen port
    int number_of_thread_; // The number of thread
    EventLoop *base_eventloop_;
    int listen_fd;
    SPChannel accept_channel_;
    std::unique_ptr<EventLoopThreadPool> up_eventloop_threadpool_;
    int accept_fd_sum = 0;
};
}
