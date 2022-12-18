#pragma once

#include <Arduino.h>

class ScheduleWeekDay{
  private:
    bool _sun;
    bool _mon;
    bool _tue;
    bool _wed;
    bool _thu;
    bool _fri;
    bool _sat;
   public:
    ScheduleWeekDay(bool sun, bool mon, bool tue, bool wed, bool thu, bool fri, bool sat);
    byte toBinary();
};
