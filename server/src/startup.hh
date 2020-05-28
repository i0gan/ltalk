#pragma once
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>

#include "json/json.hpp"

#define d_cout(str) std::cout << __FILE__ << " : " << __LINE__ << " " << str << std::endl;

namespace Ltalk {
class StartUp final{
public:
     StartUp();
    ~StartUp();
    bool Init(int argv, char **argc);
    bool Run();
    bool Stop();
    bool LoadConfig();
    bool InitNetwork();
    bool InitDatabase();
    bool InitLog();

private:
    int thread_num_ = 4;
    int queue_size_ = 65535;
    int http_port_ = 8080;
    int udp_port_ = 8000;

    std::string db_host_;
    int         db_port_;
    std::string db_user_;
    std::string db_password_;
    std::string db_name_;

    std::string log_path_ = "/";
    std::string config_file_ = "./config";
};
};
