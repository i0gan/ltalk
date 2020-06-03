#include "http.hh"
//declare static variable
std::unordered_map<std::string, std::string> HttpContentType::umap_type_;
pthread_once_t HttpContentType::once_control_;

const __uint32_t EPOLL_DEFAULT_EVENT = EPOLLIN | EPOLLET | EPOLLONESHOT;
const int DEFAULT_EXPIRED_TIME = 2000;              //ms
const int DEFAULT_KEEP_ALIVE_TIME = 5 * 60 * 1000;  //ms


char web_favicon[555] = {
    '\x89', 'P',    'N',    'G',    '\xD',  '\xA',  '\x1A', '\xA',  '\x0',
    '\x0',  '\x0',  '\xD',  'I',    'H',    'D',    'R',    '\x0',  '\x0',
    '\x0',  '\x10', '\x0',  '\x0',  '\x0',  '\x10', '\x8',  '\x6',  '\x0',
    '\x0',  '\x0',  '\x1F', '\xF3', '\xFF', 'a',    '\x0',  '\x0',  '\x0',
    '\x19', 't',    'E',    'X',    't',    'S',    'o',    'f',    't',
    'w',    'a',    'r',    'e',    '\x0',  'A',    'd',    'o',    'b',
    'e',    '\x20', 'I',    'm',    'a',    'g',    'e',    'R',    'e',
    'a',    'd',    'y',    'q',    '\xC9', 'e',    '\x3C', '\x0',  '\x0',
    '\x1',  '\xCD', 'I',    'D',    'A',    'T',    'x',    '\xDA', '\x94',
    '\x93', '9',    'H',    '\x3',  'A',    '\x14', '\x86', '\xFF', '\x5D',
    'b',    '\xA7', '\x4',  'R',    '\xC4', 'm',    '\x22', '\x1E', '\xA0',
    'F',    '\x24', '\x8',  '\x16', '\x16', 'v',    '\xA',  '6',    '\xBA',
    'J',    '\x9A', '\x80', '\x8',  'A',    '\xB4', 'q',    '\x85', 'X',
    '\x89', 'G',    '\xB0', 'I',    '\xA9', 'Q',    '\x24', '\xCD', '\xA6',
    '\x8',  '\xA4', 'H',    'c',    '\x91', 'B',    '\xB',  '\xAF', 'V',
    '\xC1', 'F',    '\xB4', '\x15', '\xCF', '\x22', 'X',    '\x98', '\xB',
    'T',    'H',    '\x8A', 'd',    '\x93', '\x8D', '\xFB', 'F',    'g',
    '\xC9', '\x1A', '\x14', '\x7D', '\xF0', 'f',    'v',    'f',    '\xDF',
    '\x7C', '\xEF', '\xE7', 'g',    'F',    '\xA8', '\xD5', 'j',    'H',
    '\x24', '\x12', '\x2A', '\x0',  '\x5',  '\xBF', 'G',    '\xD4', '\xEF',
    '\xF7', '\x2F', '6',    '\xEC', '\x12', '\x20', '\x1E', '\x8F', '\xD7',
    '\xAA', '\xD5', '\xEA', '\xAF', 'I',    '5',    'F',    '\xAA', 'T',
    '\x5F', '\x9F', '\x22', 'A',    '\x2A', '\x95', '\xA',  '\x83', '\xE5',
    'r',    '9',    'd',    '\xB3', 'Y',    '\x96', '\x99', 'L',    '\x6',
    '\xE9', 't',    '\x9A', '\x25', '\x85', '\x2C', '\xCB', 'T',    '\xA7',
    '\xC4', 'b',    '1',    '\xB5', '\x5E', '\x0',  '\x3',  'h',    '\x9A',
    '\xC6', '\x16', '\x82', '\x20', 'X',    'R',    '\x14', 'E',    '6',
    'S',    '\x94', '\xCB', 'e',    'x',    '\xBD', '\x5E', '\xAA', 'U',
    'T',    '\x23', 'L',    '\xC0', '\xE0', '\xE2', '\xC1', '\x8F', '\x0',
    '\x9E', '\xBC', '\x9',  'A',    '\x7C', '\x3E', '\x1F', '\x83', 'D',
    '\x22', '\x11', '\xD5', 'T',    '\x40', '\x3F', '8',    '\x80', 'w',
    '\xE5', '3',    '\x7',  '\xB8', '\x5C', '\x2E', 'H',    '\x92', '\x4',
    '\x87', '\xC3', '\x81', '\x40', '\x20', '\x40', 'g',    '\x98', '\xE9',
    '6',    '\x1A', '\xA6', 'g',    '\x15', '\x4',  '\xE3', '\xD7', '\xC8',
    '\xBD', '\x15', '\xE1', 'i',    '\xB7', 'C',    '\xAB', '\xEA', 'x',
    '\x2F', 'j',    'X',    '\x92', '\xBB', '\x18', '\x20', '\x9F', '\xCF',
    '3',    '\xC3', '\xB8', '\xE9', 'N',    '\xA7', '\xD3', 'l',    'J',
    '\x0',  'i',    '6',    '\x7C', '\x8E', '\xE1', '\xFE', 'V',    '\x84',
    '\xE7', '\x3C', '\x9F', 'r',    '\x2B', '\x3A', 'B',    '\x7B', '7',
    'f',    'w',    '\xAE', '\x8E', '\xE',  '\xF3', '\xBD', 'R',    '\xA9',
    'd',    '\x2',  'B',    '\xAF', '\x85', '2',    'f',    'F',    '\xBA',
    '\xC',  '\xD9', '\x9F', '\x1D', '\x9A', 'l',    '\x22', '\xE6', '\xC7',
    '\x3A', '\x2C', '\x80', '\xEF', '\xC1', '\x15', '\x90', '\x7',  '\x93',
    '\xA2', '\x28', '\xA0', 'S',    'j',    '\xB1', '\xB8', '\xDF', '\x29',
    '5',    'C',    '\xE',  '\x3F', 'X',    '\xFC', '\x98', '\xDA', 'y',
    'j',    'P',    '\x40', '\x0',  '\x87', '\xAE', '\x1B', '\x17', 'B',
    '\xB4', '\x3A', '\x3F', '\xBE', 'y',    '\xC7', '\xA',  '\x26', '\xB6',
    '\xEE', '\xD9', '\x9A', '\x60', '\x14', '\x93', '\xDB', '\x8F', '\xD',
    '\xA',  '\x2E', '\xE9', '\x23', '\x95', '\x29', 'X',    '\x0',  '\x27',
    '\xEB', 'n',    'V',    'p',    '\xBC', '\xD6', '\xCB', '\xD6', 'G',
    '\xAB', '\x3D', 'l',    '\x7D', '\xB8', '\xD2', '\xDD', '\xA0', '\x60',
    '\x83', '\xBA', '\xEF', '\x5F', '\xA4', '\xEA', '\xCC', '\x2',  'N',
    '\xAE', '\x5E', 'p',    '\x1A', '\xEC', '\xB3', '\x40', '9',    '\xAC',
    '\xFE', '\xF2', '\x91', '\x89', 'g',    '\x91', '\x85', '\x21', '\xA8',
    '\x87', '\xB7', 'X',    '\x7E', '\x7E', '\x85', '\xBB', '\xCD', 'N',
    'N',    'b',    't',    '\x40', '\xFA', '\x93', '\x89', '\xEC', '\x1E',
    '\xEC', '\x86', '\x2',  'H',    '\x26', '\x93', '\xD0', 'u',    '\x1D',
    '\x7F', '\x9',  '2',    '\x95', '\xBF', '\x1F', '\xDB', '\xD7', 'c',
    '\x8A', '\x1A', '\xF7', '\x5C', '\xC1', '\xFF', '\x22', 'J',    '\xC3',
    '\x87', '\x0',  '\x3',  '\x0',  'K',    '\xBB', '\xF8', '\xD6', '\x2A',
    'v',    '\x98', 'I',    '\x0',  '\x0',  '\x0',  '\x0',  'I',    'E',
    'N',    'D',    '\xAE', 'B',    '\x60', '\x82',
};


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
    map_header_info_.clear();
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
    //HandleError(HttpResponseCode::OK, "Just a test");
    do {
        int read_len = Util::ReadData(fd_, in_buffer_);
        //d_cout << "Request: \n" << in_buffer_;
        //if state as disconnecting will clean th in buffer
        if(http_connection_state_ == HttpConnectionState::DISCONNECTING) {
            in_buffer_.clear();
            break;
        }
        if(read_len == 0) {
            http_connection_state_ = HttpConnectionState::DISCONNECTING;
            break;
        }else if(read_len < 0) { // Read data error
            //perror("ReadData ");
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
        }
        //std::cout << "content: [" << in_buffer_ << "]\n";
        // Get content length
        if(map_header_info_["methoed"] == "POST") {
            int content_length = 0;
            if(map_header_info_.find("Content-length") != map_header_info_.end()) {
                content_length = std::stoi(map_header_info_["Content-length"]);
            } else {
                error_ = true;
                HandleError(HttpResponseCode::BAD_REQUEST, "Bad Request");
                break;
            }
            if(static_cast<int>(in_buffer_.size()) < content_length) {
                error_ = true;
                HandleError(HttpResponseCode::BAD_REQUEST, "Bad Request");
            }
        }

        // The content length error
        // Get content
        http_process_state_ = HttpProcessState::PROCESS;

    } while(false);
    std::cout << "OKKK\n";
    std::string a = "sss";
    send_content_ = a;
    HandleWrite();
    std::cout << "End\n";
    // end
    if(!error_ && http_process_state_ == HttpProcessState::FINISH) {
        this->Reset();
        if(in_buffer_.size()) {
            if(http_connection_state_ != HttpConnectionState::DISCONNECTING)
                HandleRead();
        }
    } else if (!error_ && http_connection_state_ != HttpConnectionState::DISCONNECTED) {
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
        std::string one_line = recv_data.substr(0, key_end_pos);
        value_start_pos = one_line.find(':');
        // Last line have not  key and value
        if(value_start_pos < 0) {
            result = HttpParseHeaderResult::SUCCESS;
            break;
        }
        std::string key = one_line.substr(0, value_start_pos);
        std::string value = one_line.substr(value_start_pos + 2);
        map_header_info_[key] = value;
        // sub str
        recv_data = recv_data.substr(key_end_pos + 2);
    }
    return result;
}


void Ltalk::Http::HandleProcess() {

}
void Ltalk::Http::HandleWrite() {
    out_buffer_.clear();
    //std::cout << "handle write";
    if(error_ || http_connection_state_ == HttpConnectionState::DISCONNECTED) {
        return;
    }
    __uint32_t &event = sp_channel_->get_event();
    std::string header;
    header += "HTTP/1.1 200 OK\r\n";
    if (map_header_info_.find("Connection") != map_header_info_.end() &&
            (map_header_info_["Connection"] == "Keep-Alive" ||
             map_header_info_["Connection"] == "keep-alive")) {
        keep_alive_ = true;
        header += std::string("Connection: Keep-Alive\r\n") + "Keep-Alive: timeout=" +
                std::to_string(DEFAULT_KEEP_ALIVE_TIME) + "\r\n";
    }

    header += HttpContentType::GetType(send_content_type_);
    header += "Server: Linux x64 LYXF Ltalk server\r\n";
    header += "Content-type: " + HttpContentType::GetType("default") + "\r\n";
    header += "Content-Length: " +  std::to_string(send_content_.size()) + "\r\n";
    header += "\r\n";

    out_buffer_ += header;
    out_buffer_ += send_content_;

    if(Util::WriteData(fd_, out_buffer_) < 0) {
        perror("write data");
        event = 0;
        error_ = true;
    }

    if (out_buffer_.size() > 0)
        event |= EPOLLOUT;
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
