#ifndef __TIMER
#define __TIMER

#include "DateTime.hpp"

class ClockHandler {

private:
    unsigned long _timestamp;
    void (*_everySecondsFunc) (unsigned long ts);
    void (*_everyMinutesFunc) (unsigned long ts);
    void (*_everyHoursFunc) (unsigned long ts);
    void (*_rtcUpdateRequestFunc) ();
    //void (*_countdownFunc) ();
    
    unsigned long _lastmls;
    TimeZone _currentTimezone;
    unsigned int _updateRTCRequestFreqMin;
    bool _isDST;
    unsigned long _DSTTimestamps[2];
    //unsigned long _countDownSec;
    //bool          _isCountingDown;
    
    void internalUpdateDSTOnYear(unsigned int year);
    void internalEachHour();
    void internalTick();
    
public:
    ClockHandler();
    ClockHandler(TimeZone timezone);
    void setTimestamp(unsigned long ts);
    void updateDSTState();
    void loop();
    void setRTCUpdateRequestFrequency(unsigned int freqMin);
    void onEverySeconds(void (*tickCall)(unsigned long ts));
    void onEveryMinutes(void (*tickCall)(unsigned long ts));
    void onEveryHours(void (*tickCall)(unsigned long ts));
    void onRTCUpdateRequest(void (*rtcUpdateRequest)());
    //void onCountDownTriggered(void (*rtcUpdateRequest)());
    //void startCountdown(unsigned long countDownSec);
    DateTime getCurrentDateTime();
    bool isDST();
    //void setTimezone(unsigned char tz, unsigned char fromLastSummerOfMonth, unsigned char toLastSummerOfMonth);
};

#endif
