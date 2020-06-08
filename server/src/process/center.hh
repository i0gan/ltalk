#pragma once

#include "../ltalk.hh"
#include "../db/mysql/mysql.hh"
#include <map>
#include <string>
#include "../json/json.hpp"
#include <unistd.h>
#include <sstream>
#include <time.h>
#include <sys/time.h>


namespace Ltalk {
enum class ResponseCode {
    SUCCESS = 0,
    FAILURE,
    NOT_FOUND,
    INTERNAL_ERROR,
    NO_ACCESS,
    NO_PERMISSSION,
    ERROR_PARSING_URL,
    ERROR_PARSING_CONTENT,
    ERROR_CONTENT_TYPE
};
class Center {
public:
    Center(const std::map<std::string, std::string> &map_header_info, std::string &content);
    ~Center();
    void Process();
    void set_send_data_handler(CallBack2 send_data_handler);
    void set_error_handler(CallBack2 error_handler);
    void set_send_file_handler(CallBack1 send_file_handler);

    void HandleGet();
    void HandlePost();
    void HandlePut();
    void HandleUrlRequest(const std::string &request, const std::string &platform);
private:
    const std::map<std::string, std::string> &map_header_info_;
    std::string &content_;
    CallBack1 send_file_handler_;
    CallBack2 send_data_handler_;
    CallBack2 error_handler_;
    std::map<std::string, std::string> map_url_info_;
    std::map<std::string, std::string> map_url_value_info_;
    void SendData(const std::string &suffix, const std::string &content);
    void SendJson(nlohmann::json json_obj);
    void SendFile(std::string file_name);
    bool ParseUrl();
    void HandleNotFound();
    void Response(ResponseCode error_code);
    /* */
    void DealWithRegisterUser();
    void DealWithRegisterGroup();
    void DealWithLogin();
    void DealWithSendUserInfo();
    bool CheckJsonContent(nlohmann::json &json_obj);

    std::string GetDateTime();
};
}
