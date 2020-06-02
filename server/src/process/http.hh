#pragma once
#include <memory>
#include <unordered_map>
#include <map>
#include <pthread.h>
#include <sys/epoll.h>

#include "../ltalk.hh"
#include "../net/eventloop.hh"
#include "../net/timer.hh"

namespace Ltalk {

enum class HttpProcessState {
    PARSE_URI = 0,
    PARSE_HEADER,
    RECVING_BODY,
    ANALYSIS,
    FINISH
};

enum class HttpConnectionState {
    CONNECTED = 0,
    DISCONNECTING,
    DISCONNECTED
};

enum class HttpParseHeaderState {
    START = 0,
    KEY,
    COLON,
    SPACES_AFTER_COLON,
    VALUE,
    CR,
    LF,
    END_CR,
    END_LF
};

enum class HttpParseURIResult {
    SUCCESS = 0,
    AGAIN,
    ERROR
};

enum class HttpParseHeaderResult {
    SUCCESS = 0,
    AGAIN,
    ERROR
};

enum class HttpAnalysisResult {
    SUCCESS = 0,
    ERROR
};


enum class HttpMethod {
    GET = 0,
    POST,
    HEAD
};

enum class HttpVersion {
    V_1_0 = 0,
    V_1_1
};

class HttpContentType {
public:
    static std::string GetType(const std::string name);
private:
    static std::unordered_map<std::string, std::string> umap_type_;
    static pthread_once_t once_control_;
    static void Init();
};

class Http final : public std::enable_shared_from_this<Http> {
public:
    Http(int fd,EventLoop *eventloop);
    ~Http();
    void Reset();
    void UnlinkTimer();
    void LinkTimer(SPNetTimer sp_net_timer);
    SPChannel get_sp_channel();
    EventLoop *get_eventloop();

    void HandleClose();
    void NewEvnet();

private:
    int fd_;
    EventLoop *eventloop_;
    SPChannel sp_channel_;
    std::string in_buffer_;
    std::string out_buffer_;
    bool error_;
    HttpConnectionState http_connection_state_;
    HttpMethod http_method_;
    HttpVersion http_version_;
    HttpProcessState http_process_state_;
    HttpParseHeaderState http_parse_header_state_;
    HttpParseURIResult http_parse_uri_result_;
    HttpParseHeaderResult http_parse_header_result_;
    HttpAnalysisResult http_analysis_result_;

    bool keep_alive_;
    std::string file_name_;
    std::string path_;
    int read_postioin_;
    std::map<std::string, std::string> map_headers_;
    std::weak_ptr<NetTimer> wp_net_timer_;

    void HandleRead();
    void HandleWrite();
    void HandleConnect();
    void HandleError();
};
}
