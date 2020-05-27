
TARGET = ltalk
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_NO_WARNING_OUTPUT\ QT_NO_DEBUG_OUTPUT #deal with macro name is missing
CONFIG += c++11

SOURCES += \
    main.cpp \
    startup.cpp \
    core/network.cpp \
    core/epoll.cpp \

HEADERS += \
    startup.h \
    core/network.h \
    core/epoll.h \


