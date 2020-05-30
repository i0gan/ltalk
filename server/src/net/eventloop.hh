#pragma once
#include <functional>
#include <vector>
#include <memory>

namespace Ltalk {
class EventLoop {
    typedef std::function<void()> CallBack;
public:
    EventLoop();
    ~EventLoop();
    void Loop();
    void Quit();
    void RunInLoop(CallBack &&fun);

};
}
