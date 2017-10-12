TARGET = alarm-manager-gui
TEMPLATE = app

QT += core gui dbus
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Qt 6.0.0+
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += ../Shared
DEPENDPATH += ../Shared
LIBS += -L$$OUT_PWD/../Shared -lShared
win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../Shared/Shared.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../Shared/libShared.a

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    alarmwidget.cpp

HEADERS += \
    mainwindow.hpp \
    alarmwidget.hpp

FORMS += \
    mainwindow.ui \
    alarmwidget.ui
