#pragma once

class DateTime {

private:
    unsigned long _timestamp;
    unsigned char _timezone;
    unsigned char _yearShort;
    unsigned int _year;
    unsigned char _month;
    unsigned char _day;
    unsigned char _hour;
    unsigned char _minute;
    unsigned char _second;
    void build();
public:
    DateTime(unsigned long timestamp, unsigned char timezone);
    unsigned long getTimestamp();
    unsigned char getTimezone();
    unsigned char getYearShort();
    unsigned char getYear();
    unsigned char getMonth();
    unsigned char getDay();
    unsigned char getHour();
    unsigned char getMinute();
    unsigned char getSecond();
    
    void fillTimeStringBuffer(char * buff, unsigned char len); //Prepare a buffer 9 bytes;
    void fillShortTimeStringBuffer(char * buff, unsigned char len); //Prepare a buffer 6 bytes;
    void fillDateStringBuffer(char * buff, unsigned char len); //Prepare a buffer 9 bytes;
};
