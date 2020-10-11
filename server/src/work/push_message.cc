 #include "push_message.hh"
extern MYSQL Database::global_mysql;

extern std::unordered_map<std::string, Data::User> Data::map_user;
extern std::unordered_map<std::string, Data::Group> Data::map_group;

std::queue<::Data::Message> Work::global_queue_messages;

Work::PushMessage::PushMessage() {
}

Work::PushMessage::~PushMessage() {

}

void Work::PushMessage::ToUser(const std::string &tid, const Json &json) {
    ::Data::Message msg;
    msg.tid = tid;
    msg.type = ::Data::Message::Type::USER;
    msg.content = json;
    msg.time = time(nullptr);
    global_queue_messages.push(msg);
}

void Work::PushMessage::ToGroup(std::string &gid, const Json &json) {

}

void Work::PushMessage::Send() {

    while (global_queue_messages.size() > 0) {
        ::Data::Message msg = global_queue_messages.front();
        global_queue_messages.pop();
        if(msg.type == ::Data::Message::Type::USER) {
            SendToUser(msg);
        } else if(msg.type == ::Data::Message::Type::GROUP) {
            SendToGroup(msg);
        }

    }
}

void Work::PushMessage::SendToUser(::Data::Message &msg) {
    //std::cout << "Send to User\n";
    std::ostringstream json_sstream;
    json_sstream << msg.content;
    std::string data = json_sstream.str();
    ::Data::User user = Data::map_user[msg.tid];
    //std::cout << "user: uid " << user.uid << "content[ " << data << " ]\n";
    ::Net::SPHttp linux_http   = user.linux_http;
    ::Net::SPHttp windows_http = user.windows_http;
    ::Net::SPHttp android_http = user.android_http;
    ::Net::SPHttp web_http     = user.web_http;

    if(linux_http != nullptr) {
        std::cout << "push _message: linux\n";
        linux_http->SendData(".json", data);
    }

    if(windows_http != nullptr) {
        std::cout << "push _message: windows\n";
        windows_http->SendData(".json", data);
    }

    if(android_http != nullptr) {
        std::cout << "push _message: android\n";
        android_http->SendData(".json", data);
    }

    if(web_http != nullptr) {
        std::cout << "push _message: web\n";
        web_http->SendData(".json", data);
    }
}

void Work::PushMessage::SendToGroup(::Data::Message &msg) {

}
