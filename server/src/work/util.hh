#pragma once
#include "../ltalk.hh"

enum class Work::ResponseCode {
    SUCCESS = 0,
    FAILURE,
    NOT_FOUND,
    INTERNAL_ERROR,
    NO_ACCESS,
    NO_PERMISSSION = 5,
    ERROR_PARSING_URL,
    ERROR_PARSING_CONTENT,
    ERROR_HTTP_CONTENT,
    ERROR_JSON_CONTENT_TYPE,
    ERROR_JSON_CONTENT = 10,
    EXIST,
    NOT_EXIST,
    LOGINED,
};

enum class Work::RequestType {
    error = 0,
    main_page,
    register_page,
    register_success_page,
    login,
    register_,
    keep_connect,
    get_user_info,
    get_public_file,
    get_private_file,
    upload_profile_image,
    get_profile_image,
    search_user,
};

enum class Work::Platform {
    unknown = 0,
    linux_,
    windows,
    android,
};

namespace Work {
class Request {
public:
    static RequestType toEnum(std::string request);

private:
    static void Init();
    static std::unordered_map<std::string, RequestType> umap_type_;
    static pthread_once_t once_control_;
};
}


