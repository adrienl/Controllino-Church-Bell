#include "DateTime.hpp"

#include <stdio.h>
#include "DateTimeConst.h"
#include "DateTimeTool.hpp"

#define CURRENT_CENTURY 21

DateTime::DateTime(unsigned long timestamp) {
  _timestamp = timestamp;
  _timezone = 0;
  buildDatetime();
}

DateTime::DateTime(unsigned long timestamp, unsigned char timezone) {
  _timestamp = timestamp;
  _timezone = timezone;
  buildDatetime();
}

DateTime::DateTime(unsigned int year, unsigned char month, unsigned char day, unsigned char hour, unsigned char minute, unsigned char second) {
  _year = year;
  _month = month;
  _day = day;
  _hour = hour;
  _minute = minute;
  _second = second;
  _timezone = 0;
  buildTimestamp();
}

DateTime::DateTime(unsigned int year, unsigned char month, unsigned char day, unsigned char hour, unsigned char minute, unsigned char second, unsigned char timezone) {
  _year = year;
  _month = month;
  _day = day;
  _hour = hour;
  _minute = minute;
  _second = second;
  _timezone = timezone;
  buildTimestamp();
}

void DateTime::buildTimestamp(){
  unsigned long timestampzone = _timestamp + ((unsigned long)SEC_IN_HOUR * _timezone);
  unsigned long ts = DateTimeTool::dateTimeToTimestamp(this);
  _timestamp = ts;
}

void DateTime::buildDatetime(){
  unsigned long timestampzone = _timestamp + ((unsigned long)SEC_IN_HOUR * _timezone);
  DateTime datetime = DateTimeTool::timestampToDateTime(timestampzone);
   
  _yearShort =  datetime.getYear() - ((CURRENT_CENTURY - 1) * 100);
  _year = datetime.getYear();
  _month = datetime.getMonth();
  _day = datetime.getDay();
  _hour = datetime.getHour();
  _minute = datetime.getMinute();
  _second = datetime.getSecond();
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

unsigned int DateTime::getYear(){
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
  snprintf(buff, len, "%02d/%02d/%04d\0", getDay(), getMonth(), getYear());
}

void DateTime::fillShortDateStringBuffer(char * buff, unsigned char len){
  snprintf(buff, len, "%02d/%02d/%02d\0", getDay(), getMonth(), getYearShort());
}
