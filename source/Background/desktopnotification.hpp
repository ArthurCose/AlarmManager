#pragma once

#include <QDBusInterface>

class DesktopNotification : public QObject
{
    Q_OBJECT

public:
    DesktopNotification();
    bool isVisible();
    void setAppName(QString title);
    void setIcon(QString icon);
    void setSummary(QString summary);
    void setBody(QString body);
    void setActions(QMap<QString, QString> actions);
    void addAction(QString identifier, QString localized);
    void addHint(QString key, QVariant value);
    void setTimeout(int ms);
    void show();
    void close();

private:
    bool visible = false;
    uint id = -1;
    QDBusInterface dbus;
    QString appName;
    QString icon;
    QString summary;
    QString body;
    QStringList actionList;
    QVariantMap hints;
    int timeout = 0;
    void connectSlot(QString signal, const char* slot);

signals:
    void closed();
    void action(QString identifier);

private slots:
    void actionInvoked(uint id, QString identifier);
    void notificationClosed(uint id, uint reason);
};

