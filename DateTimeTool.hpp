#ifndef __DATETIMETOOL_
#define __DATETIMETOOL_

#include <stdbool.h>
#include "DateTimeConst.h"
#include "DateTime.hpp"
#include "Timezone.hpp"

class DateTimeTool {
private:
    static unsigned long secsPastFrom1970ToCurYear(unsigned int currentYear);
    static unsigned long secsPastFromJanToLastMonth(unsigned int currentYear, unsigned char currentMonth);
    static unsigned long secsPastFromFirstDayOfMonthToCurDay(unsigned char currentDay);
    static unsigned long secsPastFromMidnight(unsigned char h, unsigned char m, unsigned char s);
    static unsigned int leapYearsPastSince1970(unsigned int yearToCompare);
    static void getDayAndMonthFromPastdays(unsigned int currentYear, unsigned int pastdays, unsigned char * month, unsigned char * day);
    static bool getIsLeapYear(unsigned int year);
public:
    static unsigned int dayOfWeek(unsigned int y, unsigned int m, unsigned int d);
    static DateTime timestampToDateTime(unsigned long tsorigin);
    static unsigned long dateTimeToUTCTimestamp(DateTime * datetime);
    static unsigned long dateTimeToLocalTimestamp(DateTime * datetime);
    static unsigned char DSTTimeDayBegin(unsigned int y);
    static unsigned char DSTTimeDayEnd(unsigned int y);
    static DateTime DateTimeTool::DSTBeginDatetime(unsigned int year, TimeZone tz);
    static DateTime DateTimeTool::DSTEndDatetime(unsigned int year, TimeZone tz);
};


#endif
