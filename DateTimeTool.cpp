#include "DateTimeTool.hpp"
#include <stdio.h>
#include <math.h>
#include "DateTimeConst.h"

static bool DateTimeTool::isLeapYear(unsigned int year){ //Est-ce une année bissextile ?
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

static unsigned int DateTimeTool::leapYearsPastSince1970(unsigned int yearToCompare){
  unsigned int yearsPast = yearToCompare - TIMESTAMP_ZERO_YEAR;
  unsigned int leapYearsLength = 0;
  unsigned int i = 0;
  while (i <  yearsPast){
    if (isLeapYear(TIMESTAMP_ZERO_YEAR + i) == true){
      leapYearsLength++;
    }
    i++;
  }
  return leapYearsLength;
}

static void DateTimeTool::getDayAndMonthFromPastdays(unsigned int pastdays, unsigned char * month, unsigned char * day){
  unsigned char i = 0;
  pastdays += 1;
  while (i < 12 && pastdays > DAYS_MONTH[i]){
    pastdays -= DAYS_MONTH[i];
    i++;
  }
  *month = i + 1;
  *day = pastdays;
}

static void DateTimeTool::timestampToDateTime(unsigned long tsorigin, ALDateTime * datetime){
  unsigned long timestamp = tsorigin;
  datetime->second = timestamp % SEC_IN_MIN;
  timestamp -= datetime->second;
  timestamp /= SEC_IN_MIN;
  datetime->minute = timestamp % SEC_IN_MIN;
  timestamp -= datetime->minute;
  timestamp /= SEC_IN_MIN;
  datetime->hour = timestamp % HOURS_IN_DAY;
  timestamp -= datetime->hour;
  timestamp /= HOURS_IN_DAY;
  datetime->year = TIMESTAMP_ZERO_YEAR + ((timestamp / (3 * DAYS_IN_SDYEAR + DAYS_IN_LPYEAR)) * 4);
  unsigned int yearPast = datetime->year - TIMESTAMP_ZERO_YEAR;
  unsigned int quartYear = yearPast / 4;
  unsigned int quartYearMod = yearPast % 4;
  unsigned long daysPast = (quartYear * (DAYS_IN_SDYEAR * 3 + DAYS_IN_LPYEAR)) + (quartYearMod * DAYS_IN_SDYEAR);
  unsigned long daysPastInYear = timestamp - daysPast;
  DateTimeTool::getDayAndMonthFromPastdays(daysPastInYear, &datetime->month, &datetime->day);
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

static unsigned int DateTimeTool::summerTimeDayChange(unsigned int y){
    unsigned int weekDay = DateTimeTool::dayOfWeek(y, 3, 31);
    return 31 - weekDay;
}

static unsigned int DateTimeTool::winterTimeDayChange(unsigned int y){
    unsigned int weekDay = DateTimeTool::dayOfWeek(y, 10, 31);
    return 31 - weekDay;
}
