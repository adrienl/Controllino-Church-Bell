#include "ScheduleWeekDay.hpp"

ScheduleWeekDay::ScheduleWeekDay(bool sun, bool mon, bool tue, bool wed, bool thu, bool fri, bool sat) :
  _sun(sun), _mon(mon), _tue(tue), _wed(wed), _thu(thu), _fri(fri), _sat(sat)
{
  
}

byte ScheduleWeekDay::toBinary(){
  byte val = 0;
  if (_sun){val |= 0b00000001;}
  if (_mon){val |= 0b00000010;}
  if (_tue){val |= 0b00000100;}
  if (_wed){val |= 0b00001000;}
  if (_thu){val |= 0b00010000;}
  if (_fri){val |= 0b00100000;}
  if (_sat){val |= 0b01000000;}
  return val;  
}
