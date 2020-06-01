#pragma once
#include <memory>
#include <unordered_map>

#include "../ltalk.hh"
#include "../net/eventloop.hh"
#include "../net/timer.hh"

namespace Ltalk {
enum class HttpProcessState {
    STATE_PARSE_URI = 0,
    STATE_PARSE_HEADERS,
    STATE_RECV_BODY,
    STATE_ANALYSIS,
    STATE_FINISH
};

enum class HttpURIState {
    PARSE_URI_AGAIN = 0,
    PARSE_URI_ERROR,
    PARSE_URI_SUCCESS,
};

enum class HttpHeaderState {
    PARSE_HEADER_SUCCESS = 0,
    PARSE_HEADER_AGAIN,
    PARSE_HEADER_ERROR
};

enum class HttpAnalysisState {
    ANALYSIS_SUCCESS = 0,
    ANALYSIS_ERROR
};

enum class HttpParseState {
    H_START = 0,
    H_KEY,
    H_COLON,
    H_SPACES_AFTER_COLON,
    H_VALUE,
    H_CR,
    H_LF,
    H_END_CR,
    H_END_LF
};

enum class HttpConnectionState {
    H_CONNECTED = 0,
    H_DISCONNECTING,
    H_DISCONNECTED
};

enum class HttpMethod {
    METHOD_POST = 0,
    METHOD_GET,
    METHOD_HEAD
};

enum class HttpVersion {
    METHOD_POST = 0,
    METHOD_GET,
    METHOD_HEAD
};
class HttpContentType {
public:
    static std::string GetType(const std::string name);
private:
    static void Init();
    static std::unordered_map<std::string, std::string> type_;
    static pthread_once_t once_control_;
};

class Http final{
public:
    Http(int fd,EventLoop *eventloop);
    ~Http();
    void Reset();
    void Seperatetime();
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
    HttpParseState http_parse_state_;
    HttpProcessState http_process_state_;
    HttpURIState http_uri_state_;
    HttpHeaderState http_header_state_;
    HttpAnalysisState http_analysis_state_;
    bool keep_alive_;
    std::string file_name_;
    std::string path_;
    int read_postioin_;

    void HandleRead();
    void HandleWrite();
    void HandleConnect();
    void HandleError();
};
}
