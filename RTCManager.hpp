#ifndef __RCTMANAGER
#define __RCTMANAGER

class RTCManager {

private:
    unsigned long secsPastFrom1970ToCurYear();
    unsigned long secsPastFromJanToLastMonth();
    unsigned long secsPastFromFirstDayOfMonthToCurDay();
    unsigned long secsPastFromMidnight();
public:
    RTCManager();
    void  init();
    void  setUTCTimeDate(unsigned char aDay, unsigned char aWeekDay,unsigned char aMonth, unsigned char aYear, unsigned char aHour, unsigned char aMinute, unsigned char aSecond);
    void  setFromTimestamp(unsigned long ts);
    char  getDay();
    char  getMonth();
    char  getShortYear();
    int   getYear();
    char  getWeekDay();
    char  getHour();
    char  getMinute();
    char  getSecond();
    unsigned long getTimestamp();
};

#endif
