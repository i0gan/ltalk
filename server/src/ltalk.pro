
TARGET = ltalk
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_NO_WARNING_OUTPUT\ QT_NO_DEBUG_OUTPUT #deal with macro name is missing
CONFIG += c++11

SOURCES += \
    main.cc \
    startup.cc \
    db/db.cc \

HEADERS += \
    startup.hh \
    db/db.hh \
    json/json.hpp\


