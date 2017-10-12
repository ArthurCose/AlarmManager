#pragma once

#include <QMap>
#include "alarmdata.hpp"

typedef QMap<int, AlarmData> AlarmMap;

Q_DECLARE_METATYPE(AlarmMap)
