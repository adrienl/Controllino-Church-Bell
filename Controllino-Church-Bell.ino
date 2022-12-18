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

//1648256390 2023 Summer time
//1667005185 2023 Classik time

#define BT_SEC_PLUS   CONTROLLINO_A0
#define BT_SEC_MINUS  CONTROLLINO_A1
#define BT_ONE_PULSE  CONTROLLINO_A2
static unsigned char bts[3] = {BT_SEC_PLUS, BT_SEC_MINUS, BT_ONE_PULSE};
static bool bts_pushed[3] = {false, false, false};
#define SYNC_RTC_EVERY_XMIN 1440//Update every 6 hours

Display display = Display::build2X16();
RTCManager rtcManager = RTCManager();
TimeZone tz = TimeZone::buildEuropeParisTimezone();
ClockHandler clockHandler = ClockHandler(tz);
RelayManager relayManager = RelayManager();

Event     * _nextBellEvent = NULL;

//Church clock events  
Schedule bellSchedules[] = {
  Schedule(EET_Simple, 6, 30, ScheduleWeekDay(0, 1, 0, 1, 1, 1, 0)),
  Schedule(EET_Simple, 7, 0, ScheduleWeekDay(1, 0, 1, 0, 0, 0, 1)),
  Schedule(EET_Angelus, 7, 10, ScheduleWeekDay(0, 1, 0, 1, 1, 1, 0)),
  Schedule(EET_Angelus, 7, 40, ScheduleWeekDay(0, 0, 1, 0, 0, 0, 1)),
  Schedule(EET_Triple, 8, 25, ScheduleWeekDay(1, 0, 0, 0, 0, 0, 0)),
  Schedule(EET_Simple, 8, 30, ScheduleWeekDay(1, 0, 0, 0, 0, 0, 0)),
  Schedule(EET_Triple, 11, 25, ScheduleWeekDay(0, 0, 1, 0, 0, 0, 1)),
  Schedule(EET_Simple, 11, 30, ScheduleWeekDay(0, 0, 1, 0, 0, 0, 1)),
  Schedule(EET_Triple, 12, 10),
  Schedule(EET_Simple, 12, 15),
  Schedule(EET_Angelus, 12, 25),
  Schedule(EET_Triple, 17, 40, ScheduleWeekDay(1, 0, 0, 0, 1, 0, 0)),
  Schedule(EET_Simple, 17, 45, ScheduleWeekDay(1, 0, 0, 0, 1, 0, 0)),
  Schedule(EET_Triple, 17, 55, ScheduleWeekDay(0, 1, 1, 1, 0, 1, 1)),
  Schedule(EET_Simple, 18, 0, ScheduleWeekDay(0, 1, 1, 1, 0, 1, 1)),
  Schedule(EET_Simple, 18, 30),
  Schedule(EET_Triple, 20, 40),
  Schedule(EET_Simple, 20, 45),
};

unsigned long lastmls = 0;

/* Others calls */

void initInputs(){
  pinMode(BT_ONE_PULSE, INPUT);
  pinMode(BT_SEC_PLUS, INPUT);
  pinMode(BT_SEC_MINUS, INPUT);
  pinMode(CONTROLLINO_D0, OUTPUT);
}

void updateMCUClockFromRTC(){
  unsigned long ts = rtcManager.getTimestamp();
  clockHandler.setTimestamp(ts);
}

void startBell(E_EventType event){
  RelayAction * act = NULL;
  if (event == EET_Angelus){act = RelayAction::buildAngelusActions();}
  else if (event == EET_Triple){act = RelayAction::buildTripleAction();}
  else if (event == EET_Simple){act = RelayAction::buildSimpleAction();}
  if (act != NULL && relayManager.startFromAction(act) == false){
    RelayAction::deleteAllNodes(act);
  }
}

void triggerEvent(E_EventType event){
  Serial.println("Dring !");
  startBell(event);
  setNextBellEventScheduled(1);
  displayNextBellEvent();
}

/* ----------------------- */

/* Display Updates */

void displayDate(DateTime * dateTimeObj){
  char strDate[6] = {0, 0, 0, 0, 0, 0};
  dateTimeObj->fillUltraShortDateStringBuffer(strDate, 6);
  display.printStringAt(0, 0, strDate);
}

void displayTime(DateTime * dateTimeObj){
  char strTime[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  dateTimeObj->fillTimeStringBuffer(strTime, 9);
  display.printStringAt(6, 0, strTime);
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
    if (eventType == EET_Simple){strcpy(title, "Simple\0");}
    if (eventType == EET_Triple){strcpy(title, "Triple\0");}
    if (eventType == EET_Angelus){strcpy(title, "Angelus\0");}
    unsigned int len = strlen(title);
    display.clearAt(0, 1, 16);
    display.printStringAt(0, 1, title);
    DateTime dt = _nextBellEvent->getDateTime();
    unsigned char hour = dt.getHour();
    unsigned char minute = dt.getMinute();
    char dateBuff[6] = {0, 0, 0, 0, 0, 0};
    snprintf(dateBuff, 6, "%02d:%02d\0", hour, minute);
    display.printStringAt(len + 1, 1, dateBuff);
  }
}

void updateFullDisplay(){
  DateTime dateTime = clockHandler.getCurrentDateTime();
  displayDate(&dateTime);
  displayTime(&dateTime);
  displayNextBellEvent();
  //displayTimeZone();
}

/* ------------------- */

/* ------- Called Every Hour */

void everyHours(unsigned long tmstp){
  DateTime dateTime = clockHandler.getCurrentDateTime();
  displayDate(&dateTime);
  //displayTimeZone();
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
  checkBellEvent();
}

/* ----------------------------- */

/* ------- Called Every Seconds */

void everySeconds(unsigned long tmstp){
  DateTime dateTime = clockHandler.getCurrentDateTime();
  displayTime(&dateTime);
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
  if (BT_SEC_PLUS == button){//Add one second button
    unsigned long ts = rtcManager.getTimestamp();
    rtcManager.setFromTimestamp(ts + 1);
    updateMCUClockFromRTC();
  }else if (BT_SEC_MINUS == button){//Add one second button
    unsigned long ts = rtcManager.getTimestamp();
    rtcManager.setFromTimestamp(ts - 1);
    updateMCUClockFromRTC();
  }else if(BT_ONE_PULSE){
    startBell(EET_Simple);
  }
}

void onReleased(unsigned int button){
  
}

void checkButtonsCalls(){
  for (unsigned char i = 0; i < 3; i++){
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
  Serial.println("relayActionStarted");
}

void relayActionChanged(RelayAction * action){
  Serial.println("relayActionChanged");
}

void relayActionEnded(RelayAction * action){
  Serial.println("relayActionEnded");
  RelayAction::deleteAllNodes(action);
}

/* -------------------------- */

void setup() {
  Serial.begin(115200);
  display.init();
  rtcManager.init();
  //rtcManager.setFromTimestamp(1671143122);
  updateMCUClockFromRTC();
  clockHandler.onEverySeconds(everySeconds);
  clockHandler.onEveryMinutes(everyMinutes);
  clockHandler.onEveryHours(everyHours);
  clockHandler.setRTCUpdateRequestFrequency(SYNC_RTC_EVERY_XMIN);
  clockHandler.onRTCUpdateRequest(updateMCUClockFromRTC);
  relayManager.setOnActionStarted(&relayActionStarted);
  relayManager.setOnActionChanged(&relayActionChanged);
  relayManager.setOnActionEnded(&relayActionEnded);
  initInputs();
  setNextBellEventScheduled(0);
  updateFullDisplay();
}

void loop() {
  clockHandler.loop();
  relayManager.loop();
  checkButtonsCalls();
}
