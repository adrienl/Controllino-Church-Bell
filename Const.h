#ifndef __CONST
#define __CONST

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

#define SYNC_RTC_EVERY_XMIN 1440//Update every 6 hours

const unsigned char DAYS_SDMONTH[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const unsigned char DAYS_LPMONTH[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

typedef enum E_EventType {
  EET_One,
  EET_Three,
  EET_Five,
  EET_Angelus
} E_EventType;

<<<<<<< Updated upstream
=======
typedef enum eSetTime{
  eSetMinute = 1,
  eSetHour = 2,
  eSetDay = 3,
  eSetMonth = 4,
  eSetYear = 5
} tSetTime;

typedef enum tMenuItemType{
    eMenu_Date,
    eMenu_Time
}eMenuItemType;

typedef enum tMenuSubItemType{
    eMenu_Date_Day = 1,
    eMenu_Date_Month = 2,
    eMenu_Date_Year = 4,
    eMenu_Time_Minute = 8,
    eMenu_Time_Hour = 16
}eMenuSubItemType;

>>>>>>> Stashed changes
#endif
