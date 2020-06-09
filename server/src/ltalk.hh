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

#define SERVER_NAME "Ltalk Server Debian Linux x64"

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


class UserInfo {
public:
    void operator=(const UserInfo &user_info) {
        uid = user_info.uid;
        account = user_info.account;
        linux_fd = user_info.linux_fd;
        windows_fd = user_info.windows_fd;
        android_fd = user_info.android_fd;
        web_fd = user_info.web_fd;
        linux_token = user_info.linux_token;
        windows_token = user_info.windows_token;
        android_token = user_info.android_token;
        web_token = user_info.web_token;
    }

    std::string uid;
    std::string account;
    int linux_fd;
    int windows_fd;
    int android_fd;
    int web_fd;
    std::string linux_token;
    std::string windows_token;
    std::string android_token;
    std::string web_token;
};

class GroupInfo {
public:
    void operator=(const GroupInfo &group_info) {
        gid = group_info.gid;
        account = group_info.account;
        member_list = group_info.member_list;
    }

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


