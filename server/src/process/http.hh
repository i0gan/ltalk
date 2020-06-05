#pragma once
#include <memory>
#include <unordered_map>
#include <map>
#include <pthread.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "../ltalk.hh"
#include "../net/eventloop.hh"
#include "../net/timer.hh"
#include "center.hh"

namespace Ltalk {

enum class HttpRecvState {
    PARSE_HEADER = 0,
    RECV_CONTENT,
    PROCESS,
    FINISH
};

enum class HttpSendState {
    SEND = 0,
    FINISH
};

enum class HttpConnectionState {
    CONNECTED = 0,
    DISCONNECTING,
    DISCONNECTED
};

enum class HttpParseURIResult {
    SUCCESS = 0,
    ERROR
};

enum class HttpParseHeaderResult {
    SUCCESS = 0,
    ERROR
};

enum class HttpAnalysisResult {
    SUCCESS = 0,
    ERROR
};


enum class HttpMethod {
    GET = 0,
    POST,
    PUT,
    HEAD,
    DELETE,
    CONNECT,
    TRACE,
    OPTIONS
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
    explicit Http(int fd,EventLoop *eventloop);
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
    std::string in_content_buffer_;
    bool recv_error_;
    bool send_error_;
    HttpConnectionState http_connection_state_;
    HttpRecvState http_process_state_;
    HttpSendState http_send_state_;
    int content_length_;

    bool keep_alive_;
    std::map<std::string, std::string> map_header_info_;
    std::weak_ptr<NetTimer> wp_net_timer_;


    void HandleRead();

    void HandleWrite();
    void HandleConnect();
    void HandleError(HttpResponseCode error_number, std::string message);
    void HandleNotFound();
    HttpParseHeaderResult ParseHeader();
    void HandleProcess();
    std::string GetSuffix(std::string file_name);
    void SendData(const std::string &type,const std::string &content);
    void SendFile(const std::string &file_name);
    //Http
};
}
