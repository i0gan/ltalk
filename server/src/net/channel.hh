#pragma once
#include <functional>
#include <unordered_map>
#include <memory>

#include "eventloop.hh"
#include "../process/http.hh"

namespace Ltalk {

class Channel {
    typedef std::function<void()> CallBack;
public:
    Channel();
    ~Channel();
private:
    EventLoop *evnetloop_;
    int fd_;
    __uint32_t evnets_;
    __uint32_t revents_;
    __uint32_t last_evnets;
    std::weak_ptr<Ltalk::Http> holder_;
    CallBack read_handler_;
    CallBack write_handler_;
    CallBack error_handler_;
    CallBack connect_handler_;
};
};
