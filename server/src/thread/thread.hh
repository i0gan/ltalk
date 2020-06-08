#pragma once

#include <string>
#include <pthread.h>
#include <cstdio>
#include <cassert>
#include <unistd.h>
#include <syscall.h>
#include <sys/prctl.h>

#include "noncopyable.hh"
#include "count_down_latch.hh"
#include "../ltalk.hh"

namespace Ltalk {
namespace CurrentThread {

enum State{
    STARTING,
    RUNING,
    STOPED
};

extern __thread pid_t  tid;
extern __thread State  state;
extern __thread char *name;
inline pid_t get_tid() {
/* #define likely(x) __builtin_expect(!!(x), 1)   //x likely as true
 * #define unlikely(x) __builtin_expect(!!(x), 0) //x likely as false
 */
    if(__builtin_expect(CurrentThread::tid == 0, 0)) {
        CurrentThread::tid = ::syscall(SYS_gettid);  //get real trhead id
    }
    return CurrentThread::tid;
}
}

/* Thread class
 */
class Thread : Noncopyable {
public:
    explicit Thread(const CallBack &call_back, const std::string &name = std::string());
    ~Thread();
    void Start();
    int Join();
    bool IsStarted();
    pid_t get_tid;
    const std::string &get_name();
    void set_name(const std::string &name);

private:
    static void *Run(void *arg);
    bool started_;
    bool joined_;
    pthread_t pthread_id;
    pid_t tid_;
    CallBack func_;
    std::string name_;
    CountDownLatch count_down_latch_;
};


class ThreadData {
public:
    ThreadData(const CallBack &func, const std::string &name,
               pid_t *tid, CountDownLatch *count_down_latch);
    ~ThreadData();
    void Run();

private:
    CallBack func_;
    std::string name_;
    pid_t *tid_;
    CountDownLatch *count_down_latch_;
};
}
