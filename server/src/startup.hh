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

#include "net/net.hh"
#include "net/eventloop.hh"
#include "ltalk.hh"
#include "database/mysql.hh"

#include "work/eventloop_thread.hh"
#include "work/push_message.hh"

class Ltalk::StartUp final{
public:
     StartUp();
    ~StartUp();
    void ShowText();
    bool Run();
    bool Stop();
    bool LoadConfig();
    bool RunDatabaseModule();
    bool RunLoggerModule();
    bool RunNetworkModule();
    bool RunWorkModule();
    void StartBase();
    void KeepConnectDatabase();
private:
    int number_of_net_thread_;
    int queue_size_;
    int tcp_port_;
    int udp_port_;

    std::string db_host_;
    int         db_port_;
    std::string db_user_;
    std::string db_password_;
    std::string db_name_;
    std::string log_path_;

    std::shared_ptr<::Work::EventLoopThread> sp_work_eventloop_thread_;
    ::Work::PushMessage push_message_;
};
