#include <memory>
namespace Ltalk {
class Channel;
class Http;
class Epoll;
class Channel;
class EventLoop;
class EventLoopThreadPool;

using SPChannel = std::shared_ptr<Channel> ;
using SPHttp = std::shared_ptr<Http>;

}


