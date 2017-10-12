#pragma once

#include <QMainWindow>
#include <QDBusInterface>
#include "alarmwidget.hpp"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QDBusInterface dbus;
    void displayError(QString message);
    void loadAlarms();
    void addAlarmWidget();
    void addAlarmWidget(AlarmData alarmData);
};
