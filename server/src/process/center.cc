#include "center.hh"

extern MYSQL Database::global_mysql;

std::unordered_map<std::string, Data::User> Data::map_user;
std::unordered_map<std::string, Data::Group> Data::map_group;
std::string Data::web_root;
std::string Data::web_page;
std::string Data::web_404_page;

Process::Center::Center(const std::map<std::string, std::string> &map_header_info, std::string &content, std::string &http_uid, std::string &http_platform) :
    map_header_info_(map_header_info),
    content_(content),
    send_file_handler_(nullptr),
    send_data_handler_(nullptr),
    http_uid_(http_uid),
    http_platform_(http_platform) {

}

Process::Center::~Center() {

}

void Process::Center::set_fd(int fd) {
    fd_ = fd;
}
void Process::Center::set_send_file_handler(Util::CallBack1 send_file_handler) {
    send_file_handler_ = send_file_handler;
}

void Process::Center::set_send_data_handler(Util::CallBack2 send_data_handler) {
    send_data_handler_ = send_data_handler;
}

void Process::Center::Process() {
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

void Process::Center::HandleGet() {
    bool error = false;
    std::string path = map_url_info_["path"];
    std::string web_path = Data::web_root;
    RequestType request_type = Request::toEnum(request_);
    do {
        if(path == "/" && platform_.empty()) {
            path = Data::web_root + "/" + Data::web_page;
            SendFile(path);
            break;
        }
        switch (request_type) {
        case RequestType::register_page: {
            web_path += "/register/index.html";
            SendFile(web_path);
        } break;
        case RequestType::register_success_page: {
            web_path += "/register/success/index.html";
            SendFile(web_path);
        } break;
        case RequestType::main_page: {
            web_path += "/main/index.html";
            SendFile(web_path);
        } break;
        case RequestType::keep_connect: {
            DealWithKeepConnect();
        }break;
        case RequestType::get_user_info: {
            DealWithGetUserInfo();
        } break;
        case RequestType::get_public_file: {
            DealWithGetUserPublicFile();
        } break;
        case RequestType::get_private_file: {
            DealWithGetUserPrivateFile();
        } break;
        case  RequestType::get_profile_image: {
            DealWithGetProfileImage();
        } break;
        case  RequestType::search_user: {
            DealWithSearchUser();
        } break;
        default: {
            path = Data::web_root + path;
            SendFile(path);
        } break;
        }
    } while(false);
    // Send get file
    if(error) {
        std::cout << "error\n";
        HandleNotFound();
    }
}

void Process::Center::HandlePost() {
    RequestType request_type = Request::toEnum(request_);
    //std::cout << "post: " << request_ << '\n';
    switch (request_type) {
    case RequestType::register_page: {
        if(platform_ == "web")
            DealWithRegisterUser();
    } break;
    case RequestType::login: {
        DealWithLogin();
    } break;
    case RequestType::upload_profile_image: {
        DealWithUploadProfileImage();
    } break;
    default: {
        Response(ResponseCode::NO_ACCESS);
    } break;
    }
}

bool Process::Center::ParseUrl() {
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

void Process::Center::SendFile(std::string file_name) {
    if(send_file_handler_)
        send_file_handler_(file_name);
}

void Process::Center::SendData(const std::string &suffix, const std::string &content) {
    if(send_data_handler_)
        send_data_handler_(suffix, content);
}

void Process::Center::HandleNotFound() {
    SendFile(Data::web_404_page);
}

void Process::Center::Response(ResponseCode code) {
    Json json_obj = {
        { "server", SERVER_NAME },
        { "request", request_ },
        { "code", code },
        { "datetime" , GetDateTime() }
    };
    SendJson(json_obj);
}

void Process::Center::SendJson(Json &json_obj) {
    std::ostringstream json_sstream;
    json_sstream << json_obj;
    std::string data = json_sstream.str();
    SendData(".json", data);
}

std::string Process::Center::GetDateTime() {
    char time_str[128] = {0};
    struct timeval tv;
    time_t time;
    gettimeofday(&tv, nullptr);
    time = tv.tv_sec;
    struct tm *p_time = localtime(&time);
    strftime(time_str, 128, "%Y-%m-%d %H:%M:%S", p_time);
    return std::string(time_str);
}

bool Process::Center::CheckJsonBaseContent(Json &json_obj) {
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

bool Process::Center::CheckJsonContentType(Json &recv_json_obj, const std::string &type) {
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

void Process::Center::DealWithKeepConnect() {
    Response(ResponseCode::SUCCESS);
}

void Process::Center::GenerateUserPath(const std::string &uid) {
    std::string base_path = "data/user/" + uid;
    mkdir(base_path.c_str(), S_IRWXU);
    mkdir((base_path + "/public").c_str(), S_IRWXU);
    mkdir((base_path + "/public/profile").c_str(), S_IRWXU);
    mkdir((base_path + "/public/files").c_str(), S_IRWXU);
    mkdir((base_path + "/private").c_str(), S_IRWXU);
    mkdir((base_path + "/private/chat").c_str(), S_IRWXU);
}

void Process::Center::DealWithRegisterUser() {
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
    Database::MysqlQuery::Escape(json_name);
    Database::MysqlQuery::Escape(json_email);
    Database::MysqlQuery::Escape(json_phone_number);
    Database::MysqlQuery::Escape(json_address);
    Database::MysqlQuery::Escape(json_occupation);

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
    Database::MysqlQuery sql_query;
    sql_query.Select("user_", "email", "email = '" + json_email + '\'');
    if(sql_query.Next()) {
        Response(ResponseCode::EXIST);
        return;
    }

    std::string uid = MakeUid(json_email);
    std::string encode_password = Crypto::MD5(json_password).toString();

    // Insert into database
    std::string key_sql = "uid, account, email, name, phone_number, address, occupation, created_time, password";
    std::string value_sql = '\'' + uid + "',"; // set uid
    value_sql += '\'' + json_email + "',";     // init account as email
    value_sql += '\'' + json_email + "',";
    value_sql += '\'' + json_name + "',";
    value_sql += '\'' + json_phone_number + "',";
    value_sql += '\'' + json_address + "',";
    value_sql += '\'' + json_occupation + "',";
    value_sql += '\'' + GetDateTime() + "',";
    value_sql += '\'' + encode_password + "'";
    if (!sql_query.Insert("user_", key_sql, value_sql)) {
        Response(ResponseCode::FAILURE);
        return;
    }
    Json send_json = {
        { "server", SERVER_NAME },
        { "code", ResponseCode::SUCCESS },
        { "datetime" , GetDateTime() },
        { "access_url", "/?request=register_success_page&platform=web"},
        { "uid" , uid},
        { "token", MakeToken(uid) }
    };

    GenerateUserPath(uid);
    SendJson(send_json);
}

std::string Process::Center::MakeToken(std::string uid) {
    time_t t = time(nullptr);
    return Crypto::MD5(std::to_string(t) + uid).toString();
}

std::string Process::Center::MakeUid(std::string str) {
    //::Database::MysqlQuery query;
    //query.Select("user_", "uid", "uid=" )
    return Crypto::MD5(str).toString();
}

void Process::Center::DealWithRegisterGroup() {

}

void Process::Center::DealWithLogin() {
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
    Database::MysqlQuery::Escape(json_account);
    Database::MysqlQuery sql_query;

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

    http_platform_ = platform_;


    if(CheckIsLogined(db_uid)) {
        Response(ResponseCode::LOGINED);
        return;
    }

    if(!UpdateUserInfo(db_uid, token)) {
        Response(ResponseCode::FAILURE);
        return;
    }
    sql_query.Update("user_", "last_login", GetDateTime(), "uid='" + db_uid + '\'');
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

bool Process::Center::CheckIsLogined(const std::string &uid) {
    Data::User user_info;
    bool ret_result = false;
    do {
        if(Data::map_user.find(uid) != Data::map_user.end()) {
            user_info = Data::map_user[uid];
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

bool Process::Center::UpdateUserInfo(const std::string &uid, const std::string &token) {
    http_uid_ = uid;
    Data::User user;
    user.linux_fd = -1;
    user.windows_fd = -1;
    user.android_fd = -1;
    user.web_fd = -1;
    user.uid = uid;
    //std::cout << "update uid[" << uid << "]\n";
    if(Data::map_user.find(uid) != Data::map_user.end()) {
        user = Data::map_user[uid];
    }

    if(platform_ == "linux") {
        user.linux_fd = fd_;
        user.linux_token = token;
    }else if(platform_ == "windows") {
        user.windows_fd = fd_;
        user.windows_token = token;
    }else if(platform_ == "android") {
        user.android_fd = fd_;
        user.android_token = token;
    }else if(platform_ == "web") {
        user.web_fd = fd_;
        user.web_token = token;
    }else {
        http_uid_ = "";
        return false;
    }
    Data::map_user[uid] = user;

    return true;
}

void Process::Center::DealWithGetUserInfo() {
    std::string account;
    std::string uid;
    std::string token;
    try {
        account = map_url_value_info_.at("account");
        uid = map_url_value_info_.at("uid");
        token = map_url_value_info_.at("token");
    }  catch (std::out_of_range e) {
        std::cout << "NO_access\n";
        Response(ResponseCode::NO_ACCESS);
        return;
    }

    if(!CheckToken(uid, token)) {
        Response(ResponseCode::FAILURE);
        return;
    }

    Database::MysqlQuery query;
    Database::MysqlQuery::Escape(account);
    query.Select("user_",
                 "account, email, nickname, name, phone_number, address, occupation, created_time, "
                 "network_state, last_login, head_image, profile_image_1, profile_image_2, profile_image_3, "
                 "profile_image_4"
                 ,"uid = '" + uid + '\'');
    if(!query.Next()) {
        Response(ResponseCode::FAILURE);
        return;
    }
    std::string db_account, db_email, db_nickname, db_name, db_phone_number
            , db_address, db_occupation, db_created_time, db_network_state, db_last_login,
            db_head_image, db_profile_image_1, db_profile_image_2, db_profile_image_3, db_profile_image_4;

    try {
        db_account = query.Value(0);
        db_email = query.Value(1);
        db_nickname = query.Value(2);
        db_name = query.Value(3);
        db_phone_number = query.Value(4);
        db_address = query.Value(5);
        db_occupation = query.Value(6);
        db_created_time = query.Value(7);
        db_network_state = query.Value(8);
        db_last_login = query.Value(9);
        db_head_image = query.Value(10);
        db_profile_image_1 = query.Value(11);
        db_profile_image_2 = query.Value(12);
        db_profile_image_3 = query.Value(13);
        db_profile_image_4 = query.Value(14);
    }  catch (Database::mysql_out_of_range e) {
        std::cout << e.what();
        Response(ResponseCode::FAILURE);
        return;
    }

    Json send_json = {
        { "server", SERVER_NAME },
        { "code", ResponseCode::SUCCESS },
        { "request", request_},
        { "datetime" , GetDateTime() },
        { "platform", platform_ },
        { "content-type", "user_info"},
        { "content", {
              {"account", db_account},
              {"email", db_email},
              {"nickname", db_nickname},
              {"name", db_name},
              {"phone_number", db_phone_number},
              {"address", db_address},
              {"occupation", db_occupation},
              {"created_time", db_created_time},
              {"network_state", db_network_state},
              {"last_login", db_last_login},
              {"head_image", db_head_image},
              {"profile_image_1", db_profile_image_1},
              {"profile_image_2", db_profile_image_2},
              {"profile_image_3", db_profile_image_3},
              {"profile_image_4", db_profile_image_4},
          }}
    };
    SendJson(send_json);
}

bool Process::Center::CheckToken(const std::string &uid, const std::string &token) {
    Data::User user;
    bool ret_value = false;
    if(Data::map_user.find(uid) != Data::map_user.end()) {
        user = Data::map_user[uid];
    }else {
        return false;
    }
    if(platform_ == "linux") {
        ret_value =  user.linux_token == token;
    } else if(platform_ == "windows"){
        ret_value =  user.windows_token == token;
    } else if(platform_ == "android") {
        ret_value =  user.android_token == token;
    } else if(platform_ == "web") {
        ret_value =  user.web_token == token;
    }else
        ret_value = false;

    return ret_value;
}

void Process::Center::DealWithGetUserPublicFile() {
    std::string uid;
    std::string file_name;
    try {
        uid = map_url_value_info_.at("uid");
        file_name = map_url_value_info_.at("file_name");

    }  catch (std::out_of_range e) {
        Response(ResponseCode::FAILURE);
        return;
    }
    std::string file_path = "data/user/" + uid + "/public/files/" + file_name;
    std::cout << "get public file [" << file_path << "]\n";
    SendFile(file_path);
}

void Process::Center::DealWithGetUserPrivateFile() {

}

void Process::Center::DealWithGetGetChatFile() {

}

void Process::Center::DealWithUploadProfileImage() {
    std::cout << "upload_profile_image\n";
    std::string name;
    std::string type;
    std::string account;
    std::string uid;
    std::string token;
    try {
        name = map_url_value_info_.at("name");
        account = map_url_value_info_.at("account");
        uid = map_url_value_info_.at("uid");
        token = map_url_value_info_.at("token");
        type = map_url_value_info_.at("type");
    } catch (std::out_of_range e) {
        Response(ResponseCode::FAILURE);
        std::cout << "lack var\n";
        return;
    }

    if(!CheckToken(uid, token)) {
        Response(ResponseCode::NO_ACCESS);
        std::cout << "check token error\n";
        return;
    }

    if(type == "head_image" || type == "profile_image_1" || type == "profile_image_2"
            || type == "profile_image_3" || type == "profile_image_4") {
    }else {
        Response(ResponseCode::FAILURE);
        std::cout << "file type error\n";
        return;
    }

    std::string save_image_path = "data/user/" + uid + "/public/profile/";

    if(!name.empty()) {
        save_image_path += name;
    } else {
        Response(ResponseCode::FAILURE);
        std::cout << "path: name empty\n";
        return;
    }
    std::cout << "path: " << save_image_path << '\n';

    int fd = open(save_image_path.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if(fd == -1) {
        Response(ResponseCode::FAILURE);
        return;
    }
    write(fd, content_.data(), content_.size());
    close(fd);
    std::string store_url = SERVER_DOMAIN;
                store_url += "/?request=get_profile_image&uid=" + uid + "&name=" + name;
    std::cout << "store_url: " << store_url << '\n';
    Database::MysqlQuery query;
    query.Update("user_", type, store_url, "uid='" + uid + '\'');
    Response(ResponseCode::SUCCESS);
}

void Process::Center::DealWithGetProfileImage() {
    std::string name;
    std::string uid;
    try {
        name = map_url_value_info_.at("name");
        uid = map_url_value_info_.at("uid");
    } catch (std::out_of_range e) {
        Response(ResponseCode::FAILURE);
        return;
    }
    std::string file_path = "data/user/" + uid + "/public/profile/" + name;
    SendFile(file_path);
}

void Process::Center::DealWithSearchUser() {
    std::string search;
    std::string type;

    std::string db_uid, db_account, db_nickname, db_head_image,
                db_address, db_network_state, db_email, db_occupation;

    try {
        search = map_url_value_info_.at("search");
        type = map_url_value_info_.at("type");
    } catch (std::out_of_range e) {
        Response(ResponseCode::FAILURE);
        return;
    }
    ::Database::MysqlQuery query;
    ::Database::MysqlQuery::Escape(search);

    if(type == "account") {
        query.Select("user_", "uid", "account='" + search + '\'');
        if(query.Next()) {
            db_uid = query.Value(0);
        }else {
            Response(ResponseCode::NOT_EXIST);
            return;
        }

        query.Select("user_", "account, email, nickname, occupation, head_image, address, network_state", "uid='" + db_uid + '\'');
        if(query.Next()) {
            try {
                db_account = query.Value(0);
                db_email = query.Value(1);
                db_nickname = query.Value(2);
                db_occupation = query.Value(3);
                db_head_image = query.Value(4);
                db_address = query.Value(5);
                db_network_state = query.Value(6);
            }  catch (::Database::mysql_out_of_range e) {
                std::cout << e.what() << '\n';
                Response(ResponseCode::FAILURE);
                return;
            }
        }else {
            Response(ResponseCode::FAILURE);
            return;
        }
    }

    //std::cout << "search " << search << " type " << type << "\n";
    Json send_json = {
        { "server", SERVER_NAME },
        { "code", ResponseCode::SUCCESS },
        { "request", request_},
        { "datetime" , GetDateTime() },
        { "platform", platform_ },
        { "content-type", "user_info"},
        { "content", {
              {"uid", db_uid},
              {"account", db_account},
              {"email", db_email},
              {"nickname", db_nickname},
              {"address", db_address},
              {"occupation", db_occupation},
              {"network_state", db_network_state},
              {"head_image", db_head_image},
          }}
    };
    SendJson(send_json);
}



















