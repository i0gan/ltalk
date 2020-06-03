#include "center.hh"
extern MYSQL* global_mysql_ptr;
using json = nlohmann::json;

Ltalk::Center::Center(const std::map<std::string, std::string> &map_header_info, std::string &content) :
    map_header_info_(map_header_info),
    content_(content),
    write_handler_(nullptr) {

}

Ltalk::Center::Center(const std::map<std::string, std::string> &map_header_info, std::string &content, WriteCallBack write_handler) :
    map_header_info_(map_header_info),
    content_(content) ,
    write_handler_(write_handler) {

}

Ltalk::Center::~Center() {

}

void Ltalk::Center::HandleWrite(const std::string &type, const std::string &content) {
    if(write_handler_)
        write_handler_(type, content);
}

void Ltalk::Center::Process() {
    try {
        std::cout << "method" << map_header_info_.at("method");

    }  catch (std::out_of_range e) {
        std::cout << "catch " << e.what() << '\n';
    }
    json j = {
        {"Name" , "I0gan"},
        {"Age" , 18 }
    };
    std::strstream str_stream;
    str_stream << j;
    std::string type = ".html";
    std::string content = str_stream.str();
    HandleWrite(type, content);
}

void Ltalk::Center::set_write_handler(WriteCallBack write_handler) {
    write_handler_ = write_handler;
}
