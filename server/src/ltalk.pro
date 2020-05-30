
TARGET = ltalk
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_NO_WARNING_OUTPUT\ QT_NO_DEBUG_OUTPUT #deal with macro name is missing
CONFIG += c++11

HEADERS += \
    ltalk.hh \
    startup.hh \
    db/mysql.hh \
    json/json.hpp\
    process/http.hh \
    \
    net/epoll.hh \
    net/net.hh \
    net/util.hh \
    net/channel.hh \
    net/eventloop.hh \
    net/eventloop_thread.hh \
    net/eventloop_threadpool.hh \
    net/timer.hh \
    \
    thread/noncopyable.hh \
    thread/thread.hh \
    thread/mutex_lock.hh \
    thread/condition.hh \



SOURCES += \
    main.cc \
    startup.cc \
    db/mysql.cc \
    process/http.cc \
    \
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





