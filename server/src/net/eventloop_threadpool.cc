#include "eventloop_threadpool.hh"
Ltalk::EventLoopThreadPool::EventLoopThreadPool(EventLoop *base_eventloop, int number_of_thread) :
    started_(false),
    base_eventloop_(base_eventloop),
    number_of_thread_(number_of_thread),
    next_(0) {
    if(number_of_thread <= 0) {
        d_cout << "The number of thread must be >= 1\n";
        abort();
    }
}

Ltalk::EventLoopThreadPool::~EventLoopThreadPool() {
    d_cout << "~EventLoopThreadPool()\n";
}

void Ltalk::EventLoopThreadPool::Start() {
    base_eventloop_->AssertInLoopThread();
    started_ = true;
    for(int idx = 0; idx < number_of_thread_; ++idx) {
        SPEventLoopThread sp_eventloop_thread(new EventLoopThread());
        v_sp_eventloop_threads_.push_back(sp_eventloop_thread);
        //store a new eventloop
        v_eventloops_.push_back(sp_eventloop_thread->StartLoop());
    }
}
