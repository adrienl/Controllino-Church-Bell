#pragma once

#include "TimeZone.hpp"

class DateTime {

private:
    unsigned long _timestamp;
    TimeZone _timezone;
    unsigned char _yearShort;
    unsigned int  _year;
    unsigned char _month;
    unsigned char _day;
    unsigned char _hour;
    unsigned char _minute;
    unsigned char _second;
    void buildDatetime();
    void buildTimestamp();
public:
    DateTime(unsigned long ts);
    DateTime(unsigned long ts, TimeZone timezone);
    DateTime(unsigned int yr, unsigned char mo, unsigned char da, unsigned char hr, unsigned char mn, unsigned char sc);
    DateTime(unsigned int yr, unsigned char mo, unsigned char da, unsigned char hr, unsigned char mn, unsigned char sc, TimeZone timezone);
    unsigned long getTimestamp();
    TimeZone getTimezone();
    unsigned char getYearShort();
    unsigned int getYear();
    unsigned char getMonth();
    unsigned char getDay();
    unsigned char getHour();
    unsigned char getMinute();
    unsigned char getSecond();
    
    void fillTimeStringBuffer(char * buff, unsigned char len); //Prepare a buffer 9 bytes;
    void fillShortTimeStringBuffer(char * buff, unsigned char len); //Prepare a buffer 6 bytes;
    void fillDateStringBuffer(char * buff, unsigned char len); //Prepare a buffer 9 bytes;
    void fillShortDateStringBuffer(char * buff, unsigned char len); //Prepare a buffer 11 bytes;
};
