#ifndef __DATETIMECONST
#define __DATETIMECONST

#define CURRENT_CENTURY 21

#define DAYS_IN_SDYEAR 365
#define DAYS_IN_LPYEAR 366

#define SEC_IN_MIN 60

#define MIN_IN_HOUR 60
#define SEC_IN_HOUR 3600

#define SEC_IN_DAY 86400
#define HOURS_IN_DAY 24

#define TIMESTAMP_ZERO_YEAR 1970
#define SEC_IN_STDR_YEAR 31536000
#define SEC_IN_LEAP_YEAR 31622400

const unsigned char DAYS_SDMONTH[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const unsigned char DAYS_LPMONTH[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

#endif
