#pragma once
#include <functional>
#include <vector>
#include <memory>

#include "../thread/thread.hh"
#include "epoll.hh"
#include "util.hh"
#include <sys/epoll.h>
#include <sys/eventfd.h>

namespace Net {
class EventLoop {
    typedef std::function<void()> CallBack;
public:
    EventLoop();
    ~EventLoop();
    int CreateEventFd();
    void Loop();
    void Quit();
    void RunInLoop(CallBack &&func);
    void PushBack(CallBack &&func);
    bool IsInLoopThread();              // 判断是否在事件循环的线程中
    void AssertInLoopThread();          // 在线程中断言
    void Shutdown(SPChannel sp_channel);                        // 关闭fd的写端
    void RemoveFromEpoll(SPChannel sp_channel);                 // 移除事件
    void UpdateEpoll(SPChannel sp_channel, int ms_timeout = 0); // 更新epoll事件
    void AddToEpoll(SPChannel sp_channel, int ms_timeout = 0);  // 添加epoll事件
private:
    bool looping_;
    int awake_fd_;
    bool quit_;
    bool event_handling_;
    const pid_t thread_id_;
    SPEpoll sp_epoll_;
    SPChannel sp_awake_channel_;           // 用于唤醒的Channel
    mutable Thread::MutexLock mutex_lock_; // 互斥锁
    std::vector<CallBack> pending_callback_functions_;
    bool calling_pending_callback_function_;

    void WakeUp(); //for write one byte to client
    void RunPendingCallBackFunc();
    void HandleRead();
    void HandleConnect();
};
}
