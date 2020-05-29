#pragma once
#include <memory>
#include <queue>
#include "../process/http.hh"
namespace Ltalk {
class Timer;
using SPTimer = std::shared_ptr<Timer>;

class Timer {

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
