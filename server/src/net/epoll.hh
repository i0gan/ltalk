#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>

#include <iostream>
#include <cassert>
#include <string.h>
#include "channel.hh"

#define EPOLL_MAX_FDS 100000
namespace Ltalk {
class Epoll {
public:
    Epoll();
    ~Epoll();
    void Add(SPChannel sp_channel, int timeout);
    void Modify(SPChannel sp_channel, int timeout);
    void Delete(SPChannel sp_channel);
private:
    std::vector<epoll_event> v_event_;
    SPChannel sp_channels_[EPOLL_MAX_FDS];
    SPHttp sp_https[EPOLL_MAX_FDS];

};
};


