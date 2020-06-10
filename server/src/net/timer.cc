#include "timer.hh"
Net::Timer::Timer(SPHttp sp_http, int ms_timeout) :
    deleted_(false),
    sp_http_(sp_http) {

    struct timeval time_now;
    gettimeofday(&time_now, nullptr);
    expired_ms_time_ = (((time_now.tv_sec % 10000) * 1000) + (time_now.tv_usec / 1000)) + ms_timeout;
}

Net::Timer::Timer(Timer &timer) :
    expired_ms_time_(0),
    sp_http_(timer.sp_http_) {
}


Net::Timer::~Timer() {
    if(sp_http_) {
        sp_http_->HandleClose();
    }
}

time_t Net::Timer::GetExpiredTime() {
    return expired_ms_time_;
}

void Net::Timer::Update(int ms_timeout) {
    struct timeval time_now;
    gettimeofday(&time_now, nullptr);
    expired_ms_time_ = (((time_now.tv_sec % 10000) * 1000) + (time_now.tv_usec / 1000)) + ms_timeout;
}

bool Net::Timer::IsValid() {
    struct timeval time_now;
    gettimeofday(&time_now, nullptr);
    time_t now_ms_time = (((time_now.tv_sec % 10000) * 1000) + (time_now.tv_usec / 1000));
    if(now_ms_time < expired_ms_time_) {
        return true;
    }else {
        return false;
    }
}

void Net::Timer::Clear() {
    sp_http_.reset();
    this->set_deleted(); //set as delete will deleted by NetTimerManager::HandleExpiredEvent
}

void Net::Timer::set_deleted() {
    deleted_ = true;
}

bool Net::Timer::IsDeleted() {
    return deleted_;
}

Net::TimerManager::TimerManager() {
}

Net::TimerManager::~TimerManager() {
}

void Net::TimerManager::AddTimer(std::shared_ptr<Net::Http> sp_http, int ms_timeout) {
    SPTimer sp_net_timer(new Net::Timer(sp_http, ms_timeout));
    sort_sp_timer_queue.push(sp_net_timer);
    sp_http->BindTimer(sp_net_timer); //set http to link timer
}

void Net::TimerManager::HandleExpiredEvent() {
    while(!sort_sp_timer_queue.empty()) {
        SPTimer sp_net_timer = sort_sp_timer_queue.top();
        if(sp_net_timer->IsDeleted()) {
            sort_sp_timer_queue.pop();
        }else if (!sp_net_timer->IsValid()) {
            sort_sp_timer_queue.pop();
        }else
            break;
    }
}

