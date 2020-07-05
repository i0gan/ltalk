#pragma once
#include "../ltalk.hh"
#include "../database/mysql.hh"
#include "event.hh"
#include <vector>

namespace Work {
class EventLoop {
public:
    EventLoop();
    ~EventLoop();
    void Loop();
    void Quit();
    void AddWork(SPEvent sp_event);
private:
    bool quit_ = false;
    std::vector<SPEvent> v_sp_events_;
};
}
