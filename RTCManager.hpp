#pragma once

class RTCManager {

private:
public:
    RTCManager();
    void init();
    void setTimeDate(unsigned char aDay, unsigned char aWeekDay,unsigned char aMonth, unsigned char aYear, unsigned char aHour, unsigned char aMinute, unsigned char aSecond);
    void setCompilationTimeDate();
    char getDay();
    char getMonth();
    char getYear();
    char getWeekDay();
    char getHour();
    char getMinute();
    char getSecond();

    void fillTimeStringBuffer(char * buff, unsigned char len); //Prepare a buffer 9 bytes;
    void fillDateStringBuffer(char * buff, unsigned char len); //Prepare a buffer 9 bytes;
};
