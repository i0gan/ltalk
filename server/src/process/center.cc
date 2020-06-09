#include "center.hh"

extern MYSQL Ltalk::Db::global_mysql;

std::unordered_map<std::string, Ltalk::UserInfo> Ltalk::global_map_user_info;
std::unordered_map<std::string, Ltalk::GroupInfo> Ltalk::global_map_group_info;

std::string Ltalk::global_web_root;
std::string Ltalk::global_web_page;
std::string Ltalk::global_web_404_page;

Ltalk::Center::Center(const std::map<std::string, std::string> &map_header_info, std::string &content, std::string &http_uid, std::string &http_platform) :
    map_header_info_(map_header_info),
    content_(content),
    send_file_handler_(nullptr),
    send_data_handler_(nullptr),
    http_uid_(http_uid),
    http_platform_(http_platform) {

}

Ltalk::Center::~Center() {

}

void Ltalk::Center::set_fd(int fd) {
    fd_ = fd;
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
    } catch (std::out_of_range e) { std::cout << "no method\n"; }

    if(ParseUrl() == false) {
        Response(ResponseCode::ERROR_PARSING_URL);
        return;
    }

    try {
        platform_ = map_url_value_info_.at("platform");
        request_ = map_url_value_info_.at("request");
    } catch (std::out_of_range e) {}

    //std::cout << "method: "<< http_method << " url:[" << map_url_info_["url"] << "]\n";
    if(http_method == "get") {
        HandleGet();
    }else if(http_method == "put") {

    }else if(http_method == "post") {
        HandlePost();
    }
}

void Ltalk::Center::HandleWebRequest() {
    std::string path = global_web_root;
    if(request_ == "register") {
        path += "/register/index.html";
        SendFile(path);
    }else if(request_ == "register_success") {
        path += "/register/success/index.html";
        SendFile(path);
    }else if(request_ == "download") {
        path += "/download/index.html";
        SendFile(path);
    }else {
        HandleNotFound();
    }
}

void Ltalk::Center::HandleGet() {
    bool error = false;
    std::string path = map_url_info_["path"];
    do {
        if(path == "/" && platform_.empty()) {
            path = global_web_root + "/" + global_web_page;
            SendFile(path);
            break;
        }else if(platform_ == "web"){
            HandleWebRequest();
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
    do {
        if(request_ == "register" && platform_ == "web") {
            DealWithRegisterUser();
        }else if(request_ == "login" && !platform_.empty()) {
            DealWithLogin();
        }
        else {
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
        { "request", request_ },
        { "code", code },
        { "datetime" , GetDateTime() }
    };
    SendJson(json_obj);
}

void Ltalk::Center::SendJson(Json &json_obj) {
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
    Db::MysqlQuery::Escape(json_name);
    Db::MysqlQuery::Escape(json_email);
    Db::MysqlQuery::Escape(json_phone_number);
    Db::MysqlQuery::Escape(json_address);
    Db::MysqlQuery::Escape(json_occupation);

    // check size
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
    Db::MysqlQuery sql_query;
    sql_query.Select("user_", "email", "email = '" + json_email + '\'');
    if(sql_query.Next()) {
        Response(ResponseCode::EXIST);
        return;
    }

    std::string uid = MakeUid(json_email);
    std::string encode_password = Crypto::MD5(json_password).toString();

    // Insert into database
    std::string key_sql = "uid, account, email, name, phone_number, address, occupation, password";
    std::string value_sql = '\'' + uid + "',"; // set uid
    value_sql += '\'' + json_email + "',";     // init account as email
    value_sql += '\'' + json_email + "',";
    value_sql += '\'' + json_name + "',";
    value_sql += '\'' + json_phone_number + "',";
    value_sql += '\'' + json_address + "',";
    value_sql += '\'' + json_occupation + "',";
    value_sql += '\'' + encode_password + "'";
    if (!sql_query.Insert("user_", key_sql, value_sql)) {
        Response(ResponseCode::FAILURE);
        return;
    }

    Json send_json = {
        { "server", SERVER_NAME },
        { "code", ResponseCode::SUCCESS },
        { "datetime" , GetDateTime() },
        { "access_url", "/?request=register_success&platform=web"},
        { "uid" , uid},
        { "token", MakeToken(uid) }
    };

    SendJson(send_json);
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

void Ltalk::Center::DealWithLogin() {
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

    if(!CheckJsonContentType(recv_json_obj, "login_info")) {
        Response(ResponseCode::ERROR_JSON_CONTENT_TYPE);
        return;
    }

    std::string json_account;
    std::string json_password;
    try {
        json_account = recv_json_obj["content"]["account"];
        json_password = recv_json_obj["content"]["password"];
    }  catch (Json::type_error) {
        Response(ResponseCode::ERROR_JSON_CONTENT_TYPE);
        return;
    }

    // filter
    Db::MysqlQuery::Escape(json_account);
    Db::MysqlQuery sql_query;

    sql_query.Select("user_", "account", "account = '" + json_account + '\'');

    if(!sql_query.Next()) {
        Response(ResponseCode::NOT_EXIST);
        return;
    }

    sql_query.Select("user_", "password", "account = '" + json_account + '\'');
    if(!sql_query.Next()) {
        Response(ResponseCode::FAILURE);
        return;
    }

    std::string encode_password = Crypto::MD5(json_password).toString();
    std::string db_password = sql_query.Value(0);

    if(encode_password != db_password) {
        Response(ResponseCode::FAILURE);
        return;
    }

    sql_query.Select("user_", "uid", "account = '" + json_account + '\'');
    if(!sql_query.Next()) {
        Response(ResponseCode::FAILURE);
        return;
    }

    std::string db_uid = sql_query.Value(0);
    std::string token = MakeToken(db_uid);
    http_uid_ = db_uid;
    http_platform_ = platform_;


    if(CheckIsLogined(db_uid)) {
        Response(ResponseCode::LOGINED);
        return;
    }

    if(!UpdateUserInfo(db_uid, token)) {
        Response(ResponseCode::FAILURE);
        return;
    }

    Json send_json = {
        { "server", SERVER_NAME },
        { "code", ResponseCode::SUCCESS },
        { "request", request_},
        { "datetime" , GetDateTime() },
        { "access_url", "none"},
        { "uid" , db_uid},
        { "token",  token }
    };
    SendJson(send_json);
}
bool Ltalk::Center::CheckIsLogined(const std::string &uid) {
    UserInfo user_info;
    bool ret_result = false;
    do {
        if(global_map_user_info.find(uid) != global_map_user_info.end()) {
            user_info = global_map_user_info[uid];
        }else {
            std::cout << "not exited!\n";
            std::cout << "check uid[" << uid << "]\n";
            break;
        }

        if(platform_ == "linux" && user_info.linux_fd != -1) {
            ret_result =  true;
            break;
        }else if(platform_ == "windows" && user_info.windows_fd != -1) {
            ret_result =  true;
            break;
        }else if(platform_ == "android" && user_info.android_fd != - 1) {
            ret_result =  true;
            break;
        }else if(platform_ == "web" && user_info.web_fd != - 1) {
            ret_result =  true;
            break;
        }else {
            ret_result =  false;
            break;
        }
    } while(false);
    return ret_result;
}

bool Ltalk::Center::UpdateUserInfo(const std::string &uid, const std::string &token) {
    UserInfo user_info;
    user_info.linux_fd = -1;
    user_info.windows_fd = -1;
    user_info.android_fd = -1;
    user_info.web_fd = -1;
    user_info.uid = uid;
    std::cout << "update uid[" << uid << "]\n";
    if(global_map_user_info.find(uid) != global_map_user_info.end()) {
        user_info = global_map_user_info[uid];
    }

    if(platform_ == "linux") {
        user_info.linux_fd = fd_;
        user_info.linux_token = token;
    }else if(platform_ == "windows") {
        user_info.windows_fd = fd_;
        user_info.windows_token = token;
    }else if(platform_ == "android") {
        user_info.android_fd = fd_;
        user_info.android_token = token;
    }else if(platform_ == "web") {
        user_info.web_fd = fd_;
        user_info.web_token = token;
    }else {
        return false;
    }
    global_map_user_info[uid] = user_info;

    return true;
}
