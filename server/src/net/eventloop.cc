#include "eventloop.hh"
__thread Ltalk::EventLoop * global_eventloop = nullptr;

Ltalk::EventLoop::EventLoop() :
    looping_(false),
    awake_fd_(EventLoop::CreateEventFd()),
    quit_(false),
    event_handling_(false),
    thread_id_(CurrentThread::get_tid()),
    sp_epoll_(new Epoll()),
    sp_awake_channel_(new Channel(this, awake_fd_))
{
    if(global_eventloop) {

    }else {
        global_eventloop = this;
    }
    sp_awake_channel_->set_event(EPOLLIN | EPOLLET);
    sp_awake_channel_->set_read_handler(std::bind(&EventLoop::HandleRead, this));
    sp_awake_channel_->set_connect_handler(std::bind(&EventLoop::HandleConnect, this));
    sp_epoll_->Add(sp_awake_channel_, 0);
}
Ltalk::EventLoop::~EventLoop() {

}

void Ltalk::EventLoop::HandleRead() {
    char buf[8];
    ssize_t read_len = Util::ReadData(awake_fd_, &buf, sizeof (buf));
    if(read_len != sizeof (buf)) {
        std::cout << "EventLoop::handRead() reads " << read_len << "instead of 8\n";
    }
    sp_awake_channel_->set_event(EPOLLIN | EPOLLET);
}

void Ltalk::EventLoop::HandleConnect() {

}

void Ltalk::EventLoop::UpdateEpoll(SPChannel sp_channel, int ms_timeout) {
    sp_epoll_->Mod(sp_channel, ms_timeout);
}
void Ltalk::EventLoop::AddToEpoll(SPChannel sp_channel, int ms_timeout) {
    sp_epoll_->Add(sp_channel, ms_timeout);
}


int Ltalk::EventLoop::CreateEventFd() {
    return 0;
}

void Ltalk::EventLoop::Loop() {
    assert(looping_ == false);
    assert(IsInLoopThread());
    looping_ = true;
    quit_ = false;
    std::vector<SPChannel> v_sp_event_channels;
    while(!quit_) {
        v_sp_event_channels.clear();
        v_sp_event_channels = sp_epoll_->GetAllEventChannels(); // get all event

        event_handling_ = true;
        for (auto &sp_channel : v_sp_event_channels) {
            sp_channel->HandleEvent(); // Handel event
        }
        event_handling_  = false;
        RunPendingCallBackFunc(); // Run pending callback function
        //sp_epoll
    }
}

void Ltalk::EventLoop::Quit() {
    quit_ = true;
    if(IsInLoopThread() == false) {
        WakeUp();
    }
}

void Ltalk::EventLoop::QueueInLoop(CallBack &&func) {
    MutexLockGuard mutex_lock_guard(mutex_lock_);
    pending_callback_functions_.emplace_back(std::move(func));
    if(!IsInLoopThread() || calling_pending_callback_function_)
        WakeUp();
}

bool Ltalk::EventLoop::IsInLoopThread() {
    return thread_id_ == CurrentThread::get_tid();
}

void Ltalk::EventLoop::RemoveFromEpoll(SPChannel sp_channel) {
    sp_epoll_->Del(sp_channel);
}

void Ltalk::EventLoop::WakeUp() {
    char buf[8];
    ssize_t write_len = Util::WriteData(awake_fd_, buf, sizeof (buf));
    if(write_len != sizeof (buf)) {
        d_cout << "eventloop::wakeup write:" << write_len << " instead of 8\n";
    }
}

void Ltalk::EventLoop::RunPendingCallBackFunc() {
    std::vector<CallBack> v_callback_functions;
    calling_pending_callback_function_ = true;
    MutexLockGuard mutex_lock_guard(mutex_lock_);

    //Calling all functions in pending vecotr
    v_callback_functions.swap(pending_callback_functions_);
    for( size_t idx = 0; idx < v_callback_functions.size(); ++idx) {
        v_callback_functions[idx]();
    }
    calling_pending_callback_function_ = false;
}

void Ltalk::EventLoop::AssertInLoopThread() {
    assert(IsInLoopThread());
}
