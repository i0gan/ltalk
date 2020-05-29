
TARGET = ltalk
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_NO_WARNING_OUTPUT\ QT_NO_DEBUG_OUTPUT #deal with macro name is missing
CONFIG += c++11

HEADERS += \
    startup.hh \
    db/db.hh \
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


SOURCES += \
    main.cc \
    startup.cc \
    db/db.cc \
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





