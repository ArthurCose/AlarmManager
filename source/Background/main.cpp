#include <QCoreApplication>
#include <QDBusInterface>
#include <QDBusMetaType>
#include <iostream>
#include "dbusservicename.hpp"
#include "alarmdata.hpp"
#include "alarmmap.hpp"
#include "alarmmanager.hpp"

int main(int argc, char *argv[])
{
    qRegisterMetaType<AlarmData>();
    qDBusRegisterMetaType<AlarmData>();
    qRegisterMetaType<AlarmMap>();
    qDBusRegisterMetaType<AlarmMap>();

    QCoreApplication app(argc, argv);

    if (!QDBusConnection::sessionBus().isConnected()) {
        std::cerr << "Failed to connect to DBus" << std::endl;
        std::cerr << QDBusConnection::sessionBus().lastError().message().toStdString();
        return 1;
    }

    if (!QDBusConnection::sessionBus().registerService(DBUS_SERVICE_NAME)) {
        std::cerr << QDBusConnection::sessionBus().lastError().message().toStdString();
        return 1;
    }

    AlarmManager alarmManager;
    QDBusConnection::sessionBus().registerObject("/", &alarmManager, QDBusConnection::ExportAllSlots);

    return app.exec();
}
