#include "RTCManager.hpp"
#include <Controllino.h>
#include <stdio.h>
#include "Const.h"
#include "DateTimeTool.hpp"
#include <Arduino.h>

RTCManager::RTCManager() {

}

void RTCManager::init(){
  Controllino_RTC_init(0);
}

void RTCManager::setUTCTimeDate(unsigned char da, unsigned char we,unsigned char mo, unsigned char ye, unsigned char ho, unsigned char mi, unsigned char se){
  Controllino_SetTimeDate(da,we,mo,ye,ho,mi,se);  
}

void RTCManager::setFromTimestamp(unsigned long ts){
  DateTime dt = DateTimeTool::timestampToDateTime(ts);
  unsigned char day = dt.getDay();
  unsigned char month = dt.getMonth();
  unsigned char shortYear = dt.getYearShort();
  unsigned int year = dt.getYear();
  unsigned char hour = dt.getHour();
  unsigned char minute = dt.getMinute();
  unsigned char second = dt.getSecond();
  unsigned int dayOfWeek = DateTimeTool::dayOfWeek(year, month, day);
  Controllino_SetTimeDate(day, dayOfWeek, month, shortYear, hour, minute, second);  
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

unsigned long RTCManager::getTimestamp(){
  unsigned long ts;
  DateTime datetime(
    getYear(),
    getMonth(),
    getDay(),
    getHour(),
    getMinute(),
    getSecond()
  );
  ts = DateTimeTool::dateTimeToUTCTimestamp(&datetime);
  return ts;
}