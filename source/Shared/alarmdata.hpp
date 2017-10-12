#pragma once

#include <QDataStream>
#include <QDBusArgument>
#include <iostream>

class AlarmData
{
public:
    int id;
    bool enabled = true;
    int hour = 0;
    int minute = 0;
    QString label = "";
    QString audioPath = "";

    // recurrences
    bool sunday = false;
    bool monday = false;
    bool tuesday = false;
    bool wednesday = false;
    bool thursday = false;
    bool friday = false;
    bool saturday = false;

    bool reoccurs();
    int getFirstRecurrenceOfTheWeek();
    int getRemainingDays();
    int getRemainingSeconds();

    friend QDataStream& operator<<(QDataStream&, const AlarmData&);
    friend QDataStream& operator>>(QDataStream&, AlarmData&);
    friend QDBusArgument& operator<<(QDBusArgument& stream, const AlarmData& alarm);
    friend const QDBusArgument& operator>>(const QDBusArgument& stream, AlarmData& alarm);

private:
    template<class T>
    static void streamOut(T&, const AlarmData&);
    template<class T>
    static void streamIn(T&, AlarmData&);
};

Q_DECLARE_METATYPE(AlarmData)
