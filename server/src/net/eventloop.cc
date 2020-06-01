#include "eventloop.hh"
Ltalk::EventLoop::EventLoop() :
    looping_(false),
    awake_fd_(EventLoop::CreateEventFd()),
    quit_(false),
    event_handling_(false),
    thread_id_(CurrentThread::get_tid()),
    sp_epoll_(new Epoll())
{

}

Ltalk::EventLoop::~EventLoop() {

}

int Ltalk::EventLoop::CreateEventFd() {
    return 0;
}

void Ltalk::EventLoop::Loop() {

}
