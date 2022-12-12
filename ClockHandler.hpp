#ifndef __TIMER
#define __TIMER

#include "DateTime.hpp"

class ClockHandler {

private:
    unsigned long _timestamp;
    void (*_tickFunc) (unsigned long ts);
    void (*_rtcUpdateRequestFunc) ();
    unsigned long _lastmls;
    TimeZone _currentTimezone;
    unsigned int _updateRTCRequestFreqMin;
    void internalTick();
    bool _isDST;
    unsigned long _DSTTimestamps[2];
    void internalUpdateDSTOnYear(unsigned int year);
    
public:
    ClockHandler();
    ClockHandler(TimeZone timezone);
    void setTimestamp(unsigned long ts);
    void updateDSTState();
    void loop(unsigned long mls);
    void setRTCUpdateRequestFrequency(unsigned int freqMin);
    void onTick(void (*tickCall)(unsigned long ts));
    void onRTCUpdateRequest(void (*rtcUpdateRequest)());
    DateTime getCurrentDateTime();
    bool isDST();
    //void setTimezone(unsigned char tz, unsigned char fromLastSummerOfMonth, unsigned char toLastSummerOfMonth);
};

#endif
