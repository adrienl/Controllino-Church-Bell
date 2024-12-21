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

#define SEC_1970_2000 946684800


const unsigned char DAYS_SDMONTH[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; //February is 28
const unsigned char DAYS_LPMONTH[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; //February is 29

typedef enum eEventType {
  EET_One,
  EET_Three,
  EET_Five,
  EET_Angelus
}eEventType;

typedef enum eMenuItemType{
  eMenu_Date,
  eMenu_Time
}eMenuItemType;

typedef enum eButton{
  BT_SELECT,
  BT_BACK,
  BT_PLUS,
  BT_MINUS
}eButton;

typedef enum eMenuValueType{
  MP_UNDEFINED,
  MP_MINUTE,
  MP_HOUR,
  MP_DAY,
  MP_MONTH,
  MP_YEAR
}eMenuValueType;

typedef struct sMenuComposition{
  unsigned char idx;
  eMenuValueType partType;
  eMenuItemType menuType;
  char name[8];
  unsigned char cursorPos;
  int minVal;
  int maxVal;
}sMenuComposition;

    // static const unsigned char BT_MINUS = CONTROLLINO_A0;
    // static const unsigned char BT_PLUS = CONTROLLINO_A1;
    // static const unsigned char BT_SEL = CONTROLLINO_A2;
    // static const unsigned char BT_MENU = CONTROLLINO_A3;

typedef enum eSetTime{
  eSetMinute = 1,
  eSetHour = 2,
  eSetDay = 3,
  eSetMonth = 4,
  eSetYear = 5
} tSetTime;

#endif
