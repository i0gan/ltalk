#pragma once

#include "../ltalk.hh"
#include "../database/mysql.hh"
#include <map>
#include <string>
#include <unistd.h>
#include <sstream>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unordered_map>
#include <pthread.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include "../crypto/md5.hh"
#include "util.hh"
#include "push_message.hh"

using Json = Third::Json;

class Work::Center {
public:
    explicit Center(const std::map<std::string, std::string> &map_header_info, std::string &content, std::string &http_uid, std::string &http_platform); // uid for deal with offline
    ~Center();
    void Process();
    void set_send_data_handler(Util::CallBack2 send_data_handler);
    void set_error_handler(Util::CallBack2 error_handler);
    void set_send_file_handler(Util::CallBack1 send_file_handler);
    void set_http(const ::Net::SPHttp &http);
    void set_fd(int fd) ;

    void HandleGet();
    void HandlePost();
    void HandlePut();
    void Offline();
private:
    const std::map<std::string, std::string> &map_header_info_;
    std::string &content_;
    Util::CallBack1 send_file_handler_;
    Util::CallBack2 send_data_handler_;
    Util::CallBack2 error_handler_;
    std::map<std::string, std::string> map_url_info_;
    std::map<std::string, std::string> map_url_value_info_;
    std::string request_;
    std::string platform_;
    std::string &http_uid_;
    std::string &http_platform_;
    std::weak_ptr<::Net::Http> wp_http_;
    int fd_;
    void SendData(const std::string &suffix, const std::string &content);
    void SendJson(Json &json_obj);
    void SendFile(std::string file_name);
    bool ParseUrl();
    void HandleNotFound();
    void Response(Work::ResponseCode error_code);
    /* */
    void DealWithRegisterUser();
    void GenerateUserPath(const std::string &uid);
    bool CheckJsonContentType(Json &recv_json_obj, const std::string &type);
    void DealWithUploadProfileImage(); //上传profile图片
    void DealWithGetProfileImage();    //获取profile图片
    void DealWithRegisterGroup();
    void DealWithLogin();
    bool CheckIsLogined(const std::string &uid);
    void DealWithGetUserInfo(); // GET method to get user info
    void DealWithGetUserPublicFile();
    void DealWithGetUserPrivateFile();
    void DealWithGetGetChatFile();
    void DealWithKeepConnect();
    void DealWithSearchUser();
    void DealWithAddUser();
    bool UpdateUserInfo(const std::string &uid, const std::string &token, const std::string &platform); // Update memory infomation

    bool CheckToken(const std::string &uid, const std::string &token);
    std::string StoreNewEvent(const std::string &cmd, const std::string &uid, const std::string &tid);
    void StoreMessage(const std::string &uid, const Json &json);
    std::string GetDateTime();
    std::string MakeToken(const std::string &uid);
    std::string MakeUid(const std::string &str);
};
