#pragma once
#include <functional>
#include <unordered_map>
#include <memory>
#include <sys/epoll.h>

#include "eventloop.hh"
#include "../process/http.hh"
#include "../ltalk.hh"

/*
 * This class is a event callback function to control read, write and new connect
 *
 */

namespace Ltalk {
class Channel {
    typedef std::function<void()> CallBack;
public:
    Channel() {};
    Channel(EventLoop *eventloop);
    Channel(EventLoop *eventloop, int fd);
    ~Channel();
    void set_fd(int fd);
    int get_fd();
    SPHttp get_holder();
    void set_holder(SPHttp  sp_http);

    void HandleRead();
    void HandleWrite();
    void HandleConnect(); // handle new connect
    void HandleEvent();
    void HandleError();
    void set_revent(__uint32_t revent);
    void set_event(__uint32_t event);
    __uint32_t get_event();
    __uint32_t get_last_event();
    void UpdateLastEvnet();

private:
    EventLoop *eventloop_ = nullptr;
    int fd_ = -1;
    __uint32_t event_ = 0;
    __uint32_t revent_ = 0;
    __uint32_t last_event_ = 0;
    std::weak_ptr<Ltalk::Http> holder_;
    CallBack read_handler_ = nullptr;
    CallBack write_handler_ = nullptr;
    CallBack error_handler_ = nullptr;
    CallBack connect_handler_ = nullptr;
};
};
