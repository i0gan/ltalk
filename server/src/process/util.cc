#include "util.hh"


std::unordered_map<std::string, Process::RequestType> Process::Request::umap_type_;
pthread_once_t Process::Request::once_control_;
void Process::Request::Init() {
    Request::umap_type_["login"] = RequestType::login;
    Request::umap_type_["download"] = RequestType::download;
    Request::umap_type_["register"] = RequestType::register_;
    Request::umap_type_["register_success"] = RequestType::register_success;
    Request::umap_type_["keep_connect"] = RequestType::keep_connect;
    Request::umap_type_["get_user_info"] = RequestType::get_user_info;
    Request::umap_type_["get_public_file"] = RequestType::get_public_file;
    Request::umap_type_["get_private_file"] = RequestType::get_private_file;
    Request::umap_type_["upload_profile_image"] = RequestType::upload_profile_image;
}

Process::RequestType Process::Request::toEnum(std::string request) {
    pthread_once(&Request::once_control_, Request::Init);
    RequestType ret_type = RequestType::error;
    try {
        ret_type = Request::umap_type_.at(request);
    } catch (std::out_of_range e) {
        ret_type = RequestType::error;
    }
    return ret_type;
}
