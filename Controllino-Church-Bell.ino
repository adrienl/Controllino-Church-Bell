#include <SPI.h>
#include <Controllino.h>
#include <time.h>
#include "Display.hpp"
#include "RTCManager.hpp"
#include "ClockHandler.hpp"
#include "Schedule.hpp"
#include "ScheduleWeekDay.hpp"
#include "Event.hpp"
#include "RelayManager.hpp"

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

#define NB_INPUT 4

#define BT_MIN_PLUS   CONTROLLINO_A1
#define BT_MIN_MINUS  CONTROLLINO_A0
#define BT_ONE_PULSE  CONTROLLINO_IN0
#define BT_ANGELUS  CONTROLLINO_IN1
static unsigned char bts[NB_INPUT] = {BT_ONE_PULSE, BT_ANGELUS, BT_MIN_PLUS, BT_MIN_MINUS};
static bool bts_pushed[NB_INPUT] = {false, false, false, false};
#define SYNC_RTC_EVERY_XMIN 1440//Update every 6 hours

Display display = Display::build2X16();
RTCManager rtcManager = RTCManager();
TimeZone tz = TimeZone::buildEuropeParisTimezone();
ClockHandler clockHandler = ClockHandler(tz);
RelayManager relayManager = RelayManager();

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
  displayNextBellEvent();
}

/* ----------------------- */

/* Display Updates */

void displayDate(DateTime * dateTimeObj){
  uint8_t arrayLength = 8;
  char strDate[arrayLength] = {0, 0, 0, 0, 0, 0, 0, 0};
  dateTimeObj->fillUltraShortDateStringBuffer(strDate, arrayLength);
  display.printStringAt(0, 0, strDate);
}

void displayTime(DateTime * dateTimeObj){
  uint8_t arrayLength = 6;
  char strTime[arrayLength] = {0, 0, 0, 0, 0, 0};
  dateTimeObj->fillShortTimeStringBuffer(strTime, arrayLength);
  display.printStringAt(11, 0, strTime);
}

/*void displayTimeZone(){
  char dstName[5] = {0, 0, 0, 0, 0};
  if (clockHandler.isDST()){
    tz.getRegionalShortDSTName(dstName, 5);
  }else{
    tz.getRegionalShortName(dstName, 5);
  }
  display.clearAt(12, 0, 4);
  display.printStringAt(12, 0, dstName);
}*/

void displayNextBellEvent(){
  if (_nextBellEvent != NULL){
    char title[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    E_EventType eventType = _nextBellEvent->getEventType();
    if (eventType == EET_One){strcpy(title, "Simple\0");}
    if (eventType == EET_Three){strcpy(title, "Triple\0");}
    if (eventType == EET_Five){strcpy(title, "Cinq\0");}
    if (eventType == EET_Angelus){strcpy(title, "Angelus\0");}
    unsigned int len = strlen(title);
    display.clearAt(0, 1, 16);
    display.printStringAt(0, 1, title);
    DateTime dt = _nextBellEvent->getDateTime();
    unsigned char hour = dt.getHour();
    unsigned char minute = dt.getMinute();
    char dateBuff[6] = {0, 0, 0, 0, 0, 0};
    snprintf(dateBuff, 6, "%02d:%02d:00\0", hour, minute);
    display.printStringAt(11, 1, dateBuff);
  }
}

void updateFullDisplay(){
  DateTime dateTime = clockHandler.getCurrentDateTime();
  displayDate(&dateTime);
  displayTime(&dateTime);
  displayNextBellEvent();
}

/* ------------------- */

/* ------- Called Every Hour */

void everyHours(unsigned long tmstp){
  DateTime dateTime = clockHandler.getCurrentDateTime();
  displayDate(&dateTime);
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
  displayTime(&dateTime);
  checkBellEvent();
}

/* ----------------------------- */

/* ------- Called Every Seconds */

void everySeconds(unsigned long tmstp){

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

void onPushed(unsigned int button){
  if(BT_ONE_PULSE == button){
    startBell(EET_One);
  }else if(BT_ANGELUS == button){
    startBell(EET_Angelus);
  }else if(BT_MIN_PLUS == button){
    rtcManager.addOneMinute();
    updateMCUClockFromRTC();
  }else if(BT_MIN_MINUS == button){
    rtcManager.subtractOneMinute();
    updateMCUClockFromRTC();
  }
}

void onReleased(unsigned int button){
  
}

void checkButtonsCalls(){
  for (unsigned char i = 0; i < NB_INPUT; i++){
    bool bt = digitalRead(bts[i]);  
    if (bt > 0 && bts_pushed[i] == false){
      bts_pushed[i] = true;
      onPushed(bts[i]);
    }else if (bt < 1 && bts_pushed[i] == true) {
      bts_pushed[i] = false;
      onReleased(bts[i]);
    }
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

void initIOs(){
  pinMode(BT_ONE_PULSE, INPUT);
  pinMode(BT_ANGELUS, INPUT);
  pinMode(BT_MIN_PLUS, INPUT);
  pinMode(BT_MIN_MINUS, INPUT);
  //pinMode(BT_SEC_PLUS, INPUT);
  //pinMode(BT_SEC_MINUS, INPUT);
  pinMode(CONTROLLINO_D0, OUTPUT);
}

void setup() {
  display.init();
  rtcManager.init();
  //rtcManager.setFromTimestamp(1731799306);
  updateMCUClockFromRTC();
  clockHandler.onEverySeconds(&everySeconds);
  clockHandler.onEveryMinutes(&everyMinutes);
  clockHandler.onEveryHours(&everyHours);
  clockHandler.setRTCUpdateRequestFrequency(SYNC_RTC_EVERY_XMIN);
  clockHandler.onRTCUpdateRequest(&updateMCUClockFromRTC);
  relayManager.setOnActionStarted(&relayActionStarted);
  relayManager.setOnActionChanged(&relayActionChanged);
  relayManager.setOnActionEnded(&relayActionEnded);
  initIOs();
  setNextBellEventScheduled(0);
  updateFullDisplay();
}

void loop() {
  clockHandler.loop();
  relayManager.loop();
  checkButtonsCalls();
}
