#include "alarm.hpp"

// 5 min in ms
#define ALARM_DURATION 360000

Alarm::Alarm()
{
    notification.setIcon("clock");
    notification.setAppName(tr("Alarm Manager"));
    notification.setSummary(tr("Alarm"));
    notification.addAction("dismiss", tr("dismiss"));
    notification.addHint("suppress-sound", true);
    notification.addHint("urgency", (uchar) 1);
    notification.setTimeout(ALARM_DURATION);

    audioLooper.setPlaybackMode(QMediaPlaylist::Loop);
    audioPlayer.setPlaylist(&audioLooper);
    audioPlayer.setVolume(100);

    connect(&notification, &DesktopNotification::closed, this, &Alarm::dismiss);
}

bool Alarm::isPlaying()
{
    return notification.isVisible();
}

void Alarm::playAlarm(AlarmData alarmData)
{
    displayNotification(alarmData.label);

    if(alarmData.audioPath != "")
        startAudio(alarmData.audioPath);

    decayTimer.setSingleShot(true);
    decayTimer.connect(&decayTimer, &QTimer::timeout, [this]{
        this->dismiss();
    });

    decayTimer.start(ALARM_DURATION);
}

void Alarm::displayNotification(QString label)
{
    notification.setBody(label);
    notification.show();
}

void Alarm::startAudio(QString path)
{
    audioLooper.clear();
    audioLooper.addMedia(QUrl::fromLocalFile(path));
    audioPlayer.play();
}

void Alarm::stopAudio()
{
    audioPlayer.stop();
}

void Alarm::dismiss()
{
    if(notification.isVisible())
        notification.close();

    stopAudio();

    if(decayTimer.isActive())
        decayTimer.stop();

    emit dismissed();
}
