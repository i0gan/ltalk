#include <memory>
#include <functional>

namespace Ltalk {
class Channel;
class Http;
class Epoll;
class Channel;
class EventLoop;
class EventLoopThreadPool;

using CallBack = std::function<void()> ;
using SPChannel = std::shared_ptr<Channel> ;
using SPHttp = std::shared_ptr<Http>;

}


