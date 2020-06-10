#include "eventloop.hh"
__thread Net::EventLoop * global_eventloop = nullptr;

Net::EventLoop::EventLoop() :
    looping_(false),
    awake_fd_(EventLoop::CreateEventFd()),
    quit_(false),
    event_handling_(false),
    thread_id_(Thread::CurrentThread::get_tid()),
    sp_epoll_(new Net::Epoll()),
    sp_awake_channel_(new Channel(this, awake_fd_))
{
    if(global_eventloop) {

    }else {
        global_eventloop = this;
    }
    sp_awake_channel_->set_event(EPOLLIN | EPOLLET);
    sp_awake_channel_->set_read_handler(std::bind(&EventLoop::HandleRead, this));
    sp_awake_channel_->set_connected_handler(std::bind(&EventLoop::HandleConnect, this));
    sp_epoll_->Add(sp_awake_channel_, 0);
}
Net::EventLoop::~EventLoop() {

}

void Net::EventLoop::HandleRead() {
    char buf[8];
    ssize_t read_len = Util::Read(awake_fd_, &buf, sizeof (buf));
    if(read_len != sizeof (buf)) {
        std::cout << "EventLoop::handRead() reads " << read_len << "instead of 8\n";
    }
    sp_awake_channel_->set_event(EPOLLIN | EPOLLET);
}

void Net::EventLoop::HandleConnect() {

}

void Net::EventLoop::UpdateEpoll(SPChannel sp_channel, int ms_timeout) {
    sp_epoll_->Mod(sp_channel, ms_timeout);
}

void Net::EventLoop::AddToEpoll(SPChannel sp_channel, int ms_timeout) {
    sp_epoll_->Add(sp_channel, ms_timeout);
}


int Net::EventLoop::CreateEventFd() {
    int event_fd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if(event_fd < 0) {
        perror("eventfd:");
        abort();
    }
    return event_fd;
}

void Net::EventLoop::Loop() {
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
        sp_epoll_->HandleExpiredEvent();
    }
}

void Net::EventLoop::Quit() {
    quit_ = true;
    if(IsInLoopThread() == false) {
        WakeUp();
    }
}
void Net::EventLoop::RunInLoop(::Util::CallBack &&func) {
    if(IsInLoopThread())
        func();
    else
        QueueInLoop(std::move(func));
}

void Net::EventLoop::QueueInLoop(::Util::CallBack &&func) {
    Thread::MutexLockGuard mutex_lock_guard(mutex_lock_);
    pending_callback_functions_.emplace_back(std::move(func));
    if(!IsInLoopThread() || calling_pending_callback_function_)
        WakeUp();
}

bool Net::EventLoop::IsInLoopThread() {
    return thread_id_ == Thread::CurrentThread::get_tid();
}

void Net::EventLoop::RemoveFromEpoll(SPChannel sp_channel) {
    sp_epoll_->Del(sp_channel);
}

void Net::EventLoop::WakeUp() {
    char buf[8];
    ssize_t write_len = Util::Write(awake_fd_, buf, sizeof (buf));
    if(write_len != sizeof (buf)) {
        d_cout << "eventloop::wakeup write:" << write_len << " instead of 8\n";
    }
}

void Net::EventLoop::RunPendingCallBackFunc() {
    std::vector<CallBack> v_callback_functions;
    calling_pending_callback_function_ = true;
    Thread::MutexLockGuard mutex_lock_guard(mutex_lock_);

    //Calling all functions in pending vecotr
    v_callback_functions.swap(pending_callback_functions_);
    for( size_t idx = 0; idx < v_callback_functions.size(); ++idx) {
        v_callback_functions[idx]();
    }
    calling_pending_callback_function_ = false;
}

void Net::EventLoop::AssertInLoopThread() {
    assert(IsInLoopThread());
}

void Net::EventLoop::Shutdown(SPChannel sp_channel) {
    Util::ShutDownWriteFd(sp_channel->get_fd());
}
