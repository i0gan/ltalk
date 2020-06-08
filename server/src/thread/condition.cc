#include "condition.hh"

Ltalk::Condition::Condition(MutexLock &mutex_lock) : mutex_(mutex_lock) {
    pthread_cond_init(&cond_, nullptr);
}
Ltalk::Condition::~Condition() {
    pthread_cond_destroy(&cond_);
}

void Ltalk::Condition::Wait() {
    pthread_cond_wait(&cond_, mutex_.get_mutex());

}

void Ltalk::Condition::Signal() {
    pthread_cond_signal(&cond_);
}

void Ltalk::Condition::Broadcast() {
    pthread_cond_broadcast(&cond_);
}

bool Ltalk::Condition::WaitForSeconds(int seconds) {
    struct timespec time_spec;
    clock_gettime(CLOCK_REALTIME, &time_spec);
    time_spec.tv_sec += static_cast<__time_t>(seconds);
    return ETIMEDOUT == pthread_cond_timedwait(&cond_, mutex_.get_mutex(), &time_spec);
}


