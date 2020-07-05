 #include "eventloop.hh"

Work::EventLoop::EventLoop() {

}

Work::EventLoop::~EventLoop() {

}
void Work::EventLoop::Loop() {
    while (!quit_) {
        for(auto iter = v_sp_events_.begin(); iter != v_sp_events_.end(); ++iter) {
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
void Work::EventLoop::AddWork(SPEvent sp_event) {
    v_sp_events_.push_back(sp_event);
}
