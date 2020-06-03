#include "../ltalk.hh"
#include "../db/mysql/mysql.hh"
#include <map>
#include <string>

namespace Ltalk {
class Center {
public:
    Center(const std::map<std::string, std::string> &map_header_info, std::string &content);
    Center(const std::map<std::string, std::string> &map_header_info, std::string &content, WriteCallBack write_handler);
    ~Center();
    void Process();
    void set_write_handler(WriteCallBack write_handler);

private:
    const std::map<std::string, std::string> &map_header_info_;
    std::string &content_;
    WriteCallBack write_handler_;
    void HandleWrite(const std::string &type, const std::string &content);
};
}
