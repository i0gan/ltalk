#include <memory>
#include <functional>
#include <iostream>
#include <pthread.h>

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
using SPChannel = std::shared_ptr<Channel>;
using SPHttp = std::shared_ptr<Http>;
using SPEpoll = std::shared_ptr<Epoll>;
using SPNetTimer = std::shared_ptr<NetTimer>;
using SPNetTimerManager = std::shared_ptr<NetTimerManager>;
using SPEventLoopThread = std::shared_ptr<EventLoopThread>;
}


