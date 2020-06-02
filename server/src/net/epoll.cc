#include "epoll.hh"

Ltalk::Epoll::Epoll() :
    epoll_fd_(epoll_create1(EPOLL_CLOEXEC)),
    v_events_(EPOLL_MAX_EVENT_NUM),
    sp_net_timer_manager_(new NetTimerManager) {
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
        sp_channel->UpdateLastEvnet(); // update last event
        struct epoll_event event;
        event.data.fd = fd;
        event.events = sp_channel->get_event();
        if(epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, fd, &event) < 0) {
            perror("epoll_ctrl [EPOLL_CTL_MOD] ");
            sp_channels_[fd].reset();
        }
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

// Get all events from epoll
std::vector<SPChannel> Ltalk::Epoll::GetAllEventChannels() {
    while (true) {
        int number_of_events =
                //get all events
                epoll_wait(epoll_fd_, &(*v_events_.begin()), v_events_.size(), EPOLL_WAIT_TIME);
        if(number_of_events < 0)
            perror("epoll_wait: ");

        std::vector<SPChannel> v_sp_channel_all_events = GetEventChannelsAfterGetEvents(number_of_events);
        if(v_sp_channel_all_events.size() > 0)
            return v_sp_channel_all_events;
    }
}
std::vector<SPChannel> Ltalk::Epoll::GetEventChannelsAfterGetEvents(int number_of_events) {
    std::vector<SPChannel> v_sp_event_channels;
    for (int idx = 0; idx < number_of_events; ++idx) {
        int fd = v_events_[idx].data.fd;
        SPChannel sp_single_event_channel = sp_channels_[fd];
        if(sp_single_event_channel != nullptr) {
            sp_single_event_channel->set_revent(v_events_[idx].events);
            sp_single_event_channel->set_event(0);

            v_sp_event_channels.push_back(sp_single_event_channel);
        }else {
            d_cout << "sp_channel is invalid\n";
        }
    }

    return v_sp_event_channels;
}

void Ltalk::Epoll::AddTimer(SPChannel sp_channel, int ms_timeout) {
    if(ms_timeout > 0) {
        sp_net_timer_manager_->AddTimer(sp_channel->get_holder(), ms_timeout);
    }else{
        d_cout << "add timer failed!\n";
    }
}

int Ltalk::Epoll::get_epoll_fd() {
    return epoll_fd_;
}

void Ltalk::Epoll::HandleExpiredEvent() {
    sp_net_timer_manager_->HandleExpiredEvent();
}
