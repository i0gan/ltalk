#include "eventloop.hh"

Work::EventLoop::EventLoop() : quit_(false) {
    add_event_ = false;
}

Work::EventLoop::~EventLoop() {

}
void Work::EventLoop::Loop() {
    //Thread::MutexLockGuard mutex_lock_guard(mutex_lock_);
    while (!quit_) {
        if(v_sp_events_.size() < 1) continue;
        while (add_event_);
        handle_ = true;
        for(auto iter = v_sp_events_.begin(); iter != v_sp_events_.end(); ++iter) {
            if(v_sp_events_.size() < 1) break;;
            SPEvent sp_event = *iter;
            if(sp_event)
                sp_event->Run();
            else
                continue;

            if(!sp_event->IsLoop()) {
                v_sp_events_.erase(iter);
            }
        }
        handle_ = false;

    }
    std::cout << "quit work eventloop\n";
}


void Work::EventLoop::Quit() {
    quit_ = true;
}

void Work::EventLoop::AddWork(SPEvent event) {
    add_event_ = true;
    while (handle_);
    Thread::MutexLockGuard mutex_lock_guard(mutex_lock_);
    v_sp_events_.push_back(event);
    add_event_ = false;
}
