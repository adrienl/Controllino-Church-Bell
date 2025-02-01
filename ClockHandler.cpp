#include <Arduino.h>
#include <time.h>
#include "ClockHandler.hpp"
#include "DateTimeTool.hpp"

ClockHandler::ClockHandler() : _isDST(false), _DSTTimestamps({0, 0}) {
  _timestamp = 0;
  _lastmls = 0;
  _updateRTCRequestFreqMin = 60;//Default every 60 minutes
}

void ClockHandler::setTimestamp(unsigned long timestamp){
  _timestamp = timestamp;
}

void ClockHandler::internalUpdateDSTOnYear(unsigned int year){
  DateTime dtDSTBegin = DateTimeTool::DSTBeginDatetime(year);
  DateTime dtDSTEnd = DateTimeTool::DSTEndDatetime(year);
  _DSTTimestamps[0] = dtDSTBegin.getUTCTimestamp();
  _DSTTimestamps[1] = dtDSTEnd.getUTCTimestamp();
}

void ClockHandler::updateDSTState(){
  DateTime dt = getCurrentDateTime();
  if (_DSTTimestamps[0] == 0 || _DSTTimestamps[1] == 0){
    internalUpdateDSTOnYear(dt.getYear());
  }
  if(dt.getUTCTimestamp() < _DSTTimestamps[0]){
    _isDST = false;
    return;
  }
  if (dt.getUTCTimestamp() >= _DSTTimestamps[0] && dt.getUTCTimestamp() < _DSTTimestamps[1]){
    _isDST = true;
    return;
  }
  if (dt.getUTCTimestamp() >= _DSTTimestamps[1]){
    _isDST = false;
    internalUpdateDSTOnYear(dt.getYear() + 1);
  }
}

DateTime ClockHandler::getCurrentDateTime(){ // To update
  if (_isDST){
    return DateTime(_timestamp, DST_OFFSET_MIN);
  }else{
    return DateTime(_timestamp, 0);
  }
}

bool ClockHandler::isDST(){ // To update
  return _isDST;
}

void ClockHandler::internalEachHour(){
  updateDSTState();
}

void ClockHandler::internalTick(){
  if (_timestamp % (_updateRTCRequestFreqMin * 60) == 0 && _rtcUpdateRequestFunc != NULL){
    (*_rtcUpdateRequestFunc)();
  }
  if (_timestamp % 60 == 0 && _everyMinutesFunc != NULL){
    internalEachHour();
    (*_everyMinutesFunc)(_timestamp);
  }
  if (_timestamp % 3600 == 0 && _everyHoursFunc != NULL){
    internalEachHour();
    (*_everyHoursFunc)(_timestamp);
  }
}

void ClockHandler::loopSecondPast(){
  _timestamp++;
  internalTick();
  if (_everySecondsFunc != NULL){
    (*_everySecondsFunc)(_timestamp);
  }
}

void ClockHandler::loop(){
  unsigned long mls = millis();
  if (mls == _lastmls) { return; }
  _lastmls = mls;
  if (mls % 1000 == 0){//Called every Seconds
    loopSecondPast();
  }
}

void ClockHandler::onEverySeconds(void (*fnc)(unsigned long)){
  _everySecondsFunc = fnc;
}

void ClockHandler::onEveryMinutes(void (*fnc)(unsigned long)){
  _everyMinutesFunc = fnc;
}

void ClockHandler::onEveryHours(void (*fnc)(unsigned long)){
  _everyHoursFunc = fnc;
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
