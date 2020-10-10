#pragma once

#include "../ltalk.hh"
#include <queue>
#include "../net/http.hh"
/*
 * Push messaage to client
 */


using Json = Third::Json;
namespace Work {
extern std::queue<::Data::Message> global_queue_messages;
class PushMessage {
public:
    PushMessage();
    ~PushMessage();
    static void ToUser(const std::string &uid, const Json &json);
    static void ToGroup(std::string &gid, const Json &json);
    void Send();
private:
    void SendToUser(::Data::Message &msg);
    void SendToGroup(::Data::Message &msg);
};
}

