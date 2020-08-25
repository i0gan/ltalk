 #include "event.hh"
Work::Event::Event(::Util::CallBack call_back, bool loop, int ms_timeout):
    call_back_(call_back),
    loop_(loop),
    timer_(Timer(ms_timeout)) {
    ms_timeout_ = ms_timeout;
}

Work::Event::~Event() {
    std::cout << "event 析gou\n";
}

void Work::Event::Run() {
    // 执行一次
    if(loop_ == false && ms_timeout_ == 0) {
        if(call_back_)
            call_back_();
        return;
    }

    if(!timer_.Timeout()) {
        return;
    }

    timer_.Update(ms_timeout_);
    if(call_back_)
        call_back_();
}

void Work::Event::set_call_back(::Util::CallBack call_back) {
    call_back_ = call_back;
}
