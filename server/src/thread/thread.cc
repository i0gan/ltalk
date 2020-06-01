#include "thread.hh"

__thread pid_t  Ltalk::CurrentThread::tid;
__thread char Ltalk::CurrentThread::string[32];
__thread int Ltalk::CurrentThread::string_length;
__thread const char *Ltalk::CurrentThread::name;



Ltalk::Thread::Thread(const CallBack &call_back, const std::string &name) :
    started_(false),
    joined_(false),
    pthread_id(0),
    tid_(0),
    func_(call_back),
    name_(name),
    count_down_latch_(1) {

    SetDefaultName();
}

Ltalk::Thread::~Thread() {
    if(started_ && !joined_)
        pthread_detach(pthread_id);
}

void Ltalk::Thread::SetDefaultName() {
    if(name_.empty()) {
        char buf[32];
        snprintf(buf, sizeof (buf), "thread");
        name_ = buf;
    }
}

void Ltalk::Thread::Start() {
    assert(!started_); // if started will assert
    started_ = true;
    ThreadData *thread_data = new ThreadData(func_, name_, &tid_, &count_down_latch_);
    if(pthread_create(&pthread_id, nullptr, &Run, thread_data)) {
        started_ = false;
        delete thread_data;
    }else {
        count_down_latch_.Wait();
        assert(tid_ > 0);
    }
}

void *Ltalk::Thread::Run(void *arg) {
    ThreadData *thread_data = static_cast<ThreadData *>(arg);
    thread_data->Run(); //run callback function
    delete thread_data;
    return nullptr;
}

Ltalk::ThreadData::ThreadData(const CallBack &func, const std::string &name,
                              pid_t *tid, CountDownLatch *count_down_latch) :
    func_(func),
    name_(name),
    tid_(tid),
    count_down_latch_(count_down_latch) {
}

Ltalk::ThreadData::~ThreadData() {

}

void Ltalk::ThreadData::Run() {
    *tid_ = CurrentThread::get_tid();
    count_down_latch_->CountDown();

    tid_ = nullptr; //set as null
    count_down_latch_ = nullptr;
    CurrentThread::name = name_.empty() ? "thread" : name_.c_str();
    prctl(PR_SET_NAME, CurrentThread::name); //set process name
    func_(); //run callback funciton
    CurrentThread::name = "finished";
}

