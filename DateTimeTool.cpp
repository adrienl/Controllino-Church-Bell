#include "DateTimeTool.hpp"

#include <stdio.h>
#include <math.h>

typedef struct ALDateTime {
  unsigned int year;
  unsigned char month;
  unsigned char day;
  unsigned char hour;
  unsigned char minute;
  unsigned char second;
} ALDateTime;

void getDayAndMonthFromPastdays(unsigned int pastdays, unsigned char * month, unsigned char * day){
  unsigned char monthsDays [12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  unsigned char i = 0;
  pastdays += 1;
  while (i < 12 && pastdays > monthsDays[i]){
    pastdays -= monthsDays[i];
    i++;
  }
  *month = i + 1;
  *day = pastdays;
}

void timestampToDateTime(unsigned long tsorigin, ALDateTime * datetime){
  unsigned long timestamp = tsorigin;
  datetime->second = timestamp % 60;
  timestamp -= datetime->second;
  timestamp /= 60;
  datetime->minute = timestamp % 60;
  timestamp -= datetime->minute;
  timestamp /= 60;
  datetime->hour = timestamp % 24;
  timestamp -= datetime->hour;
  timestamp /= 24;
  datetime->year =  1970 + ((timestamp / (3 * 365 + 366)) * 4);
  unsigned int yearPast = datetime->year - 1970;
  unsigned int quartYear = yearPast / 4;
  unsigned int quartYearMod = yearPast % 4;
  unsigned long daysPast = (quartYear * (365 * 3 + 366)) + (quartYearMod * 365);
  unsigned long daysPastInYear = timestamp - daysPast;
  getDayAndMonthFromPastdays(daysPastInYear, &datetime->month, &datetime->day);
}

void dateToString(ALDateTime datetime, char * strToFill, char len){
  snprintf(strToFill, len, "%02d/%02d/%04d", datetime.day, datetime.month, datetime.year);
}

void timeToString(ALDateTime datetime, char * strToFill, char len){
  snprintf(strToFill, len, "%02d:%02d:%02d", datetime.hour, datetime.minute, datetime.second);
}

unsigned int dayOfWeek(unsigned int y, unsigned int m, unsigned int d){
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

unsigned int summerTimeDayChange(unsigned int y){
    unsigned int weekDay = dayOfWeek(y, 3, 31);
    return 31 - weekDay;
}

unsigned int winterTimeDayChange(unsigned int y){
    unsigned int weekDay = dayOfWeek(y, 10, 31);
    return 31 - weekDay;
}
