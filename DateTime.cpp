#include "DateTime.hpp"
#include <stdio.h>
#include "Const.h"
#include "DateTimeTool.hpp"
#include <Arduino.h>

DateTime::DateTime(unsigned long timestamp) : _timestamp(timestamp), _timeShift(0) {
  buildDatetime();
}

DateTime::DateTime(unsigned long timestamp, int timeShift) : _timestamp(timestamp), _timeShift(timeShift) {
  buildDatetime();
}

DateTime::DateTime(unsigned int year, unsigned char month, unsigned char day, unsigned char hour, unsigned char minute, unsigned char second) {
  _year = year;
  _month = month;
  _day = day;
  _hour = hour;
  _minute = minute;
  _second = second;
  _timeShift = 0;
  buildTimestamp();
}

DateTime::DateTime(unsigned int year, unsigned char month, unsigned char day, unsigned char hour, unsigned char minute, unsigned char second, int timeShift) {
  _year = year;
  _month = month;
  _day = day;
  _hour = hour;
  _minute = minute;
  _second = second;
  _timeShift = timeShift;
  buildTimestamp();
}

void DateTime::buildTimestamp(){
  _timestamp = DateTimeTool::dateTimeToUTCTimestamp(this);
}

void DateTime::buildDatetime(){
  unsigned long timestampzone = _timestamp + ((unsigned long)SEC_IN_MIN * _timeShift);
  DateTime datetime = DateTimeTool::timestampToDateTime(timestampzone);
  _year = datetime.getYear();
  _month = datetime.getMonth();
  _day = datetime.getDay();
  _hour = datetime.getHour();
  _minute = datetime.getMinute();
  _second = datetime.getSecond();
}

unsigned long DateTime::getUTCTimestamp(){
  return _timestamp;
}

unsigned long DateTime::getLocalTimestamp(){
  return _timestamp + (_timeShift * 60);
}

int DateTime::getTimeShift(){
  return _timeShift;
}

unsigned char DateTime::getYearShort(){
  return _year - ((CURRENT_CENTURY - 1) * 100);
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

unsigned int DateTime::getWeekDay(){
  return DateTimeTool::dayOfWeek(_year, _month, _day);
}

DateTime DateTime::getNextDay(){
  return DateTime(_timestamp + SEC_IN_DAY, _timeShift);
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

void DateTime::fillUltraShortDateStringBuffer(char * buff, unsigned char len){
  snprintf(buff, len, "%02d/%02d\0", getDay(), getMonth());
}