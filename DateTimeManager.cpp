#include "DateTimeManager.hpp"
#include <time.h>

DateTimeManager::DateTimeManager() {
  _timestamp = 0;
  _lastmls = 0;
  _currentTimezone = 0;
  _updateRTCRequestFreqMin = 60;//Default every 60 minutes
}

void DateTimeManager::setTimestamp(unsigned long timestamp){
  _timestamp = timestamp;
}

DateTime DateTimeManager::getCurrentDateTime(){
  return DateTime(_timestamp, _currentTimezone);
}

void DateTimeManager::loop(unsigned long mls){
  if (mls != _lastmls){
    _lastmls = mls;
    
    if (mls % 1000 == 0){//Called every Seconds
      _timestamp++;
      _tickFunc(_timestamp);
    }

    if (mls % (1000 * 60 * _updateRTCRequestFreqMin) == 0){//Called every _updateRTCRequestFreqMin
      _rtcUpdateRequestFunc();
    }
    
  }
}

void DateTimeManager::onTick(void (*tickFunc)(unsigned long)){
  _tickFunc = tickFunc;
}

void DateTimeManager::onRTCUpdateRequest(void (*onRTCUpdateRequest)()){
  _rtcUpdateRequestFunc = onRTCUpdateRequest;
}

void DateTimeManager::setTimezone(unsigned char tz){
  _currentTimezone = tz;
}

void DateTimeManager::setRTCUpdateRequestFrequency(unsigned int freqMin){
  if (freqMin < 1){
    freqMin = 1;
  }
  _updateRTCRequestFreqMin = freqMin;
}
