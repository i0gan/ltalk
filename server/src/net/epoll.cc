#include "epoll.hh"

Ltalk::Epoll::Epoll() :
    epoll_fd_(epoll_create1(EPOLL_CLOEXEC)), v_event_(MAX_EVENT_NUM) {
    assert(epoll_fd_ > 0);
}

Ltalk::Epoll::~Epoll() {

}

void Ltalk::Epoll::Add(SPChannel sp_channel, int ms_timeout) {
    int fd = sp_channel->get_fd();
    if(ms_timeout > 0) { //If set timer will add a timer
        AddTimer(sp_channel, ms_timeout);
        sp_https_[fd] = sp_channel->get_holder();
    }
    struct epoll_event event;
    event.data.fd = fd;
    event.events = sp_channel->get_event();
    sp_channel->UpdateLastEvnet(); //update events

    sp_channels_[fd] = sp_channel;
    if(epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &event) < 0) {
        perror("epoll_ctrl [EPOLL_CTL_ADD] ");
        sp_channels_[fd].reset(); //reset sp_channel
    }
}

void Ltalk::Epoll::Mod(SPChannel sp_channel, int ms_timeout) {
    if(ms_timeout > 0)
        AddTimer(sp_channel, ms_timeout);

    int fd = sp_channel->get_fd();
    //If is not last event will be modified
    if(sp_channel->IsLastEvent() == false) {
        struct epoll_event event;
        event.data.fd = fd;
        event.events = sp_channel->get_event();
        if(epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, fd, &event) < 0) {
            perror("epoll_ctrl [EPOLL_CTL_MOD] ");
            sp_channels_[fd].reset();
        }
        sp_channel->UpdateLastEvnet(); // update last event
    }
}

void Ltalk::Epoll::Del(SPChannel sp_channel) {
    int fd = sp_channel->get_fd();
    struct epoll_event event;
    event.data.fd = fd;
    event.events = sp_channel->get_last_event();
    if(epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, &event) < 0) {
        perror("epoll_ctrl [EPOLL_CTL_DEL] ");
    }

    //clean event
    sp_channels_[fd].reset();
    sp_https_[fd].reset();
}

void Ltalk::Epoll::AddTimer(SPChannel sp_channel, int ms_timeout) {
    if(ms_timeout > 0) {
        sp_net_timer_manager_->AddTimer(sp_channel->get_holder(), ms_timeout);
    }else{
        d_cout << "add timer failed!\n";
    }
}
