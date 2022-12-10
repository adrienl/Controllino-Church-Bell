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

unsigned long RTCManager::getTimestamp(){
  DateTime datetime(
    getYear(),
    getMonth(),
    getDay(),
    getHour(),
    getMinute(),
    getSecond()
  );
 
  unsigned long ts;
  ts = DateTimeTool::dateTimeToTimestamp(&datetime);
  return ts;
}
