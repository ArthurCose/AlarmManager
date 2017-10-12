#include <QApplication>
#include <QMessageBox>
#include <QDBusInterface>
#include <QDBusMetaType>
#include "mainwindow.hpp"
#include "alarmdata.hpp"
#include "alarmmap.hpp"

int main(int argc, char *argv[])
{
    qRegisterMetaType<AlarmData>();
    qDBusRegisterMetaType<AlarmData>();
    qRegisterMetaType<AlarmMap>();
    qDBusRegisterMetaType<AlarmMap>();

    QApplication app(argc, argv);

    if (!QDBusConnection::sessionBus().isConnected()) {
        QMessageBox errorDialog;
        errorDialog.critical(0, "error", "Could not connect to dbus");
        return 1;
    }

    MainWindow w;
    w.show();

    return app.exec();
}
