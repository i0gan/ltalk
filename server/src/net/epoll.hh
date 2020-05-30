#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <iostream>
#include <cassert>
#include <string.h>
#include "channel.hh"
#include "timer.hh"
#include "../ltalk.hh"

#define EPOLL_MAX_FDS 100000

namespace Ltalk {
class Epoll {
public:
    Epoll();
    ~Epoll();
    void Add(SPChannel sp_channel, int timeout);
    void Modify(SPChannel sp_channel, int timeout);
    void Delete(SPChannel sp_channel);
    std::vector<SPChannel> Pull();
    std::vector<SPChannel> GetEventRequest(int event_num);
    void AddTimer(SPChannel sp_channel, int ms_timeout);
private:
    std::vector<epoll_event> v_event_;
    SPChannel sp_channels_[EPOLL_MAX_FDS];
    SPHttp sp_https_[EPOLL_MAX_FDS]; // It's not https protocol
    NetTimerManager net_timer_manager_;
};
}


