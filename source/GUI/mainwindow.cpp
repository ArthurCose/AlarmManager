#include <QPushButton>
#include <QDBusReply>
#include <QMessageBox>
#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "dbusservicename.hpp"
#include "alarmdata.hpp"
#include "alarmmap.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    dbus(DBUS_SERVICE_NAME, "/")
{
    ui->setupUi(this);

    loadAlarms();

    connect(ui->newButton, &QPushButton::clicked,
            [this]{this->addAlarmWidget();});
}

void MainWindow::loadAlarms()
{
    if (!dbus.isValid())
    {
        displayError("invalid dbus connection");
        exit(1);
    }

    QDBusReply<AlarmMap> reply = dbus.call("getAlarms");

    if(!reply.isValid())
    {
        displayError(reply.error().message());
        exit(1);
    }

    for(auto alarmData : reply.value())
        addAlarmWidget(alarmData);
}

void MainWindow::addAlarmWidget()
{
    AlarmData alarmData;
    QDBusReply<int> reply = dbus.call("addAlarm", qVariantFromValue(alarmData));

    if(!reply.isValid())
    {
        displayError(reply.error().message());
        return;
    }

    alarmData.id = reply.value();

    // add defaults

    addAlarmWidget(alarmData);
}

void MainWindow::addAlarmWidget(AlarmData alarmData)
{
    AlarmWidget* alarmWidget = new AlarmWidget();
    alarmWidget->setAlarmData(alarmData);
    alarmWidget->connectToDBus(dbus);
    ui->alarmList->addWidget(alarmWidget);
}

void MainWindow::displayError(QString message)
{
    QMessageBox errorBox;
    errorBox.critical(this, "error", message);
}

MainWindow::~MainWindow()
{
    delete ui;
}
