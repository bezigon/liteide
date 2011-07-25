TARGET = liteenv
TEMPLATE = lib

include(../../liteideplugin.pri)
include(../../api/liteenvapi/liteenvapi.pri)

DEFINES += LITEENV_LIBRARY

SOURCES += liteenvplugin.cpp \
    envmanager.cpp \
    liteenvoptionfactory.cpp \
    liteenvoption.cpp

HEADERS += liteenvplugin.h\
        liteenv_global.h \
    envmanager.h \
    liteenvoptionfactory.h \
    liteenvoption.h

FORMS += \
    liteenvoption.ui
