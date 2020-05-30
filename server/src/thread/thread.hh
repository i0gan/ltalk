#include "noncopyable.hh"
#include "../ltalk.hh"
#include <string>

namespace Ltalk {
class Thread : Noncopyable {
public:
    explicit Thread(const CallBack &call_back, const std::string &name = std::string());
    ~Thread();
    void Start();
    int Join();
    bool IsStarted();
    pid_t get_tid;
    const std::string & get_name();
};
}
