#include "http.hh"
//declare static variable
std::unordered_map<std::string, std::string> HttpContentType::umap_type_;
pthread_once_t HttpContentType::once_control_;

void Ltalk::HttpContentType::Init() {
    //init http content type
    HttpContentType::umap_type_[".html"] = "text/html";
    HttpContentType::umap_type_[".avi"] = "video/x-msvideo";
    HttpContentType::umap_type_[".bmp"] = "image/bmp";
    HttpContentType::umap_type_[".c"] = "text/plain";
    HttpContentType::umap_type_[".doc"] = "application/msword";
    HttpContentType::umap_type_[".gif"] = "image/gif";
    HttpContentType::umap_type_[".gz"] = "application/x-gzip";
    HttpContentType::umap_type_[".htm"] = "text/html";
    HttpContentType::umap_type_[".ico"] = "image/x-icon";
    HttpContentType::umap_type_[".jpg"] = "image/jpeg";
    HttpContentType::umap_type_[".png"] = "image/png";
    HttpContentType::umap_type_[".txt"] = "text/plain";
    HttpContentType::umap_type_[".mp3"] = "audio/mp3";
    HttpContentType::umap_type_["default"] = "text/html";
}

std::string Ltalk::HttpContentType::GetType(const std::string name) {
    pthread_once(&HttpContentType::once_control_, HttpContentType::Init);
    if(umap_type_.find(name) == umap_type_.end())
        return umap_type_["default"];
    else
        return umap_type_[name];
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

void Ltalk::Http::HandleClose() {

}

void Ltalk::Http::LinkTimer(SPNetTimer sp_net_timer) {
    wp_net_timer_ = sp_net_timer;
}

void Ltalk::Http::UnlinkTimer() {
    if(wp_net_timer_.lock()) {
        SPNetTimer sp_net_timer(wp_net_timer_.lock());
        sp_net_timer->Clear();
        wp_net_timer_.reset();
    }
}
void Ltalk::Http::HandleRead() {

}

void Ltalk::Http::HandleWrite() {

}

void Ltalk::Http::HandleConnect() {

}

void Ltalk::Http::HandleError() {

}
