#include "center.hh"
extern MYSQL* global_mysql_ptr;

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
    std::string type = ".html";
    std::string content = "hello world";
    HandleWrite(type, content);
}

void Ltalk::Center::set_write_handler(WriteCallBack write_handler) {
    write_handler_ = write_handler;
}
