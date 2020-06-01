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


namespace Ltalk {
class Epoll {
public:
    Epoll();
    ~Epoll();
    void Add(SPChannel sp_channel, int ms_timeout);
    void Mod(SPChannel sp_channel, int ms_timeout);
    void Del(SPChannel sp_channel);
    std::vector<SPChannel> Poll();
    std::vector<SPChannel> GetEventRequest(int event_num);
    void AddTimer(SPChannel sp_channel, int ms_timeout);
private:
    int epoll_fd_;
    std::vector<epoll_event> v_event_;
    SPChannel sp_channels_[MAX_CONNECTED_FDS_NUM];
    SPHttp sp_https_[MAX_CONNECTED_FDS_NUM]; // It's not https protocol
    SPNetTimerManager sp_net_timer_manager_;
};
}


