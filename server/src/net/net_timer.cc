#include "net_timer.hh"
Ltalk::NetTimer::NetTimer() {

}

Ltalk::NetTimer::~NetTimer() {

}

time_t Ltalk::NetTimer::GetExpiredTime() {
    return expired_ms_time_;
}

void Ltalk::NetTimer::Update(int ms_timeout) {
    struct timeval now_time;
    gettimeofday(&now_time, NULL);
    expired_ms_time_ =
            (((now_time.tv_sec % 10000) * 1000) + (now_time.tv_usec / 1000)) + ms_timeout;
}

bool Ltalk::NetTimer::IsValid() {
    struct timeval now_time;
    gettimeofday(&now_time, NULL);
    time_t now_ms_time = (((now_time.tv_sec % 10000) * 1000) + (now_time.tv_usec / 1000));
    if(now_ms_time < expired_ms_time_) {
        return true;
    }else {
        this->set_deleted();
        return false;
    }
}

void Ltalk::NetTimer::set_deleted() {
    deleted_ = true;
}

bool Ltalk::NetTimer::is_deleted() {
    return deleted_;
}



