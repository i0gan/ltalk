 #include "push_message.hh"
extern MYSQL Database::global_mysql;

extern std::unordered_map<std::string, Data::User> Data::map_user;
extern std::unordered_map<std::string, Data::Group> Data::map_group;
Work::PushMessage::PushMessage() {
}

Work::PushMessage::~PushMessage() {

}

void Work::PushMessage::ToUser(const std::string &uid, const Json &json) {
    ::Net::SPHttp http = Data::map_user[uid].web_http.lock();
    if(http == nullptr)
        return;

    std::ostringstream json_sstream;
    json_sstream << json;
    std::string data = json_sstream.str();
    http->SendData(".json", data);
}
void Work::PushMessage::ToGroup(std::string &gid, const Json &json) {

}
