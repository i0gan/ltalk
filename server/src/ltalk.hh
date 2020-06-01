#include <memory>
#include <functional>

#define d_cout std::cout << __FILE__ << " | " << __LINE__ << " -> "
#define MAX_CONNECTED_FDS_NUM 100000

namespace Ltalk {
class Channel;
class Http;
class Epoll;
class Channel;
class EventLoop;
class EventLoopThreadPool;
class NetTimer;

using CallBack = std::function<void()>;
using SPChannel = std::shared_ptr<Channel>;
using SPHttp = std::shared_ptr<Http>;
using SPEpoll = std::shared_ptr<Epoll>;
using SPNetTimer = std::shared_ptr<NetTimer>;
}


