#pragma once

#include <memory>
#include <queue>
#include <deque>

#include <sys/time.h>
#include <unistd.h>
#include "../ltalk.hh"
#include "../process/http.hh"


using namespace Ltalk;

/*
 *  Network Timer Class set
 */

namespace Ltalk {

class NetTimer {
public:
    NetTimer(SPHttp sp_http, int ms_timeout);
    NetTimer(NetTimer &net_timer);
    ~NetTimer();

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

class NetTimerCompare {
public:
    bool operator()(SPNetTimer &sp_timer_1, SPNetTimer &sp_timer_2) const {
        return sp_timer_1->GetExpiredTime() > sp_timer_2->GetExpiredTime();
    }
};

class NetTimerManager {
public:
    explicit NetTimerManager();
    ~NetTimerManager();
    void AddTimer(std::shared_ptr<Http> sp_http, int ms_timeout);
    void HandleExpiredEvent();
private:
    std::priority_queue<SPNetTimer, std::deque<SPNetTimer>, NetTimerCompare>
    sort_sp_timer_queue;
};
}
