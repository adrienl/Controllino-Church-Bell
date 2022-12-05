#ifndef __DATETIMETOOL_
#define __DATETIMETOOL_

#include <stdbool.h>

typedef struct ALDateTime{
  unsigned char second;
  unsigned char minute;
  unsigned char hour;
  unsigned int year;
  unsigned char month;
  unsigned char day;
} ALDateTime;

class DateTimeTool {
    
public:
    static void timestampToDateTime(unsigned long tsorigin, ALDateTime * datetime);
    static void getDayAndMonthFromPastdays(unsigned int pastdays, unsigned char * month, unsigned char * day);
    static bool isLeapYear(unsigned int year);
    static unsigned int leapYearsPastSince1970(unsigned int yearToCompare);
    static unsigned int summerTimeDayChange(unsigned int y);
    static unsigned int winterTimeDayChange(unsigned int y);
    static unsigned int dayOfWeek(unsigned int y, unsigned int m, unsigned int d);
};


#endif
