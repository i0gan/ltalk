#pragma once
#include <functional>
#include <unordered_map>
#include <memory>

#include "eventloop.hh"
#include "../process/http.hh"
/*
 * This class is a event callback function to control read, write and new connect
 *
 */
class Channel;
typedef std::shared_ptr<Channel> SPChannel;

namespace Ltalk {
class Channel {
    typedef std::function<void()> CallBack;
public:
    Channel(EventLoop *eventloop);
    Channel(EventLoop *eventloop, int fd);
    ~Channel();
    void set_fd(int fd);
    int get_fd();
    void HandleRead();
    void HandleWrite();
    void HandleConnect(); // handle new connect

private:
    EventLoop *eventloop_ = nullptr;
    int fd_ = -1;
    __uint32_t evnets_ = 0;
    __uint32_t revents_ = 0;
    __uint32_t last_evnets = 0;
    std::weak_ptr<Ltalk::Http> holder_;
    CallBack read_handler_ = nullptr;
    CallBack write_handler_ = nullptr;
    CallBack error_handler_ = nullptr;
    CallBack connect_handler_ = nullptr;
};
};
