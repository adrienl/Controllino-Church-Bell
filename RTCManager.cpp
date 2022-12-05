#include "RTCManager.hpp"
#include <Controllino.h>
#include <stdio.h>
#include "DateTimeConst.h"
#include "DateTimeTool.hpp"

RTCManager::RTCManager() {

}

void RTCManager::init(){
  Controllino_RTC_init(0);
}

void RTCManager::setUTCTimeDate(unsigned char da, unsigned char we,unsigned char mo, unsigned char ye, unsigned char ho, unsigned char mi, unsigned char se){
  Controllino_SetTimeDate(da,we,mo,ye,ho,mi,se);  
}

char RTCManager::getDay(){
  return Controllino_GetDay();
}

char RTCManager::getMonth(){
  return Controllino_GetMonth();
}

char RTCManager::getShortYear(){
  return  Controllino_GetYear();
}

int RTCManager::getYear(){
  return ((CURRENT_CENTURY - 1) * 100) + getShortYear();
}

char RTCManager::getWeekDay(){
  return Controllino_GetWeekDay();
}

char RTCManager::getHour(){
  return Controllino_GetHour();
}

char RTCManager::getMinute(){
  return Controllino_GetMinute();
}

char RTCManager::getSecond(){
  return Controllino_GetSecond();
}

unsigned long RTCManager::secsPastFrom1970ToCurYear(){ //Private
  unsigned int leapYearsLength;
  unsigned long leapSec1970ToCurY;
  unsigned long stdrSec1970ToCurY;
  unsigned long sec1970ToCurY;
  unsigned int yearsPast;
  unsigned int currentYear;

  currentYear = getYear();
  yearsPast = currentYear - TIMESTAMP_ZERO_YEAR;
  leapYearsLength = DateTimeTool::leapYearsPastSince1970(currentYear);
  leapSec1970ToCurY = SEC_IN_LEAP_YEAR * leapYearsLength;
  stdrSec1970ToCurY = SEC_IN_STDR_YEAR * (currentYear - TIMESTAMP_ZERO_YEAR - leapYearsLength);
  sec1970ToCurY = stdrSec1970ToCurY + leapSec1970ToCurY;

  return sec1970ToCurY;
}

unsigned long RTCManager::secsPastFromJanToLastMonth(){ //Private
  char currentMonthIdx = getMonth() - 1;
  unsigned long secondsPast = 0;
  unsigned char i = 0;
  while (i < currentMonthIdx){
    secondsPast += DAYS_MONTH[i] * SEC_IN_DAY;
    i++;
  }
  return secondsPast;
}

unsigned long RTCManager::secsPastFromFirstDayOfMonthToCurDay(){ //Private
  unsigned char daysPast = getDay() - 1;
  return SEC_IN_DAY * daysPast;
}

unsigned long RTCManager::secsPastFromMidnight(){ //Private
  unsigned long secsPast = 0;
  char h = getHour();
  char m = getMinute();
  char s = getSecond();
  unsigned long th = h * (unsigned long)(SEC_IN_HOUR);
  int tm = m * SEC_IN_MIN;
  secsPast = th + tm + s;
  return secsPast;
}

unsigned long RTCManager::getTimestamp(){
  unsigned long ts;
  unsigned long yearSecPast = secsPastFrom1970ToCurYear();
  unsigned long monthSecPast = secsPastFromJanToLastMonth();
  unsigned long daysSecPast = secsPastFromFirstDayOfMonthToCurDay();
  unsigned long timeSecPast = secsPastFromMidnight();
  ts = yearSecPast + monthSecPast + daysSecPast + timeSecPast;
  return ts;
}
