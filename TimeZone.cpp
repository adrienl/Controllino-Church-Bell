#include "TimeZone.hpp"

#include <Arduino.h>
#include <stdio.h>
#include <string.h>

TimeZone::TimeZone(char * title, int offset) : 
  _title{""},
  _regionalShortName(""),
  _offset(offset),
  _hasDSTOffset(false),
  _regionalShortDSTName(""),
  _dstOffset(0){
  memcpy(_title, title, strlen(title));
 }

TimeZone::TimeZone(char title[16], char * regionalShortName, int offset) :
  _title(""),
  _regionalShortName(""),
  _offset(offset),
  _hasDSTOffset(false),
  _regionalShortDSTName(""),
  _dstOffset(0){
  memcpy(_title, title, strlen(title));
  memcpy(_regionalShortName, regionalShortName, strlen(regionalShortName));    
}

TimeZone::TimeZone(char * title, char * regionalShortName, char * regionalShortDSTName, int offset, int dstOffset) : 
  _title(""),
  _regionalShortName(""),
  _offset(offset),
  _hasDSTOffset(true),
  _regionalShortDSTName(""),
  _dstOffset(dstOffset){
  memcpy(_title, title, strlen(title));
  memcpy(_regionalShortName, regionalShortName, strlen(regionalShortName));
  memcpy(_regionalShortDSTName, regionalShortDSTName, strlen(regionalShortDSTName)); 
}

int TimeZone::getOffsetInMinutes(){
  return _offset;
}

int TimeZone::getDSTOffsetInMinutes(){
  return _dstOffset;
}

void TimeZone::getTitle(char * buff, unsigned char len){
  memcpy(buff, _title, len); 
}

void TimeZone::getRegionalShortDSTName(char * buff, unsigned char len){
  memcpy(buff, _regionalShortDSTName, len); 
}

void TimeZone::getRegionalShortName(char * buff, unsigned char len){
  memcpy(buff, _regionalShortName, len); 
}

bool TimeZone::hasDSTOffset(){
  return _hasDSTOffset;
}

static TimeZone TimeZone::buildEuropeParisTimezone(){
  return TimeZone("Europe/Paris\0", "CET\0", "CEST\0", 60, 120);
}

static TimeZone TimeZone::buildUTC0Timezone(){
  return TimeZone("UTC±00:00\0", 0);
}
