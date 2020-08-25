#include "eventloop.hh"

Work::EventLoop::EventLoop() : quit_(false) {
    quit_ = false;
}

Work::EventLoop::~EventLoop() {

}
void Work::EventLoop::Loop() {
    while (!quit_) {
        for(auto iter = v_sp_events_.begin(); iter != v_sp_events_.end(); ++iter) {
            if(v_sp_events_.size() < 1) break;
            SPEvent sp_event = *iter;
            sp_event->Run();
            if(!sp_event->IsLoop()) {
                v_sp_events_.erase(iter);
            }
        }
    }
}


void Work::EventLoop::Quit() {
    quit_ = true;
}

void Work::EventLoop::AddWork(SPEvent event) {
    v_sp_events_.push_back(event);
}
