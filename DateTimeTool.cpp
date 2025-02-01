#include "DateTimeTool.hpp"
#include <stdio.h>
#include <math.h>
#include "DateTime.hpp"
#include "Const.h"
#include <time.h>

/*
* Public
*/

static bool DateTimeTool::getIsLeapYear(unsigned int year){ //Est-ce une année bissextile ?
  if (year % 4 == 0){
    return true;
  }
  if (year % 100 == 0){
    return true;
  }
  if (year % 400 == 0){
    return true;
  }
  return false;
}

/*
 * Private
 */

static unsigned int DateTimeTool::leapYearsPastSince1970(unsigned int yearToCompare){
  unsigned int yearsPast = yearToCompare - TIMESTAMP_ZERO_YEAR;
  unsigned int leapYearsLength = 0;
  unsigned int i = 0;
  while (i <  yearsPast){
    if (getIsLeapYear(TIMESTAMP_ZERO_YEAR + i) == true){
      leapYearsLength++;
    }
    i++;
  }
  return leapYearsLength;
}

static unsigned long DateTimeTool::secsPastFrom1970ToCurYear(unsigned int currentYear){ //Private
  unsigned int leapYearsLength;
  unsigned long leapSec1970ToCurY;
  unsigned long stdrSec1970ToCurY;
  unsigned long sec1970ToCurY;
  unsigned int yearsPast;
 
  yearsPast = currentYear - TIMESTAMP_ZERO_YEAR;
  leapYearsLength = DateTimeTool::leapYearsPastSince1970(currentYear);
  leapSec1970ToCurY = SEC_IN_LEAP_YEAR * leapYearsLength;
  stdrSec1970ToCurY = SEC_IN_STDR_YEAR * (currentYear - TIMESTAMP_ZERO_YEAR - leapYearsLength);
  sec1970ToCurY = stdrSec1970ToCurY + leapSec1970ToCurY;

  return sec1970ToCurY;
}

static unsigned long DateTimeTool::secsPastFromJanToLastMonth(unsigned int currentYear, unsigned char currentMonth){ //Private
  unsigned char currentMonthIdx = currentMonth - 1;
  unsigned long secondsPast = 0;
  unsigned char i = 0;
  bool isLeapYear = getIsLeapYear(currentYear);
  if (isLeapYear){
    while (i < currentMonthIdx){
      secondsPast += DAYS_LPMONTH[i] * SEC_IN_DAY;
      i++;
    }
  }else{
    while (i < currentMonthIdx){
      secondsPast += DAYS_SDMONTH[i] * SEC_IN_DAY;
      i++;
    }
  }

  return secondsPast;
}

static unsigned long DateTimeTool::secsPastFromFirstDayOfMonthToCurDay(unsigned char currentDay){ //Private
  unsigned char daysPast = currentDay - 1;
  return SEC_IN_DAY * daysPast;
}

static unsigned long DateTimeTool::secsPastFromMidnight(unsigned char h, unsigned char m, unsigned char s){ //Private
  unsigned long secsPast = 0;
  unsigned long th = h * (unsigned long)(SEC_IN_HOUR);
  int tm = m * SEC_IN_MIN;
  secsPast = th + tm + s;
  return secsPast;
}

static unsigned int DateTimeTool::dayOfWeek(unsigned int y, unsigned int m, unsigned int d){
    unsigned int total;
    unsigned int yM;
    unsigned int vD;
    if (m < 3){
        yM = y - 1;
        vD = 4;
    }else{
        yM = y;
        vD = 2;
    }
    unsigned int xA = floor(23 * m / 9);
    unsigned int xB = floor(yM / 4);
    unsigned int xC = floor(yM / 100);
    unsigned int xD = floor(yM / 400);
    total = (xA + d + vD + y + xB - xC + xD) % 7;
    return total;
}

/*
 * Public
 */

static unsigned long DateTimeTool::dateTimeToLocalTimestamp(DateTime * datetime){
  unsigned long ts;
  unsigned int year = datetime->getYear();
  unsigned char month = datetime->getMonth();
  unsigned char day = datetime->getDay();
  unsigned char hour = datetime->getHour();
  unsigned char minute = datetime->getMinute();
  unsigned char second = datetime->getSecond();
  unsigned long yearSecPast = secsPastFrom1970ToCurYear(year);
  unsigned long monthSecPast = secsPastFromJanToLastMonth(year, month);
  unsigned long daysSecPast = secsPastFromFirstDayOfMonthToCurDay(day);
  unsigned long timeSecPast = secsPastFromMidnight(hour, minute, second);
  ts = yearSecPast + monthSecPast + daysSecPast + timeSecPast;
  return ts;
}

static unsigned long DateTimeTool::dateTimeToUTCTimestamp(DateTime * datetime){
  return dateTimeToLocalTimestamp(datetime) - (datetime->getTimeShift() * 60);
}

static DateTime DateTimeTool::timestampToDateTime(unsigned long tsorigin){

  unsigned long timestamp = tsorigin;
  unsigned char second;
  unsigned char minute;
  unsigned char hour;
  unsigned char day;
  unsigned char month;
  unsigned int year;
  time_t timet = tsorigin - SEC_1970_2000;
  struct tm * mytm = gmtime(&timet);
  second = mytm->tm_sec;
  minute = mytm->tm_min;
  hour = mytm->tm_hour;
  day = mytm->tm_mday;
  month = mytm->tm_mon + 1;
  year = mytm->tm_year + 1900;
  return DateTime(year, month, day, hour, minute, second);
}

static unsigned char DateTimeTool::DSTTimeDayBegin(unsigned int y){
    unsigned int weekDay = DateTimeTool::dayOfWeek(y, DST_MONTH_BEGIN, 31);
    return 31 - weekDay;
}

static unsigned char DateTimeTool::DSTTimeDayEnd(unsigned int y){
    unsigned int weekDay = DateTimeTool::dayOfWeek(y, DST_MONTH_END, 31);
    return 31 - weekDay;
}

static DateTime DateTimeTool::DSTBeginDatetime(unsigned int year){
    unsigned char dstBeginDay = DSTTimeDayBegin(year);
    return DateTime(year, DST_MONTH_BEGIN, dstBeginDay, DST_HOUR_BEGIN, DST_MIN_BEGIN, 0, 0);
}

static DateTime DateTimeTool::DSTEndDatetime(unsigned int year){
    unsigned char dstEndDay = DSTTimeDayEnd(year);
    return DateTime(year, DST_MONTH_END, dstEndDay, DST_HOUR_END, DST_MIN_END, 0, DST_OFFSET_MIN);
}
