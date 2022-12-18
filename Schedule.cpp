#include "Schedule.hpp"
#include <Arduino.h>

Schedule::Schedule(E_EventType eventType, unsigned char hour, unsigned char minute, ScheduleWeekDay weekDays) :
  _eventType(eventType), _hour(hour), _minute(minute), _weekDays(weekDays){
  
}

Schedule::Schedule(E_EventType eventType, unsigned char hour, unsigned char minute) :
  _eventType(eventType), _hour(hour), _minute(minute), _weekDays(ScheduleWeekDay(1, 1, 1, 1, 1, 1, 1)){
    
}

DateTime Schedule::datetimeWithScheduledTime(DateTime dt){
  unsigned char nDay = dt.getDay();
  unsigned char nMon = dt.getMonth();
  unsigned int  nYear = dt.getYear();
  int nShift = dt.getTimeShift();
  return DateTime(nYear, nMon, nDay, _hour, _minute, 0, nShift);
}

E_EventType Schedule::getEventType(){
  return _eventType;
}

bool Schedule::timeIsOver(DateTime dt){
  unsigned char nDay = dt.getDay();
  unsigned char nMon = dt.getMonth();
  unsigned int  nYear = dt.getYear();
  unsigned char nHour = dt.getHour();
  unsigned char nMinute = dt.getMinute();
  int nShift = dt.getTimeShift();
  DateTime sheduleDT = DateTime(nYear, nMon, nDay, _hour, _minute, 0, nShift);
  return dt.getUTCTimestamp() > sheduleDT.getUTCTimestamp();
}

int mypow(int base, int exponent){
  int i = 1;
  int result = base;
  if (base < 0 && exponent == 0) {return -1;}
  if (base >= 0 && exponent == 0) {return 1;}
  if (exponent == 1){return base;}
  while (i < exponent){
    result *= base; 
    i++;
  }
  return result;
}

unsigned char Schedule::nextDaysCount(DateTime dt){
  bool dayFound = false;
  unsigned char i = 0;
  unsigned int todayWeekDay = dt.getWeekDay();
  byte weekDaysAvailable = _weekDays.toBinary();
  char todayWeekDayBin = mypow(2, todayWeekDay);
  bool nextWeek = weekDaysAvailable < todayWeekDayBin;

  if (!nextWeek){
    while (!dayFound && i < 7){
      if (todayWeekDayBin & (weekDaysAvailable >> i)){
        dayFound = true;
        continue;
      }
      i++;  
    }
    return i;
  }else{
    while (!dayFound && i < 7){
      if (1 & (weekDaysAvailable >> i)){
        dayFound = true;
        continue;
      }
      i++;
    }
    return 7 - todayWeekDay + i;
  }
  
}

DateTime Schedule::getScheduleDatetime(DateTime nowDT){
  unsigned int i;
  unsigned int days = 0;
  DateTime scheduledDT = DateTime(1970, 1, 1, 0, 0, 0);
  if (timeIsOver(nowDT)){
    scheduledDT = nowDT.getNextDay();
    scheduledDT = datetimeWithScheduledTime(scheduledDT);
  }else{
    scheduledDT = datetimeWithScheduledTime(nowDT);
  }
  days = nextDaysCount(scheduledDT);
  i = 0;
  while (i < days){
    scheduledDT = scheduledDT.getNextDay();
    i++;
  }
  return scheduledDT;
}

unsigned long Schedule::getScheduleTimestamp(DateTime nowDT){
  DateTime dt = getScheduleDatetime(nowDT);
  return dt.getUTCTimestamp();
}
