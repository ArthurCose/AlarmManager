TEMPLATE = lib
CONFIG += staticlib

QT += dbus
QT -= gui

# Qt 6.0.0+
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
    alarmmap.hpp \
    dbusservicename.hpp \
    alarmdata.hpp

SOURCES += \
    alarmdata.cpp
