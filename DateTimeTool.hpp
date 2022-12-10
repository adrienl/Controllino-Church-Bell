#ifndef __DATETIMETOOL_
#define __DATETIMETOOL_

#include <stdbool.h>
#include "DateTimeConst.h"
#include "DateTime.hpp"

class DateTimeTool {
private:
    static unsigned long secsPastFrom1970ToCurYear(unsigned int currentYear);
    static unsigned long secsPastFromJanToLastMonth(unsigned int currentYear, unsigned char currentMonth);
    static unsigned long secsPastFromFirstDayOfMonthToCurDay(unsigned char currentDay);
    static unsigned long secsPastFromMidnight(unsigned char h, unsigned char m, unsigned char s);
    static unsigned int leapYearsPastSince1970(unsigned int yearToCompare);
    static bool getIsLeapYear(unsigned int year);
    static void getDayAndMonthFromPastdays(unsigned int currentYear, unsigned int pastdays, unsigned char * month, unsigned char * day);
    static unsigned int summerTimeDayChange(unsigned int y);
    static unsigned int winterTimeDayChange(unsigned int y);
    static unsigned int dayOfWeek(unsigned int y, unsigned int m, unsigned int d);
public:
    static DateTime timestampToDateTime(unsigned long tsorigin);
    static unsigned long dateTimeToTimestamp(DateTime * datetime);
};


#endif
