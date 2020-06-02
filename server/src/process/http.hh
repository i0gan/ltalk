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

enum class HttpResponseCode {
    OK = 200,
    CREATED,
    ACCEPTED,
    NON_AUTHORITATIVE_INFORMATION,
    NO_CONTENT,
    RESET_CONTENT,
    PARTIAL_CONTENT,

    MULTIPLE_CHOICES = 300,
    MOVED_PERMANENTLY,
    FOUND,
    SEE_OTHER,
    NOT_MODIFIED,
    USE_PROXY,
    SWITCH_PROXY,
    TEMPORARY_REDIRCT,
    RESUME_INCOMPLETE,

    BAD_REQUEST = 400,
    UNAUTHORIZED,
    PAYMENT_REAUIRED,
    FORBIDDEN,
    NOT_FOUND,
    METHOD_NOT_ALLOWED,
    NOT_ACCEPTABLE,
    PROXY_AUTHENTICATION_REQUIRED,
    REQUEST_TIMEOUT,
    CONFLICT,
    GONE,
    LENGTH_REQUIRED,
    PRECONDITION_FAILED,
    REQUEST_ENTITY_TOO_LARGE,
    REQUEST_URI_TOO_LONG,
    UNSUPPORTED_MEDIA_TYPE,
    REQUESTED_RANGE_NOT_SATISFIABLE,
    EXPECTATION_FAILED,

    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED,
    BAD_GATEWAY,
    SERVICE_UNAVAILABLE,
    GATEWAY_TIMEOUT,
    HTTP_VERSION_NOT_SUPPORTED,
    NETWORK_AUTHENTICATION_REQUIRED
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
    HttpProcessState http_process_state_;
    HttpParseHeaderState http_parse_header_state_;
    HttpMethod http_method_;
    HttpVersion http_version_;


    bool keep_alive_;
    std::string file_name_;
    std::string path_;
    int read_postioin_;
    std::map<std::string, std::string> map_headers_;
    std::weak_ptr<NetTimer> wp_net_timer_;

    void HandleRead();
    void HandleWrite();
    void HandleConnect();
    void HandleError(HttpResponseCode error_number, std::string message);
    HttpParseURIResult ParseURI();
};
}
