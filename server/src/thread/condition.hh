#pragma once
#include <pthread.h>
#include <time.h>
#include <errno.h>

#include "noncopyable.hh"
#include "mutex_lock.hh"

namespace Ltalk {
class Condition : Noncopyable {
  public:
    explicit Condition(MutexLock &mutex_lock);
    ~Condition();
    void Wait();
    void Notify();
    void NotifyAll();
    bool WaitForSeconds(int seconds);
private:
    MutexLock &mutex_;
    pthread_cond_t cond_ = PTHREAD_COND_INITIALIZER;
};
}
