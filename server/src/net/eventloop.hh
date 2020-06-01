#pragma once
#include <functional>
#include <vector>
#include <memory>

#include "../thread/thread.hh"
#include "epoll.hh"
#include "util.hh"
namespace Ltalk {
class EventLoop {
    typedef std::function<void()> CallBack;
public:
    EventLoop();
    ~EventLoop();
    int CreateEventFd();
    void Loop();
    void Quit();
    void RunInLoop(CallBack &&func);
    void QueueInLoop(CallBack &&func);
    bool IsInLoopThread();
    void AssertInLoopThread();
    void Shutdown(SPChannel sp_channel);
    void RemoveFromEpoll(SPChannel sp_channel);
    void UpdateEpoll(SPChannel sp_channel, int timeout = 0);
    void AddToEpoll(SPChannel sp_channel, int timeout = 0);
private:
    bool looping_;
    int awake_fd_;
    bool quit_;
    bool event_handling_;
    const pid_t thread_id_;
    SPEpoll sp_epoll_;
    SPChannel sp_awake_channel_;
    mutable MutexLock mutex_lock_;
    std::vector<CallBack> pending_callbacks_;
    bool calling_pending_callback_;

    void WakeUp(); //for write one byte to client
    void RunPendingCallBack();
    void HandleRead();
    void HandleConnect();
};
}
