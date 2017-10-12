#pragma once

#include <QDBusInterface>
#include <QWidget>
#include "alarmdata.hpp"

namespace Ui {
    class AlarmWidget;
}

class AlarmWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AlarmWidget(QWidget *parent = nullptr);
    void connectToDBus(QDBusInterface& dbus);
    AlarmData getAlarmData();
    void setAlarmData(AlarmData alarmData);
    ~AlarmWidget();

private:
    Ui::AlarmWidget *ui;
    AlarmData alarmData;
    void sendModificationSignal(QDBusInterface& dbus);
    void displayError(QString message);
};
