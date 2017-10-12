#include <QDir>
#include "alarmmanager.hpp"
#include "alarm.hpp"

AlarmManager::AlarmManager(QObject *parent) :
    QObject(parent),
    SAVE_FOLDER_PATH(QDir::homePath() + "/.alarms"),
    SAVE_FILE_PATH(SAVE_FOLDER_PATH + "/alarms")
{
    loadAlarms();
}

#include <iostream>
void AlarmManager::loadAlarms()
{
    QFile file(SAVE_FILE_PATH);

    if(!file.exists())
        return;

    file.open(QIODevice::ReadOnly);

    QDataStream stream(&file);

    // get alarms from the file
    while(!stream.atEnd())
    {
        AlarmData alarmData = loadAlarm(stream);

        alarms.insert(alarmData.id, alarmData);

        if(alarmData.enabled)
            createTimer(alarmData);
    }
}

void AlarmManager::saveAlarms()
{
    QDir dir(SAVE_FOLDER_PATH);
    dir.mkpath(".");

    QFile file(SAVE_FILE_PATH);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);

    QDataStream stream(&file);

    for(auto alarmData : alarms)
        saveAlarm(alarmData, stream);
}

AlarmData AlarmManager::loadAlarm(QDataStream& stream)
{
    AlarmData alarmData;

    stream >> alarmData;

    alarmData.id = lastId++;

    return alarmData;
}

void AlarmManager::saveAlarm(AlarmData& alarmData, QDataStream& stream)
{
    stream << alarmData;
}

void AlarmManager::playAlarm(AlarmData alarmData)
{
    if(alarm.isPlaying())
        alarm.dismiss();

    alarm.playAlarm(alarmData);

    if(alarmData.reoccurs())
    {
        deleteTimer(alarmData);
        createTimer(alarmData);
    }
    else
    {
        alarmData.enabled = false;
        modifyAlarm(alarmData);
    }
}

void AlarmManager::createTimer(AlarmData& alarmData)
{
    QTimer* timer = new QTimer();
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, [this, alarmData]{
        this->playAlarm(alarmData);
    });

    timers.insert(alarmData.id, timer);

    timer->start(alarmData.getRemainingSeconds() * 1000);
}

void AlarmManager::deleteTimer(AlarmData& alarmData)
{
    delete timers[alarmData.id];
    timers.remove(alarmData.id);
}


// signals

AlarmMap AlarmManager::getAlarms()
{
    return alarms;
}

int AlarmManager::addAlarm(AlarmData alarmData)
{
    alarmData.id = lastId++;

    alarms.insert(alarmData.id, alarmData);

    if(alarmData.enabled)
        createTimer(alarmData);

    saveAlarms();

    return alarmData.id;
}

void AlarmManager::modifyAlarm(AlarmData alarmData)
{
    // update alarm
    alarms[alarmData.id] = alarmData;

    deleteTimer(alarmData);

    if(alarmData.enabled)
        createTimer(alarmData);

    saveAlarms();
}

void AlarmManager::removeAlarm(AlarmData alarmData)
{
    alarms.remove(alarmData.id);

    deleteTimer(alarmData);

    saveAlarms();
}
