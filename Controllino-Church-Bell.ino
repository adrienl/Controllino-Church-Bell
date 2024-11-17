#include <SPI.h>
#include <Controllino.h>
#include <time.h>
#include "DisplayController.hpp"
#include "RTCManager.hpp"
#include "ClockHandler.hpp"
#include "Schedule.hpp"
#include "ScheduleWeekDay.hpp"
#include "Event.hpp"
#include "RelayManager.hpp"
#include "ButtonsManager.hpp"

//Bell events you can add, remove or modify.
Schedule bellSchedules[] = {
  Schedule(EET_One, 6, 30, ScheduleWeekDay(0, 1, 0, 1, 1, 1, 0)),
  Schedule(EET_One, 7, 0, ScheduleWeekDay(1, 0, 1, 0, 0, 0, 1)), 
  Schedule(EET_Angelus, 7, 10, ScheduleWeekDay(0, 1, 0, 1, 1, 1, 0)),
  Schedule(EET_Angelus, 7, 40, ScheduleWeekDay(0, 0, 1, 0, 0, 0, 1)),
  Schedule(EET_Three, 8, 25, ScheduleWeekDay(1, 0, 0, 0, 0, 0, 0)),
  Schedule(EET_One, 8, 30, ScheduleWeekDay(1, 0, 0, 0, 0, 0, 0)),
  Schedule(EET_Five, 9, 55, ScheduleWeekDay(1, 0, 0, 0, 0, 0, 0)),
  Schedule(EET_Three, 11, 25, ScheduleWeekDay(0, 0, 1, 0, 0, 0, 1)),
  Schedule(EET_One, 11, 30, ScheduleWeekDay(0, 0, 1, 0, 0, 0, 1)), 
  Schedule(EET_Three, 12, 10),                                        
  Schedule(EET_One, 12, 15),
  Schedule(EET_Angelus, 12, 25),
  Schedule(EET_Five, 16, 55, ScheduleWeekDay(0, 0, 0, 0, 1, 0, 0)),
  Schedule(EET_One, 17, 00, ScheduleWeekDay(0, 0, 0, 0, 1, 0, 0)),
  Schedule(EET_One, 17, 40, ScheduleWeekDay(1, 0, 0, 0, 0, 0, 0)),
  Schedule(EET_One, 17, 45, ScheduleWeekDay(1, 0, 0, 0, 1, 0, 0)),
  Schedule(EET_Three, 17, 55, ScheduleWeekDay(0, 1, 1, 1, 0, 1, 1)),
  Schedule(EET_One, 18, 0, ScheduleWeekDay(0, 1, 1, 1, 0, 1, 1)),
  Schedule(EET_Five, 18, 25),
  Schedule(EET_One, 18, 30),
  Schedule(EET_Three, 20, 40),
  Schedule(EET_One, 20, 45),
  // Like the Schedule lines above, add any additional Schedule lines here.
};

#define SYNC_RTC_EVERY_XMIN 1440//Update every 6 hours

RTCManager rtcManager = RTCManager();
TimeZone tz = TimeZone::buildEuropeParisTimezone();
ClockHandler clockHandler = ClockHandler(tz);
RelayManager relayManager = RelayManager();
ButtonsManager buttonsManager = ButtonsManager();
DisplayController displayController = DisplayController();

Event     * _nextBellEvent = NULL;

/* Others calls */

void updateMCUClockFromRTC(){
  unsigned long ts = rtcManager.getTimestamp();
  clockHandler.setTimestamp(ts);
  updateFullDisplay();
}

void startBell(E_EventType event){
  RelayAction * act = NULL;
  if (event == EET_Angelus){act = RelayAction::buildAngelusActions();}
  else if (event == EET_Three){act = RelayAction::buildTripleAction();}
  else if (event == EET_Five){act = RelayAction::buildFiveAction();}
  else if (event == EET_One){act = RelayAction::buildSimpleAction();}
  if (act != NULL && relayManager.startFromAction(act) == false){
    RelayAction::deleteAllNodes(act);
  }
}

void triggerEvent(E_EventType event){
  startBell(event);
  setNextBellEventScheduled(1);
  displayController.displayBellEvent(_nextBellEvent);
}

/* ----------------------- */

/* Display Updates */

void updateFullDisplay(){
  DateTime dateTime = clockHandler.getCurrentDateTime();
  displayController.displayDate(&dateTime);
  displayController.displayTime(&dateTime);
  displayController.displayBellEvent(_nextBellEvent);
}

/* ------------------- */

/* ------- Called Every Hour */

void everyHours(unsigned long tmstp){
  DateTime dateTime = clockHandler.getCurrentDateTime();
  displayController.displayDate(&dateTime);
}

/* ------------------- */

/* ------- Called Every Minutes */

void checkBellEvent(){
  if (_nextBellEvent == NULL) { return; }
  DateTime dateTime = clockHandler.getCurrentDateTime();
  if (_nextBellEvent->getUTCTimestamp() == dateTime.getUTCTimestamp()){
    triggerEvent(_nextBellEvent->getEventType());
  }
}

void everyMinutes(unsigned long tmstp){
  DateTime dateTime = clockHandler.getCurrentDateTime();
  displayController.displayTime(&dateTime);
  checkBellEvent();
}

/* ----------------------------- */

/* -- Event Updates -- */

void replaceWithNewBellEvent(Event * e){
  if (_nextBellEvent != NULL){
    delete _nextBellEvent;
  }
  _nextBellEvent = e;
}

void setNextBellEventScheduled(unsigned char timeShiftSec){
  replaceWithNewBellEvent(NULL);
  int nbItems = sizeof(bellSchedules)/sizeof(Schedule);
  if (nbItems < 1){ return;}
  int i = 0;
  DateTime currentDT = clockHandler.getCurrentDateTime();
  DateTime nextDT = DateTime(currentDT.getUTCTimestamp() + timeShiftSec, currentDT.getTimeShift());
  bool firstScheduleDefined = false;
  unsigned long ts = 0;
  Schedule * eventSchedule = NULL;
  while (i < nbItems){
    unsigned long nTS = bellSchedules[i].getScheduleTimestamp(nextDT);
    if (firstScheduleDefined == false){
      eventSchedule = &bellSchedules[i];
      firstScheduleDefined = true;
      ts = nTS;
    }else if (nTS < ts){
      eventSchedule = &bellSchedules[i];
      ts = nTS;
    }
    i++;
  }
  if (eventSchedule != NULL){
    DateTime eventDT = eventSchedule->getScheduleDatetime(nextDT);
    replaceWithNewBellEvent(new Event(eventSchedule->getEventType(), eventDT));
  }
}

/* -------------------- */

/* -- BUTTONS CALLS -- */

void onButtonPushed(unsigned int button){
  if(ButtonsManager::BT_ONE_PULSE == button){
    startBell(EET_One);
  }else if(ButtonsManager::BT_ANGELUS == button){
    startBell(EET_Angelus);
  }else if(ButtonsManager::BT_MIN_PLUS == button){
    rtcManager.addOneMinute();
    updateMCUClockFromRTC();
  }else if(ButtonsManager::BT_MIN_MINUS == button){
    rtcManager.subtractOneMinute();
    updateMCUClockFromRTC();
  }
}

/* --- Relay Action CallBack --- */

void relayActionStarted(RelayAction * action){
}

void relayActionChanged(RelayAction * action){
}

void relayActionEnded(RelayAction * action){
  RelayAction::deleteAllNodes(action);
}

/* -------------------------- */

void setup() {
  displayController.init();
  rtcManager.init();
  //rtcManager.setFromTimestamp(1731799306);
  updateMCUClockFromRTC();
  clockHandler.onEveryMinutes(&everyMinutes);
  clockHandler.onEveryHours(&everyHours);
  clockHandler.setRTCUpdateRequestFrequency(SYNC_RTC_EVERY_XMIN);
  clockHandler.onRTCUpdateRequest(&updateMCUClockFromRTC);
  relayManager.setOnActionStarted(&relayActionStarted);
  relayManager.setOnActionChanged(&relayActionChanged);
  relayManager.setOnActionEnded(&relayActionEnded);
  buttonsManager.initButtons();
  pinMode(CONTROLLINO_D0, OUTPUT);
  setNextBellEventScheduled(0);
  updateFullDisplay();
  buttonsManager.setOnButtonPushed(&onButtonPushed);
}

void loop() {
  clockHandler.loop();
  relayManager.loop();
  buttonsManager.loop();
  //checkButtonsCalls();
}
