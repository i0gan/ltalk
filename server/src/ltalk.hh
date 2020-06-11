#pragma once

#include <memory>
#include <functional>
#include <iostream>
#include <pthread.h>
#include <list>
#include <unordered_map>
#include "json/json.hpp"


#define DEFAULT_CONFIG_FILE "./data/config.json"

#define d_cout std::cout << "[" << __FILE__ << " line: " << __LINE__ << " thread id: " << std::hex <<  pthread_self() << std::oct << "] "
#define MAX_CONNECTED_FDS_NUM 100000
#define EPOLL_MAX_EVENT_NUM  4096
#define EPOLL_WAIT_TIME 10000
#define MAX_BUF_SIZE 4096

#define SERVER_NAME "Ltalk Server Debian Linux x64"

namespace Thread {

class Thread;
class MutexLock;
class CountDownLatch;
class Condition;

}

namespace Net {
class Channel;
class Epoll;
class Channel;
class Timer;
class TimerCompare;
class TimerManager;
class EventLoop;
class EventLoopThread;
class EventLoopThreadPool;

enum class HttpRecvState;
enum class HttpConnectionState;
enum class HttpParseURIResult;
enum class HttpParseHeaderResult;
enum class HttpResponseCode;
class HttpContentType;
class Http;

using SPHttp = std::shared_ptr<Net::Http>;
using SPEpoll = std::shared_ptr<Net::Epoll>;
using SPTimer = std::shared_ptr<Net::Timer>;
using SPTimerManager = std::shared_ptr<Net::TimerManager>;
using SPEventLoopThread = std::shared_ptr<Net::EventLoopThread>;
using SPChannel = std::shared_ptr<Net::Channel>;
}

namespace Process {
class Center;
enum class ResponseCode;

}

namespace Crypto {
class MD5;
}


namespace Util {
using CallBack = std::function<void()>;
using CallBack1 = std::function<void(const std::string &)>;
using CallBack2 = std::function<void(const std::string &, const std::string &)>;
class Vessel;
}


namespace Database {
class Redis;
class Mysql;
class MysqlQuery;
class Mongo;
}

namespace Third {
using Json = nlohmann::json;
}


namespace Data {
class User{
public:
    void operator=(const User &user) {
        uid = user.uid;
        account = user.account;
        linux_fd = user.linux_fd;
        windows_fd = user.windows_fd;
        android_fd = user.android_fd;
        web_fd = user.web_fd;
        linux_token = user.linux_token;
        windows_token = user.windows_token;
        android_token = user.android_token;
        web_token = user.web_token;
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

class Group {
public:
    void operator=(const Group &group) {
        gid = group.gid;
        account = group.account;
        member_list = group.member_list;
    }

    std::string gid;
    std::string account;
    std::list<User> member_list;
};
extern std::unordered_map<std::string, User> map_user;
extern std::unordered_map<std::string, Group> map_group;
extern std::string web_root;
extern std::string web_page;
extern std::string web_404_page;
}


namespace Ltalk {
class StartUp;
}




