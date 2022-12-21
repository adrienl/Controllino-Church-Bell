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

#define DST_MONTH_BEGIN 3
#define DST_HOUR_BEGIN  2
#define DST_MIN_BEGIN   0   
#define DST_MONTH_END   10
#define DST_HOUR_END    3
#define DST_MIN_END     0

const unsigned char DAYS_SDMONTH[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const unsigned char DAYS_LPMONTH[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

typedef enum E_EventType {
  EET_Simple,
  EET_Triple,
  EET_Five,
  EET_Angelus
} E_EventType;

#endif
