#include "util.hh"


std::unordered_map<std::string, Work::RequestType> Work::Request::umap_type_;
pthread_once_t Work::Request::once_control_;
void Work::Request::Init() {
    Request::umap_type_["main_page"] = RequestType::main_page;
    Request::umap_type_["register_page"] = RequestType::register_page;
    Request::umap_type_["register_success_page"] = RequestType::register_success_page;
    Request::umap_type_["login"] = RequestType::login;
    Request::umap_type_["register"] = RequestType::register_;
    Request::umap_type_["keep_connect"] = RequestType::keep_connect;
    Request::umap_type_["get_user_info"] = RequestType::get_user_info;
    Request::umap_type_["get_public_file"] = RequestType::get_public_file;
    Request::umap_type_["get_private_file"] = RequestType::get_private_file;
    Request::umap_type_["upload_profile_image"] = RequestType::upload_profile_image;
    Request::umap_type_["get_profile_image"] = RequestType::get_profile_image;
    Request::umap_type_["search_user"] = RequestType::search_user;
    Request::umap_type_["add_user"] = RequestType::add_user;
}

Work::RequestType Work::Request::toEnum(std::string request) {
    pthread_once(&Request::once_control_, Request::Init);
    RequestType ret_type = RequestType::error;
    try {
        ret_type = Request::umap_type_.at(request);
    } catch (std::out_of_range e) {
        ret_type = RequestType::error;
    }
    return ret_type;
}


