#pragma once
#include "DateTime.hpp"
#include "ScheduleWeekDay.hpp"
#include "Const.h"
#include <Arduino.h>

class Schedule {
  private:
    eEventType     _eventType;
    unsigned char   _hour;
    unsigned char   _minute;
    ScheduleWeekDay _weekDays;
    DateTime datetimeWithScheduledTime(DateTime dt);
    bool timeIsOver(DateTime dt);
  public:
    Schedule(eEventType eventType, unsigned char hour, unsigned char minute, ScheduleWeekDay weekDays);
    Schedule(eEventType eventType, unsigned char hour, unsigned char minute);
    unsigned char nextDaysCount(DateTime nowDT);
    DateTime getScheduleDatetime(DateTime nowDT);
    unsigned long getScheduleTimestamp(DateTime nowDT);
    eEventType getEventType();
};
