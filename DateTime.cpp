#include "DateTime.hpp"

#include <stdio.h>
#include "DateTimeConst.h"
#include "DateTimeTool.hpp"

#define CURRENT_CENTURY 21

DateTime::DateTime(unsigned long timestamp, unsigned char timezone) {
  _timestamp = timestamp;
  _timezone = timezone;
  build();
}

void DateTime::build(){
  ALDateTime alDateTime;
  unsigned long timestampzone = _timestamp + ((unsigned long)SEC_IN_HOUR * _timezone);
  DateTimeTool::timestampToDateTime(timestampzone, &alDateTime);
   
  _yearShort =  alDateTime.year - (CURRENT_CENTURY - 1 * 100);
  _year = alDateTime.year;
  _month = alDateTime.month;
  _day = alDateTime.day;
  _hour = alDateTime.hour;
  _minute = alDateTime.minute;
  _second = alDateTime.second;
}

unsigned long DateTime::getTimestamp(){
  return _timestamp;
}

unsigned char DateTime::getTimezone(){
  return _timezone;
}

unsigned char DateTime::getYearShort(){
  return _yearShort;
}

unsigned char DateTime::getYear(){
  return _year;
}

unsigned char DateTime::getMonth(){
  return _month;
}

unsigned char DateTime::getDay(){
  return _day;
}

unsigned char DateTime::getHour(){
  return _hour;
}

unsigned char DateTime::getMinute(){
  return _minute;
}

unsigned char DateTime::getSecond(){
  return _second;
}

void DateTime::fillTimeStringBuffer(char * buff, unsigned char len){
  snprintf(buff, len, "%02d:%02d:%02d\0", getHour(), getMinute(), getSecond());
}

void DateTime::fillShortTimeStringBuffer(char * buff, unsigned char len){
  snprintf(buff, len, "%02d:%02d\0", getHour(), getMinute());
}

void DateTime::fillDateStringBuffer(char * buff, unsigned char len){
  snprintf(buff, len, "%02d/%02d/%02d\0", getDay(), getMonth(), getYear());
}
