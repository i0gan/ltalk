#include "eventloop_thread.hh"

Net::EventLoopThread::EventLoopThread() :
    eventloop_(nullptr),
    exiting_(false),
    thread_(std::bind(&EventLoopThread::ThreadFunc, this), "EventLoopThread"),
    mutex_lock_(),
    condition_(mutex_lock_) {

}

Net::EventLoopThread::~EventLoopThread() {
    exiting_ = true;
    if(eventloop_ != nullptr) {
        eventloop_->Quit();
        thread_.Join();
    }
}
Net::EventLoop *Net::EventLoopThread::StartLoop() {
    assert(thread_.IsStarted() == false);
    thread_.Start();

    //Waiting for run
    Thread::MutexLockGuard mutex_lock_guard(mutex_lock_);
    while(eventloop_ == nullptr)
        condition_.Wait();

    // return a new eventloop object ptr
    return eventloop_;
}

void Net::EventLoopThread::ThreadFunc() {
    // create a new eventloop
    EventLoop eventloop;
    eventloop_ = &eventloop;
    condition_.Signal(); // Notify Main thread then realize Sync
    eventloop_->Loop();  //run event
    eventloop_ = nullptr;
}
