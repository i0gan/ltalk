#pragma once

#include <memory>
#include <queue>
#include <deque>

#include <sys/time.h>
#include <unistd.h>
#include "../ltalk.hh"
#include "http.hh"

/*
 *  Network Timer Class set
 */

namespace Net {
class Timer {
public:
    Timer(SPHttp sp_http, int ms_timeout);
    Timer(Timer &timer);
    ~Timer();
    void Update(int ms_timeout);
    bool IsValid();
    void Clear();
    void set_deleted();
    bool IsDeleted();
    time_t GetExpiredTime();
private:
    bool deleted_;
    time_t expired_ms_time_;
    SPHttp sp_http_;
};

class TimerCompare {
public:
    bool operator()(SPTimer &sp_timer_1, SPTimer &sp_timer_2) const {
        return sp_timer_1->GetExpiredTime() > sp_timer_2->GetExpiredTime();
    }
};

class TimerManager {
public:
    explicit TimerManager();
    ~TimerManager();
    void AddTimer(std::shared_ptr<Http> sp_http, int ms_timeout);
    void HandleExpiredEvent();
private:
    std::priority_queue<SPTimer, std::deque<SPTimer>, TimerCompare>
    sort_sp_timer_queue;
};
}
