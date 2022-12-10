#ifndef __DATETIMEMANAGER
#define __DATETIMEMANAGER

#include "DateTime.hpp"

class DateTimeManager {

private:
    unsigned long _timestamp;
    void (*_tickFunc) (unsigned long ts);
    void (*_rtcUpdateRequestFunc) ();
    unsigned long _lastmls;
    TimeZone _currentTimezone;
    unsigned int _updateRTCRequestFreqMin;
    
public:
    DateTimeManager();
    DateTimeManager(TimeZone timezone);
    void setTimestamp(unsigned long ts);
    void loop(unsigned long mls);
    void setRTCUpdateRequestFrequency(unsigned int freqMin);
    void onTick(void (*tickCall)(unsigned long ts));
    void onRTCUpdateRequest(void (*rtcUpdateRequest)());
    DateTime getCurrentDateTime();
    //void setTimezone(unsigned char tz, unsigned char fromLastSummerOfMonth, unsigned char toLastSummerOfMonth);
};

#endif
