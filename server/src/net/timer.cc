#include "timer.hh"
Ltalk::NetTimer::NetTimer(SPHttp sp_http, int ms_timeout) :
    deleted_(false),
    sp_http_(sp_http) {

    struct timeval time_now;
    gettimeofday(&time_now, nullptr);
    expired_ms_time_ =
    (((time_now.tv_sec % 10000) * 1000) + (time_now.tv_usec / 1000)) + ms_timeout;
}

Ltalk::NetTimer::NetTimer(NetTimer &net_timer) :
    expired_ms_time_(0),
    sp_http_(net_timer.sp_http_) {
}


Ltalk::NetTimer::~NetTimer() {
    if(sp_http_) {
        sp_http_->HandleClose();
    }
}

time_t Ltalk::NetTimer::GetExpiredTime() {
    return expired_ms_time_;
}

void Ltalk::NetTimer::Update(int ms_timeout) {
    struct timeval time_now;
    gettimeofday(&time_now, NULL);
    expired_ms_time_ =
            (((time_now.tv_sec % 10000) * 1000) + (time_now.tv_usec / 1000)) + ms_timeout;
}

bool Ltalk::NetTimer::IsValid() {
    struct timeval time_now;
    gettimeofday(&time_now, NULL);
    time_t now_ms_time = (((time_now.tv_sec % 10000) * 1000) + (time_now.tv_usec / 1000));
    if(now_ms_time < expired_ms_time_) {
        return true;
    }else {
        this->set_deleted();
        return false;
    }
}

void Ltalk::NetTimer::Clear() {
    sp_http_.reset();
    this->set_deleted(); //set as delete will deleted by NetTimerManager::HandleExpiredEvent
}

void Ltalk::NetTimer::set_deleted() {
    deleted_ = true;
}

bool Ltalk::NetTimer::IsDeleted() {
    return deleted_;
}

Ltalk::NetTimerManager::NetTimerManager() {

}

Ltalk::NetTimerManager::~NetTimerManager() {

}

void Ltalk::NetTimerManager::AddTimer(std::shared_ptr<Http> sp_http, int ms_timeout) {
    SPNetTimer sp_net_timer(new NetTimer(sp_http, ms_timeout));
    sort_sp_timer_queue.push(sp_net_timer);
    sp_http->LinkTimer(sp_net_timer); //link to net timer
}

void Ltalk::NetTimerManager::HandleExpiredEvent() {
    while(!sort_sp_timer_queue.empty()) {
        SPNetTimer sp_net_timer = sort_sp_timer_queue.top();
        if(sp_net_timer->IsDeleted()) {
            sort_sp_timer_queue.pop();
        }else if (sp_net_timer->IsValid() == false) {
            sort_sp_timer_queue.pop();
        }else
            break;
    }
}

