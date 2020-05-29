#pragma once
#include <memory>
#include <queue>
#include "../process/http.hh"
namespace Ltalk {
class Timer;
using SPTimer = std::shared_ptr<Timer>;

class Timer {
public:
    Timer();
    ~Timer();
    Timer(Timer &t);
    void Update(int timeout);
    bool IsValid();
    void ClearRequest();
    void set_delete();
    bool IsDelete();
    time_t GetExpiredTime();
private:
    bool deleted_;
    time_t expired_time_;
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
    TimerManager();
    ~TimerManager();
    void AddTimer(SPHttp sp_http, int timeout);
    void HandleExpired();
private:
    //std::priority_queue<SPTimer, std::deque<SPTimer>,

};
};
