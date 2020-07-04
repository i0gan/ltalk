#include "../ltalk.hh"
#include "../thread/mutex_lock.hh"

class Database::Redis {
public:
    Redis();
    ~Redis();
    bool Connect();
    bool Disconnect();
private:
};

