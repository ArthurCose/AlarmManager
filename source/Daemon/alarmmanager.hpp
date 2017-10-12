#pragma once

#include <QObject>
#include <QMap>
#include <QTimer>
#include <QFile>
#include <QDataStream>
#include "alarmdata.hpp"
#include "alarm.hpp"
#include "alarmmap.hpp"

class AlarmManager : public QObject
{
    Q_OBJECT
public:
    explicit AlarmManager(QObject *parent = nullptr);

private:
    const QString SAVE_FOLDER_PATH;
    const QString SAVE_FILE_PATH;
    AlarmMap alarms;
    QMap<int, QTimer*> timers;
    Alarm alarm;
    int lastId = 0;
    void loadAlarms();
    void saveAlarms();
    AlarmData loadAlarm(QDataStream& stream);
    void saveAlarm(AlarmData& alarmData, QDataStream& stream);
    void createTimer(AlarmData& alarmData);
    void deleteTimer(AlarmData& alarmData);
    void playAlarm(AlarmData alarmData);

public slots:
    Q_SCRIPTABLE AlarmMap getAlarms();
    Q_SCRIPTABLE int addAlarm(AlarmData);
    Q_SCRIPTABLE void modifyAlarm(AlarmData);
    Q_SCRIPTABLE void removeAlarm(AlarmData);
};
