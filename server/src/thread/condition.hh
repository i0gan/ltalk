#pragma once
#include <pthread.h>
#include <time.h>
#include <errno.h>

#include "../ltalk.hh"
#include "noncopyable.hh"
#include "mutex_lock.hh"
/*
 * Realize the synchronization mechanism through Condition class
 */

namespace Ltalk {
class Condition : Noncopyable {
  public:
    explicit Condition(MutexLock &mutex_lock);
    ~Condition();
    void Wait();
    void Signal();
    void Broadcast();
    bool WaitForSeconds(int seconds);
private:
    MutexLock &mutex_;
    pthread_cond_t cond_ = PTHREAD_COND_INITIALIZER;
};
}
