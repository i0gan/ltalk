#pragma once
#include "../ltalk.hh"
#include "timer.hh"

namespace Work {
class Event {
public:
    explicit Event(::Util::CallBack call_back, bool loop = false, int ms_timeout = 0);
    ~Event();
    void Run();
    void set_call_back(::Util::CallBack call_back);
    void set_loop(bool loop) { loop_ = loop; };
    void set_timer(const Timer &timer) { timer_ = timer; }
    bool IsLoop() { return loop_; };
private:
    ::Util::CallBack call_back_ = nullptr;
    bool loop_;
    int ms_timeout_;
    Timer timer_;
};
}
