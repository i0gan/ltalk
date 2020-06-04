
#include "http.hh"
//declare static variable
std::unordered_map<std::string, std::string> HttpContentType::umap_type_;
pthread_once_t HttpContentType::once_control_;

const __uint32_t EPOLL_DEFAULT_EVENT = EPOLLIN | EPOLLET | EPOLLONESHOT;
const int DEFAULT_EXPIRED_TIME = 2000;              //ms
const int DEFAULT_KEEP_ALIVE_TIME = 5 * 60 * 1000;  //ms

void Ltalk::HttpContentType::Init() {
    //init http content type
    HttpContentType::umap_type_[".html"] = "text/html";
    HttpContentType::umap_type_[".css"] = "text/css";
    HttpContentType::umap_type_[".js"] = "application/x-javascript";
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
    HttpContentType::umap_type_[".json"] = "application/json";
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
    error_(false),
    http_connection_state_(HttpConnectionState::CONNECTED),
    http_process_state_(HttpProcessState::PARSE_HEADER),
    keep_alive_(false) {

    //set callback function handler
    sp_channel_->set_read_handler(std::bind(&Http::HandleRead, this));
    sp_channel_->set_write_handler(std::bind(&Http::HandleWrite, this));
    sp_channel_->set_connected_handler(std::bind(&Http::HandleConnect, this));

}
Ltalk::Http::~Http() {
    close(fd_);
}

void Ltalk::Http::Reset() {
    http_process_state_ = HttpProcessState::PARSE_HEADER;
    in_content_buffer_.clear();
    map_header_info_.clear();
    error_ = false;
    if(wp_net_timer_.lock()) {
        SPNetTimer sp_net_timer(wp_net_timer_.lock());
        sp_net_timer->Clear();
        wp_net_timer_.reset();
    }
}

void Ltalk::Http::HandleClose() {
    http_connection_state_ = HttpConnectionState::DISCONNECTED;
    SPHttp guard(shared_from_this()); // avoid delete
    eventloop_->RemoveFromEpoll(sp_channel_);
}

void Ltalk::Http::NewEvnet() {
    sp_channel_->set_event(EPOLL_DEFAULT_EVENT);
    eventloop_->AddToEpoll(sp_channel_, DEFAULT_EXPIRED_TIME);
}

void Ltalk::Http::LinkTimer(SPNetTimer sp_net_timer) {
    wp_net_timer_ = sp_net_timer;
}

SPChannel Ltalk::Http::get_sp_channel() {
    return sp_channel_;
}

EventLoop *Ltalk::Http::get_eventloop() {
    return eventloop_;
}

void Ltalk::Http::UnlinkTimer() {
    if(wp_net_timer_.lock()) {
        SPNetTimer sp_net_timer(wp_net_timer_.lock());
        sp_net_timer->Clear();
        wp_net_timer_.reset();
    }
}
void Ltalk::Http::HandleRead() {
    __uint32_t &event = sp_channel_->get_event();
    do {
        int read_len = Util::ReadData(fd_, in_buffer_);
        //std::cout << "http_content:__[" << in_buffer_ << "]__";
        //if state as disconnecting will clean th in buffer
        if(http_connection_state_ == HttpConnectionState::DISCONNECTING) {
            std::cout << "DISCONNECTING\n";
            in_buffer_.clear();
            break;
        }
        if(read_len == 0) {
            http_connection_state_ = HttpConnectionState::DISCONNECTING;
            std::cout << "recv 0 DISCONNECTING\n";
            in_buffer_.clear();
            break;
        }else if(read_len < 0) { // Read data error
            perror("ReadData ");
            error_ = true;
            HandleError(HttpResponseCode::BAD_REQUEST, "Bad Request");
        }

        // Parse http header
        if(http_process_state_ == HttpProcessState::PARSE_HEADER) {
            HttpParseHeaderResult http_parse_header_result = ParseHeader();
            if(http_parse_header_result == HttpParseHeaderResult::ERROR) {
                perror("ParseHeader ");
                error_ = true;
                HandleError(HttpResponseCode::BAD_REQUEST, "Bad Request");
                break;
            }
            // Judget if have content data
            //std::cout << "method: " << map_header_info_["method"] << '\n';
            http_process_state_ = HttpProcessState::PROCESS;
            if(map_header_info_["method"] == "POST") {
                content_length_ = 0;
                if(map_header_info_.find("Content-Length") != map_header_info_.end()) {
                    content_length_ = std::stoi(map_header_info_["Content-Length"]);
                } else {
                    std::cout << "not found contnt-length\n";
                    error_ = true;
                    HandleError(HttpResponseCode::BAD_REQUEST, "Bad Request");
                    break;
                }

                if(content_length_ < 0) {
                    std::cout << "not found contnt-length\n";
                    error_ = true;
                    HandleError(HttpResponseCode::BAD_REQUEST, "Bad Request");
                    break;
                }
                //std::cout << "have body data: Cotent-length: " << content_length_ << '\n';
                http_process_state_ = HttpProcessState::RECV_BODY;
            }
        }

        // Recv body data
        if(http_process_state_ == HttpProcessState::RECV_BODY) {
            //std::cout << "recved body data\n";
            // Get content length
            in_content_buffer_ += in_buffer_;
            if(!error_ && static_cast<int>(in_content_buffer_.size()) >= content_length_) {
                //std::cout << "content: __[[" << in_content_buffer_ << "]]__";
                http_process_state_ = HttpProcessState::PROCESS;
            }
        }

        if(http_process_state_ == HttpProcessState::PROCESS) {
            HandleProcess();
            http_process_state_ = HttpProcessState::FINISH;
        }

    } while(false);

    if(error_) {
        //std::cout << "error\n";
        this->Reset();
    }
    // end
    if(http_process_state_ == HttpProcessState::FINISH) {
        this->Reset();
    } else if (!error_ && http_connection_state_ == HttpConnectionState::DISCONNECTED) {
        event |= EPOLLIN;
    }
}

Ltalk::HttpParseHeaderResult Ltalk::Http::ParseHeader() {
    std::string &recv_data = in_buffer_;
    Ltalk::HttpParseHeaderResult  result = HttpParseHeaderResult::SUCCESS;

    int first_line_read_pos = 0;
    do {
        first_line_read_pos = recv_data.find("\r\n");
        if(first_line_read_pos < 0) {
            result = HttpParseHeaderResult::ERROR;
            break;
        }
        std::string header_line_1 = recv_data.substr(0, first_line_read_pos);
        if(static_cast<int>(recv_data.size()) > first_line_read_pos + 2)
            // sub first line str
            recv_data = recv_data.substr(first_line_read_pos + 2);
        else {
            result = HttpParseHeaderResult::ERROR;
            break;
        }
        // parse http method
        do {
            int http_method_pos = -1;
            if((http_method_pos = header_line_1.find("GET")) >= 0) {
                first_line_read_pos = http_method_pos;
                map_header_info_["method"] = "GET";
                break;
            } else if((http_method_pos = header_line_1.find("POST")) >= 0) {
                first_line_read_pos = http_method_pos;
                map_header_info_["method"] = "POST";
                break;
            } else if((http_method_pos = header_line_1.find("HEAD")) >= 0) {
                first_line_read_pos = http_method_pos;
                map_header_info_["method"] = "HEAD";
                break;
            } else if((http_method_pos = header_line_1.find("DELETE")) >= 0){
                first_line_read_pos = http_method_pos;
                map_header_info_["method"] = "DELETE";
                break;
            } else {
                break;
            }
        } while(false);

        if(first_line_read_pos < 0) {
            result = HttpParseHeaderResult::ERROR;
            break;
        }

        int http_url_start_pos = header_line_1.find('/');
        if(http_url_start_pos < 0) break;
        // sub str
        header_line_1 = header_line_1.substr(http_url_start_pos);

        int http_url_end_pos = header_line_1.find (' ');
        if(http_url_end_pos < 0) {
            result = HttpParseHeaderResult::ERROR;
            break;
        }

        map_header_info_["url"] = header_line_1.substr(0, http_url_end_pos);

        // Parse http version
        header_line_1 = header_line_1.substr(http_url_end_pos);
        int http_version_pos = header_line_1.find('/');
        if(http_version_pos < 0) {
            result = HttpParseHeaderResult::ERROR;
            break;
        }

        map_header_info_["version"]  = header_line_1.substr(http_version_pos + 1);
    }  while(false);


    // chceck is error
    if (result == HttpParseHeaderResult::ERROR) {
        map_header_info_.clear();
        return result;
    }

    // Parse header key and value
    while(true) {
        int key_end_pos = -1, value_start_pos = -1;
        key_end_pos = recv_data.find("\r\n");
        if(key_end_pos < 0) {
            result = HttpParseHeaderResult::ERROR;
            break;
        }
        // sub str
        std::string one_line = recv_data.substr(0, key_end_pos);
        recv_data = recv_data.substr(key_end_pos + 2);

        value_start_pos = one_line.find(':');
        // Last line have not  key and value
        if(value_start_pos < 0) {
            result = HttpParseHeaderResult::SUCCESS;
            break;
        }
        std::string key = one_line.substr(0, value_start_pos);
        std::string value = one_line.substr(value_start_pos + 2);
        map_header_info_[key] = value;


    }
    return result;
}

void Ltalk::Http::HandleProcess() {
    Ltalk::Center center(map_header_info_, in_content_buffer_);
    center.set_send_data_handler(std::bind(&Http::SendData, this, std::placeholders::_1, std::placeholders::_2));
    center.set_send_file_handler(std::bind(&Http::SendFile, this, std::placeholders::_1));
    center.Process();
}

void Ltalk::Http::HandleWrite() {
    __uint32_t &event = sp_channel_->get_event();
    if(error_ || http_connection_state_ == HttpConnectionState::DISCONNECTED) {
        return;
    }

    if(Util::WriteData(fd_, out_buffer_) < 0) {
        perror("write data");
        event = 0;
        error_ = true;
    }
    if (out_buffer_.size() > 0)
        event |= EPOLLOUT;
}

void Ltalk::Http::SendData(const std::string &type,const std::string &content) {

}

void Ltalk::Http::SendFile(const std::string &file_name) {
    if(error_ || http_connection_state_ == HttpConnectionState::DISCONNECTED) {
        return;
    }

    std::string file_content;
    std::string suffix = file_name;
    bool error = false;
    char buf[MAX_BUF_SIZE];
    int fd = open(file_name.c_str(), O_RDONLY);
    do {
        if(fd == -1) {
            int fd_2 = open(global_web_404_page.c_str(), O_RDONLY);
            if(fd_2 == -1) {
                SendFile(global_web_404_page.c_str());
                return;
            }
            file_content = "not found";
            break;
        }
        int read_len = 0;
        while(true) {
            if((read_len = read(fd, buf, MAX_BUF_SIZE)) <= 0) {
                break;
            }
            file_content += std::string(buf, buf + read_len);
        }
        close(fd);
    } while(false);
    // get suffix name
    if(!error) {
        while (true) {
            int suffix_pos = suffix.find('.');
            if(suffix_pos < 0)
                break;
            suffix = suffix.substr(suffix_pos + 1);
        }
    }
    suffix = '.' + suffix;


    //std::cout << "suffix: [" << suffix << "] file_name: [" << file_name << "]\n";
    //std::cout << "handle write";
    std::string header;
    header += "HTTP/1.1 200 OK\r\n";
    if (map_header_info_.find("Connection") != map_header_info_.end() &&
            (map_header_info_["Connection"] == "Keep-Alive" ||
             map_header_info_["Connection"] == "keep-alive")) {
        keep_alive_ = true;
        header += std::string("Connection: Keep-Alive\r\n") + "Keep-Alive: timeout=" +
                std::to_string(DEFAULT_KEEP_ALIVE_TIME) + "\r\n";
    }

    header += "Server: Linux x64 LYXF Ltalk server\r\n";
    header += "Content-type: " + HttpContentType::GetType(suffix) + "\r\n";
    //header += "Content-Length: " +  std::to_string(100) + "\r\n";
    header += "\r\n";

    out_buffer_.clear();
    out_buffer_ += header;
    std::cout << "send:[" << header << "]";
    HandleWrite();


    //out_buffer_ += send_content;

}

void Ltalk::Http::HandleConnect() {
    int ms_timeout = 0;
    __uint32_t &event = sp_channel_->get_event();

    UnlinkTimer();
    if(!error_ && http_connection_state_ == HttpConnectionState::CONNECTED) {
        if(event != 0) {
            if(keep_alive_)
                ms_timeout = DEFAULT_KEEP_ALIVE_TIME;
            else
                ms_timeout = DEFAULT_EXPIRED_TIME;
            if((event & EPOLLIN) && (event & EPOLLOUT)) {
                event = 0;
                event |= EPOLLOUT;
            }
            event |= EPOLLET;
        } else if (keep_alive_) {
            event |= (EPOLLIN | EPOLLET);
            ms_timeout = DEFAULT_KEEP_ALIVE_TIME;
        } else {
            event |= (EPOLLIN | EPOLLET);
            ms_timeout = DEFAULT_KEEP_ALIVE_TIME >> 1;
        }

        eventloop_->UpdateEpoll(sp_channel_, ms_timeout);
    } else if (!error_ && http_connection_state_ == HttpConnectionState::DISCONNECTING
               && (event & EPOLLOUT)) {
        event = (EPOLLOUT | EPOLLET);
    } else {
        eventloop_->RunInLoop(std::bind(&Http::HandleClose, shared_from_this()));
    }
}

void Ltalk::Http::HandleError(HttpResponseCode error_number, std::string message) {
    message = " " + message;
    std::string header_buffer, body_buffer;
    body_buffer += "<html><title>Bad request</title>";
    body_buffer += "<body bgcolor=\"dead00\">";
    body_buffer += std::to_string(static_cast<int>(error_number)) + message;
    body_buffer += "<hr><em> Linux x64 LYXF Ltalk Server </em>\n</body></html>";

    header_buffer += "HTTP/1.1 " + std::to_string(static_cast<int>(error_number)) + message + "\r\n";
    header_buffer += "Content-Type: text/html\r\n";
    header_buffer += "Connection: Close\r\n";
    header_buffer += "Content-Length: " + std::to_string(body_buffer.size()) + "\r\n";
    header_buffer += "Server: Linux x64 LYXF Ltalk Server\r\n";
    header_buffer += "\r\n";

    Util::WriteData(fd_, header_buffer);
    Util::WriteData(fd_, body_buffer);
}


void Ltalk::Http::HandleNotFound() {

}
