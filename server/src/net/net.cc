#include "net.hh"

Ltalk::Net::Net() {
    //Ltalk::Channel a;
    //std::cout << "\nsssss\n";
}

Ltalk::Net::Net(int port,int thread_number, EventLoop *eventloop) :
    is_started(false),
    port_(port),
    thread_number_(thread_number),
    eventloop_(eventloop),
    listen_fd(Listen()),
    accept_channel_(new Channel(eventloop_)) {

    if(listen_fd == -1) {
        std::cout << "net init fail\n";
        abort();
    }

    accept_channel_->set_fd(listen_fd);
    IgnoreSigpipe();
    if(!SetFdNonBlocking(listen_fd)) {
        std::cout << "SetFdNonBlocking error\n";
        abort();
    }
}


Ltalk::Net::~Net() {

}

void Ltalk::Net::Start() {
    //up_eventloop_threadpool_->start();
}

int Ltalk::Net::Listen() {
    if(port_ < 0 || port_ > 65535) {
        std::cout << "listen port is not right\n";
        return -1;
    }

    int listen_fd = -1;
    if((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket: ");
        return -1;
    }
    // cancel bind show "Address already in use" err
    int optval = 1;
    if(setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1) {
        close(listen_fd);
        perror("setsockopt: ");
        return -1;
    }

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(static_cast<uint16_t>(port_));

    if(bind(listen_fd, (struct sockaddr *)&server_addr, sizeof (server_addr)) == -1) {
        close(listen_fd);
        perror("bind: ");
        return -1;
    }

    if(listen(listen_fd, SOMAXCONN) == -1) {
        close(listen_fd);
        perror("listen: ");
        return -1;
    }

    return listen_fd;
}
