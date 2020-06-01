#include "eventloop_thread.hh"

Ltalk::EventLoopThread::EventLoopThread() :
    eventloop_(nullptr),
    exiting_(false),
    thread_(std::bind(&EventLoopThread::ThreadFunc, this), "EventLoopThread"),
    mutex_lock_(),
    condition_(mutex_lock_) {

}

Ltalk::EventLoopThread::~EventLoopThread() {
    exiting_ = true;
    if(eventloop_ != nullptr) {
        eventloop_->Quit();
        thread_.Join();
    }
}
EventLoop *Ltalk::EventLoopThread::StartLoop() {
    assert(thread_.IsStarted() == false);
    thread_.Start();
    d_cout << "start thread: \n";

    //Waiting for run
    MutexLockGuard mutex_lock_guard(mutex_lock_);
    while(eventloop_ == nullptr)
        condition_.Wait();

    d_cout << "start thread ok! \n";
    // return a new eventloop object ptr
    return eventloop_;
}

void Ltalk::EventLoopThread::ThreadFunc() {
    // create a new eventloop
    EventLoop eventloop;
    d_cout << "ThreadFunc OK\n";
    eventloop_ = &eventloop;
    condition_.Notify(); // Sync

    eventloop_->Loop(); //run event
    eventloop_ = nullptr;
}
