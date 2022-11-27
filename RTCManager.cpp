#include "RTCManager.hpp"
#include <Controllino.h>
#include <stdio.h>

RTCManager::RTCManager() {

}


void RTCManager::init(){
  Controllino_RTC_init(0);
}

void RTCManager::setTimeDate(unsigned char da, unsigned char we,unsigned char mo, unsigned char ye, unsigned char ho, unsigned char mi, unsigned char se){
  Controllino_SetTimeDate(da,we,mo,ye,ho,mi,se);  
}

void RTCManager::setCompilationTimeDate(){
  Controllino_SetTimeDateStrings(__DATE__, __TIME__);
}

char RTCManager::getDay(){
  return Controllino_GetDay();
}

char RTCManager::getMonth(){
  return Controllino_GetMonth();
}

char RTCManager::getYear(){
  return Controllino_GetYear();
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

char RTCManager::addSeconds(unsigned char s){
  
}

char RTCManager::removeSeconds(unsigned char s){
  
}

void RTCManager::fillTimeStringBuffer(char * buff, unsigned char len){
  snprintf(buff, len, "%02d:%02d:%02d\0", getHour(), getMinute(), getSecond());
}
void RTCManager::fillDateStringBuffer(char * buff, unsigned char len){
  snprintf(buff, len, "%02d/%02d/%02d\0", getDay(), getMonth(), getYear());
}
