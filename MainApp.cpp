#include "MainApp.hpp"

MainApp::MainApp(Display * display, ClockHandler * clockHandler) : _display(display), _clockHandler(clockHandler), _triggerEvent(NULL){}

void MainApp::init(){
    setNextBellEventScheduled(0);
}

void MainApp::replaceWithNewBellEvent(Event * e){
  if (_nextBellEvent != NULL){
    delete _nextBellEvent;
  }
  _nextBellEvent = e;
}

void MainApp::setNextBellEventScheduled(unsigned char timeShiftSec){
  replaceWithNewBellEvent(NULL);
  int nbItems = sizeof(BELL_SCHEDULES)/sizeof(Schedule);
  if (nbItems < 1){ return;}
  int i = 0;
  DateTime currentDT = _clockHandler->getCurrentDateTime();
  DateTime nextDT = DateTime(currentDT.getUTCTimestamp() + timeShiftSec, currentDT.getTimeShift());
  bool firstScheduleDefined = false;
  unsigned long ts = 0;
  Schedule * eventSchedule = NULL;
  while (i < nbItems){
    unsigned long nTS = BELL_SCHEDULES[i].getScheduleTimestamp(nextDT);
    if (firstScheduleDefined == false){
      eventSchedule = &BELL_SCHEDULES[i];
      firstScheduleDefined = true;
      ts = nTS;
    }else if (nTS < ts){
      eventSchedule = &BELL_SCHEDULES[i];
      ts = nTS;
    }
    i++;
  }
  if (eventSchedule != NULL){
    DateTime eventDT = eventSchedule->getScheduleDatetime(nextDT);
    replaceWithNewBellEvent(new Event(eventSchedule->getEventType(), eventDT));
  }
}

void MainApp::displayTime(DateTime * dateTimeObj){
  uint8_t arrayLength = 6;
  char strTime[arrayLength] = {0};
  dateTimeObj->fillShortTimeStringBuffer(strTime, arrayLength);
  _display->printStringAt(11, 0, strTime);
}

void MainApp::displayDate(DateTime * dateTimeObj){
  uint8_t arrayLength = 11;
  char strDate[arrayLength] = {0};
  dateTimeObj->fillShortDateStringBuffer(strDate, arrayLength);
  _display->printStringAt(0, 0, strDate);
}

void MainApp::displayBellEvent(Event * bellEvent){
  if (bellEvent != NULL){
      char title[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
      E_EventType eventType = bellEvent->getEventType();
      if (eventType == EET_One){strcpy(title, "Simple\0");}
      if (eventType == EET_Three){strcpy(title, "Triple\0");}
      if (eventType == EET_Five){strcpy(title, "Cinq\0");}
      if (eventType == EET_Angelus){strcpy(title, "Angelus\0");}
      unsigned int len = strlen(title);
      _display->clearAt(0, 1, 16);
      _display->printStringAt(0, 1, title);
      DateTime dt = bellEvent->getDateTime();
      unsigned char hour = dt.getHour();
      unsigned char minute = dt.getMinute();
      char dateBuff[6] = {0, 0, 0, 0, 0, 0};
      snprintf(dateBuff, 6, "%02d:%02d\0", hour, minute);
      _display->printStringAt(11, 1, dateBuff);
  }
}

void MainApp::triggerEvent(E_EventType event){
    if (_triggerEvent != NULL){
        _triggerEvent(event);
    }
    setNextBellEventScheduled(1);
    displayBellEvent(_nextBellEvent);
}

void MainApp::checkBellEvent(){
  if (_nextBellEvent == NULL) { return; }
  DateTime dateTime = _clockHandler->getCurrentDateTime();
  if (_nextBellEvent->getUTCTimestamp() == dateTime.getUTCTimestamp()){
    triggerEvent(_nextBellEvent->getEventType());
  }
}

void MainApp::handleMinuteChange(){
    DateTime dateTime = _clockHandler->getCurrentDateTime();
    displayTime(&dateTime);
    checkBellEvent();
}

void MainApp::handleHourChange(){
    DateTime dateTime = _clockHandler->getCurrentDateTime();
    displayDate(&dateTime);
}

void MainApp::updateDisplay(){
  DateTime dateTime = _clockHandler->getCurrentDateTime();
  displayDate(&dateTime);
  displayTime(&dateTime);
  displayBellEvent(_nextBellEvent);
}

void MainApp::setTriggerEvent(void (*triggerEvent)(E_EventType)){
    _triggerEvent = triggerEvent;
}