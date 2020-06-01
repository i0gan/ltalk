#pragma once
#include <memory>
#include <unordered_map>
#include <map>
#include <pthread.h>

#include "../ltalk.hh"
#include "../net/eventloop.hh"
#include "../net/timer.hh"

namespace Ltalk {
enum class HttpProcessState {
    PARSE_URI = 0,
    PARSE_HEADERS,
    RECV_BODY,
    ANALYSIS,
    FINISH
};

enum class HttpURIState {
    AGAIN = 0,
    ERROR,
    SUCCESS,
};

enum class HttpHeaderState {
    SUCCESS = 0,
    AGAIN,
    ERROR
};

enum class HttpAnalysisState {
    ANALYSIS_SUCCESS = 0,
    ANALYSIS_ERROR
};

enum class HttpParseState {
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

enum class HttpConnectionState {
    CONNECTED = 0,
    DISCONNECTING,
    DISCONNECTED
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
    HttpParseState http_parse_state_;
    HttpURIState http_uri_state_;
    HttpHeaderState http_header_state_;
    HttpAnalysisState http_analysis_state_;
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
