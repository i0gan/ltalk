#include "center.hh"

extern MYSQL *Ltalk::global_mysql_ptr;
extern std::unordered_map<std::string, Ltalk::UserInfo> Ltalk::global_map_user_info;
extern std::unordered_map<std::string, Ltalk::GroupInfo> Ltalk::global_map_group_info;
extern std::string Ltalk::global_web_root;
extern std::string Ltalk::global_web_page;

using json = nlohmann::json;

Ltalk::Center::Center(const std::map<std::string, std::string> &map_header_info, std::string &content) :
    map_header_info_(map_header_info),
    content_(content),
    send_file_handler_(nullptr),
    send_data_handler_(nullptr) {

}

Ltalk::Center::~Center() {

}

void Ltalk::Center::set_send_file_handler(CallBack1 send_file_handler) {
    send_file_handler_ = send_file_handler;
}

void Ltalk::Center::set_send_data_handler(CallBack2 send_data_handler) {
    send_data_handler_ = send_data_handler;
}


void Ltalk::Center::Process() {
    std::string http_method;
    try {
        http_method = map_header_info_.at("method");
    } catch (std::out_of_range e) {
        std::cout << e.what() << '\n';
    }

    if(ParseUrl() == false) {
        HandleNotFound();
    }

    if(http_method == "GET") {
        HandleGet();
    }else if(http_method == "PUT") {

    }else if(http_method == "POST") {

    }
}

void Ltalk::Center::HandleGet() {
    std::string path = map_url_info_["path"];
    bool error = false;
    bool send_file = false;
    //std::cout << "file_path:[" << file_path << "]\n";
    do {
        if(path == "/") {
            path = global_web_root + "/" + global_web_page;
            send_file = true;
            break;
        }else {
            path = global_web_root + path;
            send_file = true;
        }
    } while(false);

    // Send get file
    if(!error && send_file) {
        SendFile(path);
    }else {
        std::cout << "error\n";
    }
}

bool Ltalk::Center::ParseUrl() {
    std::string url;
    std::string value_url;

    try {
        url = map_header_info_.at("url");
    } catch (std::out_of_range e) {
        std::cout << "map_header_info_[url]" << e.what() << '\n';
        return false;
    }
    int value_pos = url.find("?");
    if(value_pos >= 0) {
        value_url = url.substr(value_pos + 1);
        map_url_info_["path"] = url.substr(0, value_pos);
    }else {
        map_url_info_["path"] = url;
    }

    //std::cout << "value url:" << value_url << '\n';
    return true;
}

void Ltalk::Center::SendFile(std::string file_name) {
    if(send_file_handler_)
        send_file_handler_(file_name);
}

void Ltalk::Center::SendData(const std::string &suffix, const std::string &content) {
    if(send_data_handler_)
        send_data_handler_(suffix, content);
}

void Ltalk::Center::HandleNotFound() {
    SendFile(global_web_404_page);
}
