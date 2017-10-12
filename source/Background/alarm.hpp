#pragma once

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QTimer>
#include "alarmdata.hpp"
#include "desktopnotification.hpp"

class Alarm : public QObject
{
    Q_OBJECT

public:
    Alarm();
    void playAlarm(AlarmData alarmData);
    bool isPlaying();
    void dismiss();
private:
    DesktopNotification notification;
    QMediaPlayer audioPlayer;
    QMediaPlaylist audioLooper;
    QTimer decayTimer;
    void displayNotification(QString label);
    void startAudio(QString path);
    void stopAudio();
signals:
    void dismissed();
};
