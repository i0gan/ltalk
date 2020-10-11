#include "net.hh"

Net::Net::Net() :
    started_(false),
    listened_(false),
    base_eventloop_(new EventLoop()){

}

Net::Net::~Net() {
    delete  base_eventloop_;
}

void Net::Net::Start(int port, int number_of_thread) {

    port_ = port;
    number_of_thread_ = number_of_thread;
    listen_fd = Listen(),
    accept_channel_ =  SPChannel(new Channel(base_eventloop_));
    if(listen_fd == -1) {
        d_cout << "net init fail\n";
        abort();
    }

    listened_ = true;
    accept_channel_->set_fd(listen_fd);
    Util::IgnoreSigpipe();
    if(!Util::SetFdNonBlocking(listen_fd)) {
        d_cout << "SetFdNonBlocking error\n";
        abort();
    }

    up_eventloop_threadpool_ = std::unique_ptr<EventLoopThreadPool>(new EventLoopThreadPool(base_eventloop_, number_of_thread));
    up_eventloop_threadpool_->Start();
    accept_channel_->set_event(EPOLLIN | EPOLLET); // Set as accept data event
    accept_channel_->set_read_handler(std::bind(&Net::HandleNewConnection, this));
    accept_channel_->set_connected_handler(std::bind(&Net::HandleConnected, this));
    base_eventloop_->AddToEpoll(accept_channel_, 0);
    started_ = true;
    base_eventloop_->Loop();

}

int Net::Net::Listen() {
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
    return listen_fd;
}

void Net::Net::HandleNewConnection() {
    struct sockaddr_in client_sockaddr;
    bzero(&client_sockaddr, sizeof (client_sockaddr));
    socklen_t client_sockaddr_len = sizeof (client_sockaddr);
    int accept_fd = 0;
    while((accept_fd = accept(listen_fd, (struct sockaddr *)&client_sockaddr, &client_sockaddr_len)) > 0) {
        //d_cout << "new connection: " << inet_ntoa(client_sockaddr.sin_addr) << " : " << ntohs(client_sockaddr.sin_port);
        // If the number of accept fd is greater than MAX_CONNECTED_FDS_NUM wiil be closed
        if(accept_fd_sum > MAX_CONNECTED_FDS_NUM) {
            close(accept_fd);
            d_cout << "max_connect_fd refused to connect\n";
            continue;
        }
        if(!Util::SetFdNonBlocking(accept_fd)) {
            d_cout << "SetFdNonBlocking error\n";
        }

        //set as no delay
        Util::SetFdNoDelay(accept_fd);
        // add event to deal with
        //d_cout << "handle new connection\n";
        EventLoop *next_eventloop = up_eventloop_threadpool_->get_next_eventloop();
        SPHttp sp_http(new Http(accept_fd, next_eventloop));
        sp_http->get_sp_channel()->set_holder(sp_http);
        next_eventloop->PushBack(std::bind(&Http::NewEvnet, sp_http));
    }

    accept_channel_->set_event(EPOLLIN | EPOLLET);
}

void Net::Net::HandleConnected() {
    //d_cout << "HandleConnected\n";
    base_eventloop_->UpdateEpoll(accept_channel_);
}

bool Net::Net::Stop() {
    if(!started_)
        return false;
    up_eventloop_threadpool_->Stop();
    base_eventloop_->Stop();
    return true;

}
