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
extern __thread pid_t  tid;
extern __thread char string[32];
extern __thread int string_length;
extern __thread const char *name;

inline pid_t get_tid() {
/* #define likely(x) __builtin_expect(!!(x), 1)   //x likely as true
 * #define unlikely(x) __builtin_expect(!!(x), 0) //x likely as false
 */
    if(__builtin_expect(CurrentThread::tid == 0, 0)) {

        CurrentThread::tid = ::syscall(SYS_gettid);  //get real trhead id
        CurrentThread::string_length = snprintf(CurrentThread::string, sizeof (CurrentThread::string),
                                                "%5d ", CurrentThread::tid);
        d_cout << "get tid as: " << CurrentThread::tid << '\n';
    }
    return CurrentThread::tid;
}

inline const char *get_string() {
    return CurrentThread::string;
}

inline int get_string_length() {
    return CurrentThread::string_length;
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
    const std::string & get_name();

private:
    static void *Run(void *arg);
    void SetDefaultName();
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
