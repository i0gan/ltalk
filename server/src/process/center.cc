#include "center.hh"

extern MYSQL Ltalk::global_mysql;

std::unordered_map<std::string, Ltalk::UserInfo> Ltalk::global_map_user_info;
std::unordered_map<std::string, Ltalk::GroupInfo> Ltalk::global_map_group_info;
std::string Ltalk::global_web_root;
std::string Ltalk::global_web_page;
std::string Ltalk::global_web_404_page;



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
        Response(ResponseCode::ERROR_PARSING_URL);
        return;
    }

    //std::cout << "method: "<< http_method << " url:[" << map_url_info_["url"] << "]\n";
    if(http_method == "get") {
        HandleGet();
    }else if(http_method == "put") {

    }else if(http_method == "post") {
        HandlePost();
    }
}

void Ltalk::Center::HandleUrlRequest(const std::string &request, const std::string &platform) {
    std::string path = global_web_root;
    if(request == "register" && platform == "web") {
        path += "/register/index.html";
        SendFile(path);
    }else if(request == "register_success" && platform == "web") {
        path += "/register/success/index.html";
        SendFile(path);
    }else if(request == "download" && platform == "web") {
        path += "/download/index.html";
        SendFile(path);
    }else {
        if(platform == "web") {
            HandleNotFound();
        }
    }
}

void Ltalk::Center::HandleGet() {
    bool error = false;
    std::string path = map_url_info_["path"];
    std::string platform;
    std::string request;
    try {
        platform = map_url_value_info_.at("platform");
        request = map_url_value_info_.at("request");
    }  catch (std::out_of_range e) { }

    do {
        if(path == "/" && request.empty()) {
            path = global_web_root + "/" + global_web_page;
            SendFile(path);
            break;
        }else if(path == "/" && !request.empty()){
            HandleUrlRequest(request, platform);
        }else {
            path = global_web_root + path;
            SendFile(path);
        }
    } while(false);
    // Send get file
    if(error) {
        std::cout << "error\n";
        HandleNotFound();
    }
}

void Ltalk::Center::HandlePost() {
    std::string platform;
    std::string request;
    try {
        platform = map_url_value_info_.at("platform");
        request = map_url_value_info_.at("request");
    }  catch (std::out_of_range e) {
        Response(ResponseCode::NO_ACCESS);
        return;
    }

    do {
        if(request == "register" && platform == "web") {
            DealWithRegisterUser();
        }else {
            Response(ResponseCode::NO_ACCESS);
            return;
        }
    } while(false);

}

bool Ltalk::Center::ParseUrl() {
    std::string url;
    std::string value_url;
    std::string path;
    try {
        url = map_header_info_.at("url");
    } catch (std::out_of_range e) {
        std::cout << "map_header_info_[url]" << e.what() << '\n';
        return false;
    }
    map_url_info_["url"] = url;
    int first_value_pos = url.find("?");
    if(first_value_pos > 0) {
        value_url = url.substr(first_value_pos + 1);
        path = url.substr(0, first_value_pos);
        if(path != "/" && path.size() > 0) {
            path.pop_back();
        }
    }else {
        path = url;
    }
    map_url_info_["path"] = path;
    // check is have value
    if(value_url.empty()) return true;
    value_url += '&';
    // Get value
    while(true) {
        int key_pos = value_url.find("&");
        if(key_pos < 0) {
            break;
        }
        std::string get_one = value_url.substr(0, key_pos);
        value_url = value_url.substr(key_pos + 1);
        if(get_one.empty()) continue;

        int value_pos = get_one.find('=');
        if(value_pos < 0) continue;
        std::string key = get_one.substr(0, value_pos);
        std::string value = get_one.substr(value_pos + 1);
        if(key.empty() || value.empty()) continue;
        map_url_value_info_[key] = value;
        //std::cout << "get_one: [" << get_one << "] key: [" << key << "] value: [" << map_url_value_info_[key] << "]\n";
    }
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

void Ltalk::Center::Response(ResponseCode code) {
    Json json_obj = {
        { "server", SERVER_NAME },
        { "code", code },
        { "datetime" , GetDateTime() }
    };
    SendJson(json_obj);
}

void Ltalk::Center::SendJson(Json json_obj) {
    std::ostringstream json_sstream;
    json_sstream << json_obj;
    std::string data = json_sstream.str();
    SendData(".json", data);
}

std::string Ltalk::Center::GetDateTime() {
    char time_str[128] = {0};
    struct timeval tv;
    time_t time;
    gettimeofday(&tv, nullptr);
    time = tv.tv_sec;
    struct tm *p_time = localtime(&time);
    strftime(time_str, 128, "%Y-%m-%d %H:%M:%S", p_time);
    return std::string(time_str);
}

bool Ltalk::Center::CheckJsonBaseContent(Json &json_obj) {
    bool ret = true;
    do {
        if(json_obj.find("token") == json_obj.end()) {
            ret = false;
            break;
        }
        if(json_obj.find("request") == json_obj.end()) {
            ret = false;
            break;
        }
        if(json_obj.find("datetime") == json_obj.end()) {
            ret = false;
            break;
        }
        if(json_obj.find("uid") == json_obj.end()) {
            ret = false;
            break;
        }
        if(json_obj.find("content") == json_obj.end()) {
            ret = false;
            break;
        }
        if(json_obj.find("content_type") == json_obj.end()) {
            ret = false;
            break;
        }
    } while(false);
    return ret;
}
bool Ltalk::Center::CheckJsonContentType(Json &recv_json_obj, const std::string &type) {
    bool ret = true;
    std::string recv_type;
    try {
        recv_type = recv_json_obj.at("content_type");
    } catch (std::out_of_range e) { }

    if(recv_type != type) {
        ret = false;
    }
    return ret;

}
void Ltalk::Center::DealWithRegisterUser() {
    std::string content_type;
    try {
        content_type = map_header_info_.at("content-type");
    } catch(std::out_of_range e) {
        std::cout << "no a content-type\n";
        Response(ResponseCode::ERROR_HTTP_CONTENT);
        return;
    }

    if(content_type != "application/json") {
        Response(ResponseCode::ERROR_HTTP_CONTENT);
        return;
    }

    Json recv_json_obj;
    try {
        recv_json_obj = Json::parse(content_);
    }  catch (Json::parse_error e) {
        Response(ResponseCode::ERROR_PARSING_CONTENT);
        return;
    }

    if(!CheckJsonBaseContent(recv_json_obj)) {
        Response(ResponseCode::NO_ACCESS);
        return;
    }

    if(!CheckJsonContentType(recv_json_obj, "register_info")) {
        Response(ResponseCode::ERROR_JSON_CONTENT_TYPE);
        return;
    }

    std::string json_name;
    std::string json_email;
    std::string json_phone_number;
    std::string json_address;
    std::string json_occupation;
    std::string json_password;

    //std::cout << "json [" << recv_json_obj << "]\n";
    try {
        json_name = recv_json_obj["content"]["name"];
        json_email = recv_json_obj["content"]["email"];
        json_phone_number = recv_json_obj["content"]["phone_number"];
        json_address = recv_json_obj["content"]["address"];
        json_occupation = recv_json_obj["content"]["occupation"];
        json_password = recv_json_obj["content"]["password"];
    } catch (Json::type_error e) {
        //std::cout << "Register type_error: " << e.what() << '\n';
        Response(ResponseCode::ERROR_JSON_CONTENT);
        return;
    }
    // Filter char
    MysqlQuery::Escape(json_name);
    MysqlQuery::Escape(json_email);
    MysqlQuery::Escape(json_phone_number);
    MysqlQuery::Escape(json_address);
    MysqlQuery::Escape(json_occupation);
    MysqlQuery::Escape(json_password);

    // check size
    std::cout << "name: [" << json_name << "]\n";
    std::cout << "email: [" << json_email << "]\n";
    std::cout << "phone_number: [" << json_phone_number << "]\n";
    std::cout << "address: [" << json_address << "]\n";
    std::cout << "occupation: [" << json_occupation << "]\n";
    std::cout << "password: [" << json_password << "]\n";

    bool is_valid = false;
    do {
        if(json_name.size() < 1 || json_name.size() >= 255)
            break;
        else if(json_email.size() < 4 || json_email.size() >= 255)
            break;
        else if(json_phone_number.size() < 4 || json_phone_number.size() >= 255)
            break;
        else if(json_occupation.size() >= 255)
            break;
        else if(json_password.size() < 6 || json_password.size() >= 255)
            break;
        else
            is_valid = true;
    } while(false);
    if(!is_valid) {
        Response(ResponseCode::ERROR_JSON_CONTENT);
        return;
    }

    // check is have this eamil
    MysqlQuery sql_query;
    sql_query.Select("user_", "email", "email = '" + json_email + '\'');
    if(sql_query.Next()) {
        if(sql_query.Value(0) != nullptr) {
            Response(ResponseCode::EXIST);
            return;
        }
    }

    std::string uid = MakeUid(json_email);
    // Insert into database
    std::string key_sql = "uid, email, name, phone_number, address, occupation, password";
    std::string value_sql = '\'' + uid + "',"; // set uid
    value_sql += '\'' + json_email + "',";
    value_sql += '\'' + json_name + "',";
    value_sql += '\'' + json_phone_number + "',";
    value_sql += '\'' + json_address + "',";
    value_sql += '\'' + json_occupation + "',";
    value_sql += '\'' + json_password + "'";
    if (!sql_query.Insert("user_", key_sql, value_sql)) {
        Response(ResponseCode::FAILURE);
        return;
    }

    Json json_obj = {
        { "server", SERVER_NAME },
        { "code", ResponseCode::SUCCESS },
        { "datetime" , GetDateTime() },
        { "access_url", "/?request=register_success&platform=web"},
        { "uid" , uid},
        { "token", MakeToken(uid) }
    };
    SendJson(json_obj);
}

std::string Ltalk::Center::MakeToken(std::string uid) {
    time_t t = time(nullptr);
    return Ltalk::Crypto::MD5(std::to_string(t) + uid).toString();
}

std::string Ltalk::Center::MakeUid(std::string str) {
    return Ltalk::Crypto::MD5(str).toString();
}

void Ltalk::Center::DealWithRegisterGroup() {

}
