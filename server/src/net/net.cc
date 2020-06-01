#include "net.hh"

Ltalk::Net::Net(int port,int thread_number, EventLoop *eventloop) :
    started(false),
    port_(port),
    thread_number_(thread_number),
    eventloop_(eventloop),
    listen_fd(Listen()),
    accept_channel_(new Channel(eventloop_)) {

    if(listen_fd == -1) {
        d_cout << "net init fail\n";
        abort();
    }

    accept_channel_->set_fd(listen_fd);
    Util::IgnoreSigpipe();
    if(!Util::SetFdNonBlocking(listen_fd)) {
        d_cout << "SetFdNonBlocking error\n";
        abort();
    }
}

Ltalk::Net::~Net() {

}

void Ltalk::Net::Start() {
    //up_eventloop_threadpool_->start();

}

Ltalk::EventLoop *Ltalk::Net::get_eventloop() {
    return eventloop_;
}

void Ltalk::Net::set_eventloop(EventLoop *eventloop) {
    eventloop_ = eventloop;
}

int Ltalk::Net::Listen() {
    if(port_ < 0 || port_ > 65535) {
        d_cout << "listen port is not right\n";
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
    listened = true;
    return listen_fd;
}

void Ltalk::Net::HandleNewConnection() {
    struct sockaddr_in client_sockaddr;
    bzero(&client_sockaddr, sizeof (client_sockaddr));
    socklen_t client_sockaddr_len = sizeof (client_sockaddr);
    int accept_fd = 0;
    while(true) {
        accept_fd = accept(listen_fd, (struct sockaddr *)&client_sockaddr, &client_sockaddr_len);
        if(accept_fd == -1) {
            perror("accept: ");
            return;
        }
        d_cout << "new connection: " << inet_ntoa(client_sockaddr.sin_addr) << " : " << ntohs(client_sockaddr.sin_port)
               << '\n';
        if(accept_fd_sum > MAX_CONNECTED_FDS_NUM) {
            close(accept_fd);
            d_cout << "max_connect_fd refuseed connect\n";
            continue;
        }
        if(!Util::SetFdNonBlocking(accept_fd)) {
            d_cout << "SetFdNonBlocking error\n";
        }
        //set as no delay
        Util::SetFdNoDelay(accept_fd);
        //SPHttp sp_http(new Http());

    }
}
