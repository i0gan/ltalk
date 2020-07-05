 #include "event.hh"
Work::Event::Event(::Util::CallBack call_back, bool loop, int ms_timeout):
    call_back_(call_back),
    loop_(loop),
    timer_(Timer(ms_timeout)) {

    ms_timeout_ = ms_timeout;
}

Work::Event::~Event() {

}

void Work::Event::Run() {
    if(!timer_.Timeout()) {
        return;
    }

    if(call_back_)
        call_back_();
    timer_.Update(ms_timeout_);
}

void Work::Event::set_call_back(::Util::CallBack call_back) {
    call_back_ = call_back;
}
