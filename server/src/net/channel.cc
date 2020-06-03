#include "channel.hh"

Ltalk::Channel::Channel(EventLoop *eventloop) {
    eventloop_ = eventloop;
}
Ltalk::Channel::Channel(EventLoop *eventloop, int fd) {
    eventloop_ = eventloop;
    fd_ = fd;
}

Ltalk::Channel::~Channel() {

}

void Ltalk::Channel::set_fd(int fd) {
    fd_ = fd;
}

int Ltalk::Channel::get_fd() {
    return fd_;
}

void Ltalk::Channel::set_holder(SPHttp sp_http) {
    holder_ = sp_http;
}

Ltalk::SPHttp Ltalk::Channel::get_holder() {
    SPHttp ret = holder_.lock();
    return ret;
}

void Ltalk::Channel::HandleRead() {
    if(read_handler_) {
        read_handler_();
    }
}
void Ltalk::Channel::HandleWrite() {
    if(write_handler_) {
        write_handler_();
    }
}

void Ltalk::Channel::HandleConnect() {
    if(connected_handler_) {
        connected_handler_();
    }
}
void Ltalk::Channel::HandleError() {
    if(error_handler_) {
        error_handler_();
    }
}

void Ltalk::Channel::HandleEvent() {
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

void Ltalk::Channel::set_revent(__uint32_t revent) {
    revent_ = revent;
}
void Ltalk::Channel::set_event(__uint32_t event) {
    event_ = event;
}

void Ltalk::Channel::set_read_handler(CallBack  &&read_handler) {
    read_handler_ = read_handler;
}

void Ltalk::Channel::set_write_handler(CallBack  &&write_handler) {
    write_handler_ = write_handler;
}

void Ltalk::Channel::set_error_handler(CallBack  &&error_handler) {
    error_handler_ = error_handler;
}

// For deal with connected client event
void Ltalk::Channel::set_connected_handler(CallBack  &&connected_handler) {
    connected_handler_ = connected_handler;
}


__uint32_t &Ltalk::Channel::get_event() {
    return event_;
}

__uint32_t Ltalk::Channel::get_last_event() {
    return last_event_;
}

void Ltalk::Channel::UpdateLastEvnet() {
    last_event_ = event_;
}

bool Ltalk::Channel::IsLastEvent() {
    return last_event_ == event_;
}

