#pragma once
#include "DateTime.hpp"
#include "ScheduleWeekDay.hpp"
#include <Arduino.h>

class Schedule {
  private:
    String          _title;
    unsigned char   _hour;
    unsigned char   _minute;
    ScheduleWeekDay _weekDays;
    DateTime datetimeWithScheduledTime(DateTime dt);
    bool timeIsOver(DateTime dt);
  public:
    Schedule(String title, unsigned char hour, unsigned char minute, ScheduleWeekDay weekDays);
    Schedule(String title, unsigned char hour, unsigned char minute);
    unsigned char nextDaysCount(DateTime nowDT);
    DateTime getScheduleDatetime(DateTime nowDT);
    unsigned long getScheduleTimestamp(DateTime nowDT);
    String getTitle();
};
