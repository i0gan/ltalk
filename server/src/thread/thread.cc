#include "thread.hh"

__thread pid_t  Thread::CurrentThread::tid = 0;
__thread Thread::CurrentThread::State Thread::CurrentThread::state = CurrentThread::State::STOPED;
__thread char *Thread::CurrentThread::name;

Thread::Thread::Thread(const Util::CallBack &call_back, const std::string &name) :
    started_(false),
    joined_(false),
    pthread_id(0),
    tid_(0),
    func_(call_back), // 将要运行的回调函数
    name_(name),      // 线程名称
    count_down_latch_(1) { // 等待机制, 单独一次开启一个开启线程, 就设置为1
}

Thread::Thread::~Thread() {
    if(started_ && !joined_)
        pthread_detach(pthread_id);
}

// 设置线程名称
void Thread::Thread::set_name(const std::string &name) {
    name_ = name;
}

// 启动线程
void Thread::Thread::Start() {
    assert(!started_);
    started_ = true;
    // 所进行传递的参数
    ThreadData *thread_data = new ThreadData(func_, name_, &tid_, &count_down_latch_);
    // 创建线程
    if(pthread_create(&pthread_id, nullptr, &Thread::Run, thread_data)) {
        started_ = false;
        delete thread_data; // 创建线程失败, 释放内存
    }else {
        // 等待启动子线程
        count_down_latch_.Wait();
        assert(tid_ > 0);
    }
}

// 子线程函数
void *Thread::Thread::Run(void *arg) {
    ThreadData *thread_data = static_cast<ThreadData *>(arg);
    thread_data->Run();  // 运行将要调用的函数
    delete thread_data;  // 释放内存
    return nullptr;
}

// join线程, 实现父线程等待子线程执行完毕
int Thread::Thread::Join() {
    assert(started_); // 保证已经启动
    assert(!joined_); // 保证线程没有joined
    joined_ = true;
    return pthread_join(pthread_id, nullptr); // join线程, 父线程必须等待子线程处理完毕
}

bool Thread::Thread::IsStarted() {
    return started_;
}

// 用于创建线程传递参数
Thread::ThreadData::ThreadData(const Util::CallBack &func, const std::string &name,
                              pid_t *tid, CountDownLatch *count_down_latch) :
    func_(func),
    name_(name),
    tid_(tid),
    count_down_latch_(count_down_latch) {
}

Thread::ThreadData::~ThreadData() {

}

void Thread::ThreadData::Run() {

    *tid_ = CurrentThread::get_tid();
    count_down_latch_->CountDown(); // 线程运行成功 --count
    tid_ = nullptr;                 // 不用该值
    count_down_latch_ = nullptr;    // 不用该值
    CurrentThread::name = const_cast<char *>(name_.empty() ? "thread" : name_.c_str());
    CurrentThread::state = CurrentThread::State::STARTING;
    prctl(PR_SET_NAME, CurrentThread::name); // 设置当前线程名称, 在内核中, 进程与线程都是单独的
    func_();                    //执行回调函数, 这里的回调函数是创建Thread对象传递过来的回调
    CurrentThread::name = const_cast<char *>("finished"); // 设置当前线程名称为finish
    CurrentThread::state = CurrentThread::State::STOPED;
}



