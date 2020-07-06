#include "redis.hh"

redisContext *Database::global_redis = nullptr;

Database::Redis::Redis() {

}

Database::Redis::~Redis() {

}

bool Database::Redis::Connect(std::string ip, int port) {
    redisContext *redis_context = redisConnect(ip.c_str(), port);
    if(redis_context->err) {
        redisFree(redis_context);
        std::cout << "connect redis err\n";
        return false;
    }

    global_redis = redis_context;
    return true;
}

bool Database::Redis::Disconnect() {
    if(global_redis) {
        redisFree(global_redis);
        global_redis = nullptr;
    }
    return true;
}

std::string Database::Redis::Get(std::string key) {
    if(!global_redis) {
        std::cout << "redis not connected!\n";
        return "none";
    }
    std::string exec = "GET " + key;
    redisReply *get_reply = (redisReply*)redisCommand(global_redis, exec.c_str());
    return get_reply->str;
}
