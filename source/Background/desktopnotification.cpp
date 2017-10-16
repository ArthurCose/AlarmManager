#include <QDBusReply>
#include "desktopnotification.hpp"

DesktopNotification::DesktopNotification() :
    dbus("org.freedesktop.Notifications", "/org/freedesktop/Notifications")
{
    connectSlot("ActionInvoked", SLOT(actionInvoked(uint, QString)));
    connectSlot("NotificationClosed", SLOT(notificationClosed(uint, uint)));
}

void DesktopNotification::connectSlot(QString signal, const char* slot)
{
    QDBusConnection::sessionBus().connect(
        "org.freedesktop.Notifications",
        "/org/freedesktop/Notifications",
        "org.freedesktop.Notifications",
        signal,
        this,
        slot
    );
}

bool DesktopNotification::isVisible()
{
    return visible;
}

void DesktopNotification::setAppName(QString name)
{
    this->appName = name;
}

void DesktopNotification::setIcon(QString icon)
{
    this->icon = icon;
}

void DesktopNotification::setSummary(QString summary)
{
    this->summary = summary;
}

void DesktopNotification::setBody(QString body)
{
    this->body = body;
}

void DesktopNotification::setActions(QMap<QString, QString> actions)
{
    actionList.clear();

    for(auto pair : actions.toStdMap())
        addAction(pair.first, pair.second);
}

void DesktopNotification::addAction(QString identifier, QString localized)
{
    actionList.append(identifier);
    actionList.append(localized);
}

void DesktopNotification::addHint(QString key, QVariant value)
{
    hints.insert(key, value);
}

void DesktopNotification::setTimeout(int ms)
{
    timeout = ms;
}

void DesktopNotification::show()
{
    QDBusReply<uint32_t> reply = dbus.call(
        "Notify",
        appName,
        uint(0),
        icon,
        summary,
        body,
        actionList,
        hints,
        timeout
    );

    id = reply.value();
    visible = true;
}

void DesktopNotification::close()
{
    dbus.call("CloseNotification", id);
    visible = false;
}

void DesktopNotification::actionInvoked(uint id, QString identifier)
{
    if(this->id != id)
        return;

    close();

    emit action(identifier);
    emit closed();
}

void DesktopNotification::notificationClosed(uint id, uint reason)
{
    // notification matches and wasn't programmatically closed to continue
    if(this->id != id || reason == 3)
        return;

    visible = false;

    emit closed();
}
