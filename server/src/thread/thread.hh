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

namespace Thread {
namespace CurrentThread {
enum State{
    STARTING,
    RUNING,
    STOPED
};


// __thread 代表每个线程会分配一个独立的空间
extern __thread pid_t  tid;
extern __thread State  state;
extern __thread char *name;

// 该函数用于获取该线程的一个id值, 该值是内核中独一无二的值, 主要用于辨别不同进程中不同的线程
inline pid_t get_tid() {
/* #define likely(x) __builtin_expect(!!(x), 1)   //x likely as true
 * #define unlikely(x) __builtin_expect(!!(x), 0) //x likely as false
 */
    // 这个表示很有可能为假, 即在编译的时候告诉编译器, 若为假, 就不用跳转, 这是在汇编代码中体现的, 主要用于优化代码, 提高执行效率
    if(__builtin_expect(CurrentThread::tid == 0, 0)) {
        CurrentThread::tid = ::syscall(SYS_gettid);  //get real trhead id
    }
    return CurrentThread::tid;
}

}

/* 线程类
 */
class Thread : Noncopyable {
public:
    explicit Thread(const Util::CallBack &call_back, const std::string &name = std::string());
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
    Util::CallBack func_;
    std::string name_;
    CountDownLatch count_down_latch_;
};


class ThreadData {
public:
    ThreadData(const Util::CallBack &func, const std::string &name,
               pid_t *tid, CountDownLatch *count_down_latch);
    ~ThreadData();
    void Run();

private:
    Util::CallBack func_;
    std::string name_;
    pid_t *tid_;
    CountDownLatch *count_down_latch_;
};
}
