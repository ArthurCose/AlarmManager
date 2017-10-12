TARGET = alarm-manager-daemon

CONFIG += c++11 console
CONFIG -= app_bundle

QT += dbus multimedia
QT -= gui

# Qt 6.0.0+
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += ../Shared
DEPENDPATH += ../Shared
LIBS += -L$$OUT_PWD/../Shared -lShared
win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../Shared/Shared.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../Shared/libShared.a

SOURCES += main.cpp \
    alarmmanager.cpp \
    desktopnotification.cpp \
    alarm.cpp

HEADERS += \
    alarmmanager.hpp \
    desktopnotification.hpp \
    alarm.hpp
