 #include "eventloop_thread.hh"
Work::EventLoopThread::EventLoopThread() :
    eventloop_(nullptr),
    exiting_(false),
    thread_(std::bind(&EventLoopThread::ThreadFunc, this), "WorkLoopThread"),
    mutex_lock_(),
    condition_(mutex_lock_) {

}
Work::EventLoopThread::~EventLoopThread() {
    exiting_ = true;
    if(eventloop_ != nullptr) {
        eventloop_->Quit();
        thread_.Join();
    }
}
Work::EventLoop *Work::EventLoopThread::StartLoop() {
    assert(thread_.IsStarted() == false);
    thread_.Start();

    //Waiting for run
    Thread::MutexLockGuard mutex_lock_guard(mutex_lock_);
    while(eventloop_ == nullptr)
        condition_.Wait();

    // return a new eventloop object ptr
    return eventloop_;
}


void Work::EventLoopThread::ThreadFunc() {
    // create a new eventloop
    EventLoop eventloop;
    eventloop_ = &eventloop;
    condition_.Signal();    // Notify Main thread then realize Sync
    eventloop_->Loop();      //run event
    eventloop_ = nullptr;
}

