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
    if(connect_handler_) {
        connect_handler_();
    }
}
