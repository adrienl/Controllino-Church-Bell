#include "ClockHandler.hpp"
#include <time.h>
#include "DateTimeTool.hpp"

ClockHandler::ClockHandler() : _currentTimezone(TimeZone::buildUTC0Timezone()), _isDST(false), _DSTTimestamps({0, 0}) {
  _timestamp = 0;
  _lastmls = 0;
  _updateRTCRequestFreqMin = 60;//Default every 60 minutes
}

ClockHandler::ClockHandler(TimeZone timezone) : _currentTimezone(timezone), _isDST(false), _DSTTimestamps({0, 0}){
  _timestamp = 0;
  _lastmls = 0;
  _updateRTCRequestFreqMin = 60;//Default every 60 minutes
}

void ClockHandler::setTimestamp(unsigned long timestamp){
  _timestamp = timestamp;
}

void ClockHandler::internalUpdateDSTOnYear(unsigned int year){
  DateTime dtDSTBegin = DateTimeTool::DSTBeginDatetime(year, _currentTimezone);
  DateTime dtDSTEnd = DateTimeTool::DSTEndDatetime(year, _currentTimezone);
  _DSTTimestamps[0] = dtDSTBegin.getTimestamp();
  _DSTTimestamps[1] = dtDSTEnd.getTimestamp();
}

void ClockHandler::updateDSTState(){
  DateTime dt = getCurrentDateTime();
  if (_DSTTimestamps[0] == 0 || _DSTTimestamps[1] == 0){
    internalUpdateDSTOnYear(dt.getYear());
  }
  
  if(dt.getTimestamp() < _DSTTimestamps[0]){
    _isDST = false;
    return;
  }
  if (dt.getTimestamp() >= _DSTTimestamps[0] && dt.getTimestamp() < _DSTTimestamps[1]){
    _isDST = true;
    return;
  }
  if (dt.getTimestamp() >= _DSTTimestamps[1]){
    _isDST = false;
    internalUpdateDSTOnYear(dt.getYear() + 1);
  }
}

DateTime ClockHandler::getCurrentDateTime(){ // To update
  if (_isDST){
    return DateTime(_timestamp, _currentTimezone.getDSTOffsetInMinutes());
  }else{
    return DateTime(_timestamp, _currentTimezone.getOffsetInMinutes());
  }
}

bool ClockHandler::isDST(){ // To update
  return _isDST;
}

void ClockHandler::internalTick(){
  if (_timestamp % (_updateRTCRequestFreqMin * 60) == 0){
    _rtcUpdateRequestFunc();
  }
}

void ClockHandler::loop(unsigned long mls){
  if (mls != _lastmls){
    _lastmls = mls;
    if (mls % 1000 == 0){//Called every Seconds
      _timestamp++;
      internalTick();
      _tickFunc(_timestamp);
    }
  }
}

void ClockHandler::onTick(void (*tickFunc)(unsigned long)){
  _tickFunc = tickFunc;
}

void ClockHandler::onRTCUpdateRequest(void (*onRTCUpdateRequest)()){
  _rtcUpdateRequestFunc = onRTCUpdateRequest;
}

void ClockHandler::setRTCUpdateRequestFrequency(unsigned int freqMin){
  if (freqMin < 1){
    freqMin = 1;
  }
  _updateRTCRequestFreqMin = freqMin;
}
