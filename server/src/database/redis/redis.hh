#include "../../ltalk.hh"

class Database::Redis {
public:
    Redis();
    ~Redis();
    bool Connect();
    bool Disconnect();
private:
};

