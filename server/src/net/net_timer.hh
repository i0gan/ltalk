#pragma once
#include <memory>
#include <queue>
#include <sys/time.h>
#include <unistd.h>
#include "../process/http.hh"

/*
 *  Network Timer Class set
 */

namespace Ltalk {
class NetTimer;
using SPNetTimer = std::shared_ptr<NetTimer>;

class NetTimer {
public:
    NetTimer();
    ~NetTimer();
    NetTimer(NetTimer &nt);
    void Update(int ms_timeout);
    bool IsValid();
    void ClearRequest();
    void set_deleted();
    bool is_deleted();
    time_t GetExpiredTime();
private:
    bool deleted_;
    time_t expired_ms_time_;
    SPHttp sp_http_;
};

class NetTimerCompare {
public:
    bool operator()(SPNetTimer &sp_timer_1, SPNetTimer &sp_timer_2) const {
        return sp_timer_1->GetExpiredTime() > sp_timer_2->GetExpiredTime();
    }
};

class NetTimerManager {
public:
    NetTimerManager();
    ~NetTimerManager();
    void AddTimer(SPHttp sp_http, int timeout);
    void HandleExpired();
private:
    //std::priority_queue<SPTimer, std::deque<SPTimer>,

};
}