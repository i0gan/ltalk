#pragma once

#include <memory>
#include <functional>
#include <iostream>
#include <pthread.h>
#include <list>
#include <unordered_map>
#include "json/json.hpp"

#define DEFAULT_CONFIG_FILE "./etc/config.json"

#define d_cout std::cout << "[" << __FILE__ << " line: " << __LINE__ << " thread id: " << std::hex <<  pthread_self() << std::oct << "] "
#define MAX_CONNECTED_FDS_NUM 200000
#define EPOLL_MAX_EVENT_NUM  4096
#define EPOLL_WAIT_TIME 10000
#define MAX_BUF_SIZE 4096

#define SERVER_NAME    "LTALK SERVER DEBIAN LINUX x64"
//#define SERVER_DOMAIN  "http://ltalk.co"


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

namespace Work {
class Center;
enum class ResponseCode;
enum class RequestType;
enum class Platform;
class Event;
class EventLoop;
class EventLoopThread;
class Timer;
using SPEvent = std::shared_ptr<Work::Event>;
using SPTimer = std::shared_ptr<Work::Timer>;
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
struct Message {
    enum class Type {
        USER,
        GROUP
    };
    Message::Type type;
    Third::Json content;
    std::string tid;
    time_t time;
};

struct User{
public:
    std::string uid;
    std::string account;
    int linux_fd;
    int windows_fd;
    int android_fd;
    int web_fd;
    std::shared_ptr<::Net::Http> linux_http;
    std::shared_ptr<::Net::Http> windows_http;
    std::shared_ptr<::Net::Http> android_http;
    std::shared_ptr<::Net::Http> web_http;
    std::string linux_token;
    std::string windows_token;
    std::string android_token;
    std::string web_token;
};

struct Group {
public:
    std::string gid;
    std::string account;
    std::list<User> member_list;
};

extern std::unordered_map<std::string, User> map_user;
extern std::unordered_map<std::string, Group> map_group;
extern std::string web_root;
extern std::string web_page;
extern std::string web_404_page;
extern Work::EventLoop *work_eventloop;
}


namespace Ltalk {
class StartUp;
}




