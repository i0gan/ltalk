#include "eventloop.hh"
__thread Ltalk::EventLoop * global_eventloop = nullptr;

Ltalk::EventLoop::EventLoop() :
    looping_(false),
    awake_fd_(EventLoop::CreateEventFd()),
    quit_(false),
    event_handling_(false),
    thread_id_(CurrentThread::get_tid()),
    sp_epoll_(new Epoll()),
    sp_awake_channel_(new Channel(this, awake_fd_))
{
    if(global_eventloop) {

    }else {
        global_eventloop = this;
    }
    sp_awake_channel_->set_event(EPOLLIN | EPOLLET);
    sp_awake_channel_->set_read_handler(std::bind(&EventLoop::HandleRead, this));
    sp_awake_channel_->set_connect_handler(std::bind(&EventLoop::HandleConnect, this));
    sp_epoll_->Add(sp_awake_channel_, 0);
}
Ltalk::EventLoop::~EventLoop() {

}

void Ltalk::EventLoop::HandleRead() {
    char buf[8];
    ssize_t read_len = Util::ReadData(awake_fd_, &buf, sizeof (buf));
    if(read_len != sizeof (buf)) {
        std::cout << "EventLoop::handRead() reads " << read_len << "instead of 8\n";
    }
    sp_awake_channel_->set_event(EPOLLIN | EPOLLET);
}

void Ltalk::EventLoop::HandleConnect() {

}

void Ltalk::EventLoop::UpdateEpoll(SPChannel sp_channel, int timeout) {
    sp_epoll_->Mod(sp_channel, timeout);
}
void Ltalk::EventLoop::AddToEpoll(SPChannel sp_channel, int timeout) {
    sp_epoll_->Add(sp_channel, timeout);
}


int Ltalk::EventLoop::CreateEventFd() {
    return 0;
}

void Ltalk::EventLoop::Loop() {

}
