#include "TimeZone.hpp"

#include <stdio.h>
#include <string.h>

TimeZone::TimeZone(char title[16], int offset) : 
  _title{title},
  _regionalShortName{0},
  _offset(offset),
  _hasSummerOffset(false),
  _regionalShortSummerName{0},
  _summerOffset(0){}

TimeZone::TimeZone(char title[16], char regionalShortName[4], int offset) :
  _title(title),
  _regionalShortName(regionalShortName),
  _offset(offset),
  _hasSummerOffset(false),
  _regionalShortSummerName{0},
  _summerOffset(0){}

TimeZone::TimeZone(char title[16], char regionalShortName[4], char regionalShortSummerName[5], int offset, int summerOffset) : 
  _title(title),
  _regionalShortName(regionalShortName),
  _offset(offset),
  _hasSummerOffset(true),
  _regionalShortSummerName{regionalShortSummerName},
  _summerOffset(summerOffset){}

int TimeZone::getUTCOffsetInMinutes(){
  return _offset;
}

int TimeZone::getSummerUTCOffsetInMinutes(){
  return _summerOffset;
}

void TimeZone::getTitle(char * buff, unsigned char len){
  snprintf(buff, len, "%s\0", _title);
}

void TimeZone::getRegionalShortSummerName(char * buff, unsigned char len){
  snprintf(buff, len, "%s\0", _regionalShortSummerName);
}

void TimeZone::getRegionalShortName(char * buff, unsigned char len){
  snprintf(buff, len, "%s\0", _regionalShortName);
}

bool TimeZone::changeOffsetInSummer(){
  return _hasSummerOffset;
}

static TimeZone TimeZone::buildEuropeParisTimezone(){
  return TimeZone("Europe/Paris\0", "CET\0", "CEST\0", 60, 120);
}

static TimeZone TimeZone::buildUTC0Timezone(){
  return TimeZone("UTC±00:00\0", 0);
}
