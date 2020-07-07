#include "../ltalk.hh"
#include "../thread/mutex_lock.hh"
#include <hiredis/hiredis.h>

namespace Database {
extern redisContext *global_redis;
class Redis{
public:
    Redis();
    ~Redis();
    static bool Connect(std::string ip = "127.0.0.1", int port = 6379);
    static bool Disconnect();
    std::string Get(std::string key);

private:
};

}

