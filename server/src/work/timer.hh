#pragma once
#include <time.h>
#include <sys/time.h>
#include <iostream>
#include "../ltalk.hh"

namespace Work {
class Timer {
public:
    Timer(int ms_timeout);
    ~Timer();
    bool Timeout();
    void Update(int ms_timeout);
private:
    time_t expired_ms_time_;
};
}
