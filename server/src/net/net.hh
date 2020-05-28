#pragma once
#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string>
#include <errno.h>
#include <cstring>

namespace Ltalk {
class Net {
public:
    Net();
    ~Net();

private:
    bool TcpNetListen(int port);
    bool is_listen_ = false;
    int  tcp_port_;
};
};
