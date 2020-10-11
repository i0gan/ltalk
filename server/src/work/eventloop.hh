#pragma once
#include "../ltalk.hh"
#include "../database/mysql.hh"
#include "event.hh"
#include <vector>
#include <unistd.h>
#include <iterator>
#include <atomic>
#include "../thread/mutex_lock.hh"

namespace Work {
class EventLoop {
public:
    EventLoop();
    ~EventLoop();
    void Loop();
    void Quit();
    void AddWork(SPEvent event);
private:
    std::atomic<bool> quit_;
    std::vector<SPEvent> v_sp_events_;
    Thread::MutexLock mutex_lock_;
    std::atomic<bool> handle_;
    std::atomic<bool> add_event_;

};
}
