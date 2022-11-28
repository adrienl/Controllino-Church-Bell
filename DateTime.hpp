#pragma once

class DateTime {

private:
    unsigned long _timestamp;
public:
    DateTime(unsigned long timestamp);
    unsigned long getTimestamp();
    unsigned char getYearShort();
    unsigned char getYear();
    void setYear(unsigned char month);
    unsigned char getMonth();
    void setMonth(unsigned char month);
    unsigned char getDay();
    void setDay(unsigned char day);
    unsigned char getHour();
    void setHours(unsigned char hours);
    unsigned char getMinute();
    void setMinutes(unsigned char minutes);
    unsigned char getSecond();
    void setSeconds(unsigned char seconds);
};
