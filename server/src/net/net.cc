#include "net.hh"

Ltalk::Net::Net() {

}

Ltalk::Net::~Net() {

}

bool Ltalk::Net::TcpNetListen(int port) {
    if(port < 0 || port > 65535) {
        std::cout << "listen port is not right\n";
        return false;
    }

    int listen_fd = -1;
    if((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket: ");
        return false;
    }
    // cancel bind show "Address already in use" err
    bool optval = 1;
    if(setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(bool)) == -1) {
        close(listen_fd);
        perror("setsockopt: ");
        return false;
    }

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(static_cast<uint16_t>(tcp_port_));

    if(bind(listen_fd, (struct sockaddr *)&server_addr, sizeof (server_addr)) == -1) {
        close(listen_fd);
        perror("bind: ");
        return false;
    }

    if(listen(listen_fd, SOMAXCONN) == -1) {
        close(listen_fd);
        perror("listen: ");
        return false;
    }

    return true;
}
