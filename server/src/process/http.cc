#include "http.hh"

void Ltalk::HttpContentType::Init() {
    //init http content type
    type_[".html"] = "text/html";
    type_[".avi"] = "video/x-msvideo";
    type_[".bmp"] = "image/bmp";
    type_[".c"] = "text/plain";
    type_[".doc"] = "application/msword";
    type_[".gif"] = "image/gif";
    type_[".gz"] = "application/x-gzip";
    type_[".htm"] = "text/html";
    type_[".ico"] = "image/x-icon";
    type_[".jpg"] = "image/jpeg";
    type_[".png"] = "image/png";
    type_[".txt"] = "text/plain";
    type_[".mp3"] = "audio/mp3";
    type_["default"] = "text/html";
}

std::string Ltalk::HttpContentType::GetType(const std::string name) {
    pthread_once(&HttpContentType::once_control_, HttpContentType::Init);
    if(type_.find(name) == type_.end())
        return type_["default"];
    else
        return type_[name];
}

Ltalk::Http::Http(int fd,EventLoop *eventloop) :
    fd_(fd),
    eventloop_(eventloop),
    sp_channel_(new Channel(eventloop, fd)),
    http_connection_state_(HttpConnectionState::CONNECTED),
    http_method_(HttpMethod::GET),
    http_version_(HttpVersion::V_1_0),
    http_process_state_(HttpProcessState::PARSE_URI),
    http_parse_state_(HttpParseState::START),
    keep_alive_(false) {

    //set callback function handler
    sp_channel_->set_read_handler(std::bind(&Http::HandleRead, this));
    sp_channel_->set_write_handler(std::bind(&Http::HandleWrite, this));
    sp_channel_->set_connect_handler(std::bind(&Http::HandleConnect, this));

}
void Ltalk::Http::Reset() {
    file_name_.clear();
    path_.clear();
    read_postioin_ = 0;
    http_process_state_ = HttpProcessState::PARSE_URI;
    http_parse_state_ = HttpParseState::START;
    map_headers_.clear();
    if(wp_net_timer_.lock()) {
        SPNetTimer sp_net_timer(wp_net_timer_.lock());
    }
}


