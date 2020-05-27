#ifndef STARTUP_H
#define STARTUP_H

#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/socket.h>

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
    bool load_config_();
    bool init_network_();
    bool init_database_();
    bool init_log_();

    int thread_num_ = 4;
    int queue_size_ = 65535;
    int http_port_ = 80;
    std::string path_ = "/";

};

#endif
