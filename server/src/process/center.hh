#pragma once

#include "../ltalk.hh"
#include "../db/mysql/mysql.hh"
#include <map>
#include <string>
#include "../json/json.hpp"
#include <unistd.h>


//#include <strstream>

namespace Ltalk {
class Center {
public:
    Center(const std::map<std::string, std::string> &map_header_info, std::string &content);
    ~Center();
    void Process();
    void set_send_data_handler(CallBack2 send_data_handler);
    void set_send_file_handler(CallBack1 send_file_handler);
    void HandleGet();
    void HandlePost();
    void HandlePut();

private:
    const std::map<std::string, std::string> &map_header_info_;
    std::string &content_;
    CallBack1 send_file_handler_;
    CallBack2 send_data_handler_;
    void SendData(const std::string &suffix, const std::string &content);
    void SendFile(std::string file_name);
    void Login(std::string);
    void Update(std::string);
};
}
