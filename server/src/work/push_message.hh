#include "../ltalk.hh"
#include "../net/epoll.hh"

using Json = Third::Json;

namespace Work {
class PushMessage {
public:
    PushMessage();
    ~PushMessage();
    static void ToUser(const std::string &uid, const Json &json);
    static void ToGroup(std::string &gid, const Json &json);
};
}

