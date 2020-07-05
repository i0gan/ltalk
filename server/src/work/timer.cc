 #include "timer.hh"
Work::Timer::Timer(int ms_timeout) {
    timeval time_now;
    gettimeofday(&time_now, nullptr);
    this->expired_ms_time_ = (((time_now.tv_sec % 10000) * 1000) + (time_now.tv_usec / 1000)) + ms_timeout;
}

Work::Timer::~Timer() {
}

void Work::Timer::Update(int ms_timeout) {
    struct timeval time_now;
    gettimeofday(&time_now, nullptr);
    this->expired_ms_time_ = (((time_now.tv_sec % 10000) * 1000) + (time_now.tv_usec / 1000)) + ms_timeout;
}

bool Work::Timer::Timeout() {
    struct timeval time_now;
    gettimeofday(&time_now, nullptr);
    time_t now_ms_time = (((time_now.tv_sec % 10000) * 1000) + (time_now.tv_usec / 1000));
    if(now_ms_time < this->expired_ms_time_) {
        return false;
    }else {
        return true;
    }
}
