#include <QDateTime>
#include "alarmdata.hpp"

bool AlarmData::reoccurs()
{
    return sunday | monday | tuesday | wednesday |
           thursday | friday | saturday;
}

int AlarmData::getFirstRecurrenceOfTheWeek()
{
    if(sunday)
        return 1;
    if(monday)
        return 2;
    if(tuesday)
        return 3;
    if(wednesday)
        return 4;
    if(thursday)
        return 5;
    if(friday)
        return 6;
    if(saturday)
        return 7;
    return -1;
}

int AlarmData::getRemainingDays()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    int dayOfTheWeek = currentDateTime.date().dayOfWeek();

    // start the date for the alarm on sunday
    QDateTime alarmDateTime = currentDateTime.addDays(0 - dayOfTheWeek);
    alarmDateTime.setTime(
        QTime(hour, minute)
    );

    if(!reoccurs())
    {
        // time is set to later today, so remaining days is 0
        if(currentDateTime.time() < alarmDateTime.time())
            return 0;

        // time is set to earlier today
        // this must be actually referring to tomorrow as
        // we cant set an alarm to the past
        return 1;
    }

    // find a recurrence datetime that is after the current datetime

    bool reoccur[] = {sunday, monday, tuesday, wednesday, thursday, friday, saturday};

    for(int i = 0; i < 7; ++i)
    {
        if(reoccur[i] && currentDateTime < alarmDateTime)
            return currentDateTime.daysTo(alarmDateTime);
        alarmDateTime = alarmDateTime.addDays(1);
    }

    // alarm will go off on the first recurrence of next week
    return getFirstRecurrenceOfTheWeek() + 7 - dayOfTheWeek;
}

int AlarmData::getRemainingSeconds()
{
    QDateTime now = QDateTime::currentDateTime();

    QDateTime setDateTime(
        now.date().addDays(getRemainingDays()),
        QTime(hour, minute)
    );

    return now.secsTo(setDateTime);
}

QDataStream& operator<<(QDataStream& stream, const AlarmData& alarmData)
{
    AlarmData::streamOut(stream, alarmData);

    return stream;
}

QDataStream& operator>>(QDataStream& stream, AlarmData& alarmData)
{
    AlarmData::streamIn(stream, alarmData);

    return stream;
}

QDBusArgument& operator<<(QDBusArgument& stream, const AlarmData& alarmData)
{
    stream.beginStructure();
    AlarmData::streamOut(stream, alarmData);
    stream.endStructure();

    return stream;
}

const QDBusArgument& operator>>(const QDBusArgument& stream, AlarmData& alarmData)
{
    stream.beginStructure();
    AlarmData::streamIn(stream, alarmData);
    stream.endStructure();

    return stream;
}

template<class T>
void AlarmData::streamOut(T& stream, const AlarmData& alarmData)
{
    stream << alarmData.label;
    stream << alarmData.audioPath;
    stream << alarmData.enabled;
    stream << alarmData.id;
    stream << alarmData.hour;
    stream << alarmData.minute;
    stream << alarmData.sunday;
    stream << alarmData.monday;
    stream << alarmData.tuesday;
    stream << alarmData.wednesday;
    stream << alarmData.thursday;
    stream << alarmData.friday;
    stream << alarmData.saturday;
}

template<class T>
void AlarmData::streamIn(T& stream, AlarmData& alarmData)
{
    stream >> alarmData.label;
    stream >> alarmData.audioPath;
    stream >> alarmData.enabled;
    stream >> alarmData.id;
    stream >> alarmData.hour;
    stream >> alarmData.minute;
    stream >> alarmData.sunday;
    stream >> alarmData.monday;
    stream >> alarmData.tuesday;
    stream >> alarmData.wednesday;
    stream >> alarmData.thursday;
    stream >> alarmData.friday;
    stream >> alarmData.saturday;
}
