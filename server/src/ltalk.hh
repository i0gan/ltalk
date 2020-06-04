#pragma once

#include <memory>
#include <functional>
#include <iostream>
#include <pthread.h>
#include <list>
#include <unordered_map>

#define DEFAULT_CONFIG_FILE "./data/config.json"

#define d_cout std::cout << "[" << __FILE__ << " line: " << __LINE__ << " thread id: " << std::hex <<  pthread_self() << std::oct << "] "
#define MAX_CONNECTED_FDS_NUM 100000
#define EPOLL_MAX_EVENT_NUM  4096
#define EPOLL_WAIT_TIME 10000
#define MAX_BUF_SIZE 4096

namespace Ltalk {
class StartUp;

class Channel;
class Http;
class Epoll;
class Channel;

class EventLoop;
class EventLoopThread;
class EventLoopThreadPool;

class NetTimer;
class NetTimerCompare;
class NetTimerManager;


using CallBack = std::function<void()>;
using CallBack1 = std::function<void(const std::string &)>;
using CallBack2 = std::function<void(const std::string &, const std::string &)>;


using SPChannel = std::shared_ptr<Channel>;
using SPHttp = std::shared_ptr<Http>;
using SPEpoll = std::shared_ptr<Epoll>;
using SPNetTimer = std::shared_ptr<NetTimer>;
using SPNetTimerManager = std::shared_ptr<NetTimerManager>;
using SPEventLoopThread = std::shared_ptr<EventLoopThread>;


struct UserInfo {
    std::string uid;
    std::string account;
    int fd;
    bool online;
};

struct GroupInfo {
    std::string gid;
    std::string account;
    std::list<UserInfo> member_list;
};

extern std::unordered_map<std::string, UserInfo> global_map_user_info;
extern std::unordered_map<std::string, GroupInfo> global_map_group_info;
extern std::string global_web_root;
extern std::string global_web_page;
extern std::string global_web_404_page;
}


