#include "channel.hh"

Net::Channel::Channel(EventLoop *eventloop) {
    eventloop_ = eventloop;
}
Net::Channel::Channel(EventLoop *eventloop, int fd) {
    eventloop_ = eventloop;
    fd_ = fd;
}

Net::Channel::~Channel() {

}

void Net::Channel::set_fd(int fd) {
    fd_ = fd;
}

int Net::Channel::get_fd() {
    return fd_;
}

void Net::Channel::set_holder(SPHttp sp_http) {
    holder_ = sp_http;
}

Net::SPHttp Net::Channel::get_holder() {
    Net::SPHttp ret = holder_.lock();
    return ret;
}

void Net::Channel::HandleRead() {
    if(read_handler_) {
        read_handler_();
    }
}

void Net::Channel::HandleWrite() {
    if(write_handler_) {
        write_handler_();
    }
}

void Net::Channel::HandleConnect() {
    if(connected_handler_) {
        connected_handler_();
    }
}
void Net::Channel::HandleError() {
    if(error_handler_) {
        error_handler_();
    }
}

void Net::Channel::HandleEvent() {
    event_ = 0;
    if((revent_ & EPOLLHUP) && !(revent_ & EPOLLIN)) {
        event_ = 0;
        return;
    }
    if(revent_ & EPOLLERR) {
        if(error_handler_) HandleError();
        event_ = 0;
        return ;
    }
    if(revent_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)) {
        HandleRead();
    }
    if(revent_ & EPOLLOUT) {
        HandleWrite();
    }
    //*******
    HandleConnect();
}

void Net::Channel::set_revent(__uint32_t revent) {
    revent_ = revent;
}
void Net::Channel::set_event(__uint32_t event) {
    event_ = event;
}

void Net::Channel::set_read_handler(::Util::CallBack  &&read_handler) {
    read_handler_ = read_handler;
}

void Net::Channel::set_write_handler(::Util::CallBack  &&write_handler) {
    write_handler_ = write_handler;
}

void Net::Channel::set_error_handler(::Util::CallBack  &&error_handler) {
    error_handler_ = error_handler;
}

// For deal with connected client event
void Net::Channel::set_connected_handler(::Util::CallBack  &&connected_handler) {
    connected_handler_ = connected_handler;
}

__uint32_t &Net::Channel::get_event() {
    return event_;
}

__uint32_t Net::Channel::get_last_event() {
    return last_event_;
}

void Net::Channel::UpdateLastEvnet() {
    last_event_ = event_;
}

bool Net::Channel::IsLastEvent() {
    return last_event_ == event_;
}

