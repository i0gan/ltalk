TARGET = ltalk
CONFIG += c++11

HEADERS += \
    ltalk.hh \
    startup.hh \
    \
    database/mysql/mysql.hh \
    database/redis/redis.hh \
    database/mongo/mongo.hh \
    \
    json/json.hpp\
    process/center.hh \
    \
    net/http.hh \
    net/epoll.hh \
    net/net.hh \
    net/util.hh \
    net/channel.hh \
    net/eventloop.hh \
    net/eventloop_thread.hh \
    net/eventloop_threadpool.hh \
    net/timer.hh \
    net/vessel.hpp \
    \
    thread/noncopyable.hh \
    thread/thread.hh \
    thread/mutex_lock.hh \
    thread/condition.hh \
    thread/count_down_latch.hh \
    \
    crypto/sha256.hh \
    crypto/md5.hh \

SOURCES += \
    main.cc \
    startup.cc \
    \
    database/mysql/mysql.cc \
    database/redis/redis.cc \
    database/mongo/mongo.cc \
    \
    process/center.cc \
    \
    net/http.cc \
    net/epoll.cc \
    net/net.cc \
    net/util.cc \
    net/channel.cc \
    net/eventloop.cc \
    net/eventloop_thread.cc \
    net/eventloop_threadpool.cc \
    net/timer.cc \
    \
    thread/thread.cc \
    thread/mutex_lock.cc \
    thread/condition.cc \
    thread/count_down_latch.cc \
    \
    crypto/md5.cc \





