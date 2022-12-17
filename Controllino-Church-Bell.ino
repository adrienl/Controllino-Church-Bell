#include <SPI.h>
#include <Controllino.h>
#include <time.h>
#include "Display.hpp"
#include "RTCManager.hpp"
#include "ClockHandler.hpp"
#include "BellManager.hpp"
#include "Schedule.hpp"
#include "ScheduleWeekDay.hpp"
#include "Event.hpp"

//1648256390 2023 Summer time
//1667005185 2023 Classik time

#define BT1         CONTROLLINO_A2
static bool         BT1_PUSHED = false;
#define BT2         CONTROLLINO_A3
static bool         BT2_PUSHED = false;
#define SYNC_RTC_EVERY_XMIN 1440//Update every 6 hours

Display display = Display::build2X16();
RTCManager rtcManager = RTCManager();
TimeZone tz = TimeZone::buildEuropeParisTimezone();
ClockHandler clockHandler = ClockHandler(tz);
BellManager bellManager = BellManager(CONTROLLINO_D0);

Event     * _nextEvent = NULL;

//Church clock events  
Schedule schedules[] = {
  Schedule("Simple", 6, 30, ScheduleWeekDay(0, 1, 0, 1, 1, 1, 0)),
  Schedule("Simple", 7, 0, ScheduleWeekDay(1, 0, 1, 0, 0, 0, 1)),
  Schedule("Angelus", 7, 10, ScheduleWeekDay(0, 1, 0, 1, 1, 1, 0)),
  Schedule("Angelus", 7, 40, ScheduleWeekDay(0, 0, 1, 0, 0, 0, 1)),
  Schedule("Triple", 8, 25, ScheduleWeekDay(1, 0, 0, 0, 0, 0, 0)),
  Schedule("Simple", 8, 30, ScheduleWeekDay(1, 0, 0, 0, 0, 0, 0)),
  Schedule("Triple", 11, 25, ScheduleWeekDay(0, 0, 1, 0, 0, 0, 1)),
  Schedule("Simple", 11, 30, ScheduleWeekDay(0, 0, 1, 0, 0, 0, 1)),
  Schedule("Triple", 12, 10),
  Schedule("Simple", 12, 15),
  Schedule("Angelus", 12, 25),
  Schedule("Triple", 17, 40, ScheduleWeekDay(1, 0, 0, 0, 1, 0, 0)),
  Schedule("Simple", 17, 45, ScheduleWeekDay(1, 0, 0, 0, 1, 0, 0)),
  Schedule("Triple", 17, 55, ScheduleWeekDay(0, 1, 1, 1, 0, 1, 1)),
  Schedule("Simple", 18, 0, ScheduleWeekDay(0, 1, 1, 1, 0, 1, 1)),
  Schedule("Simple", 18, 30),
  Schedule("Triple", 20, 40),
  Schedule("Simple", 20, 45),
};

unsigned long lastmls = 0;

/* Others calls */

void initInputs(){
  pinMode(BT1, INPUT);
  pinMode(BT2, INPUT);
}

void updateMCUClockFromRTC(){
  unsigned long ts = rtcManager.getTimestamp();
  clockHandler.setTimestamp(ts);
}

void triggerEvent(){
  Serial.println("Dring !");
  bellManager.ring();
  setNextEventScheduled(1);
  displayNextEvent();
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

void displayNextEvent(){
  if (_nextEvent != NULL){
    String title = _nextEvent->getTitle();
    unsigned int strlen = title.length();
    char buff[strlen + 1];
    title.toCharArray(buff, strlen + 1);
    display.clearAt(0, 1, 16);
    display.printStringAt(0, 1, buff);
    DateTime dt = _nextEvent->getDateTime();
    unsigned char hour = dt.getHour();
    unsigned char minute = dt.getMinute();
    char dateBuff[6] = {0, 0, 0, 0, 0, 0};
    snprintf(dateBuff, 6, "%02d:%02d\0", hour, minute);
    display.printStringAt(strlen + 1, 1, dateBuff);
  }
}

void updateFullDisplay(){
  DateTime dateTime = clockHandler.getCurrentDateTime();
  displayDate(&dateTime);
  displayTime(&dateTime);
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

void checkEvent(){
  if (_nextEvent == NULL) { return; }
  DateTime dateTime = clockHandler.getCurrentDateTime();
  if (_nextEvent->getUTCTimestamp() == dateTime.getUTCTimestamp()){
    triggerEvent();
  }
}

void everyMinutes(unsigned long tmstp){
  checkEvent();
}

/* ----------------------------- */

/* ------- Called Every Seconds */

void everySeconds(unsigned long tmstp){
  DateTime dateTime = clockHandler.getCurrentDateTime();
  displayTime(&dateTime);
}

/* ----------------------------- */

/* -- Event Updates -- */

void replaceWithNewEvent(Event * event){
  if (_nextEvent != NULL){
    delete _nextEvent;
  }
  _nextEvent = event;
}

void setNextEventScheduled(unsigned char timeShiftSec){
  int nbItems = sizeof(schedules)/sizeof(Schedule);
  if (nbItems < 1){
    return;  
  }
  int i = 1;
  DateTime currentDT = clockHandler.getCurrentDateTime();
  DateTime nextDT = DateTime(currentDT.getUTCTimestamp() + timeShiftSec, currentDT.getTimeShift());
  unsigned long ts = schedules[0].getScheduleTimestamp(nextDT);
  Schedule * eventSchedule = &(schedules[0]);
  while (i < nbItems){
    unsigned long nTS = schedules[i].getScheduleTimestamp(nextDT);
    if (nTS < ts){
      eventSchedule = &schedules[i];
      ts = nTS;
    }
    i++;
  }
  DateTime eventDT = eventSchedule->getScheduleDatetime(nextDT);
  replaceWithNewEvent(new Event(eventSchedule->getTitle(), eventDT));
}

/* -------------------- */

/* -- BUTTONS CALLS -- */

void onPushed(unsigned int button){
  if (BT1 == button){//Add one second button
    unsigned long ts = rtcManager.getTimestamp();
    rtcManager.setFromTimestamp(ts + 1);
    updateMCUClockFromRTC();
  }else if (BT2 == button){//Add one second button
    unsigned long ts = rtcManager.getTimestamp();
    rtcManager.setFromTimestamp(ts - 1);
    updateMCUClockFromRTC();
  }
}

void onReleased(unsigned int button){
  
}

void checkButtonsCalls(){
  bool bt1 = digitalRead(BT1);
  if (bt1 > 0 && BT1_PUSHED == false){
    BT1_PUSHED = true;
    onPushed(BT1);
  }else if (bt1 < 1 && BT1_PUSHED == true) {
    BT1_PUSHED = false;
    onReleased(BT1);
  }
  bool bt2 = digitalRead(BT2);
  if (bt2 > 0 && BT2_PUSHED == false){
    BT2_PUSHED = true;
    onPushed(BT2);
  }else if (bt2 < 1 && BT2_PUSHED == true) {
    BT2_PUSHED = false;
    onReleased(BT2);
  }
}

/* -------------------------- */

void setup() {
  Serial.begin(115200);
  display.init();
  rtcManager.init();
  bellManager.init();
  //rtcManager.setFromTimestamp(1671143122);
  updateMCUClockFromRTC();
  clockHandler.onEverySeconds(everySeconds);
  clockHandler.onEveryMinutes(everyMinutes);
  clockHandler.onEveryHours(everyHours);
  clockHandler.setRTCUpdateRequestFrequency(SYNC_RTC_EVERY_XMIN);
  clockHandler.onRTCUpdateRequest(updateMCUClockFromRTC);
  initInputs();
  updateFullDisplay();
  setNextEventScheduled(0);
  displayNextEvent();
}

void loop() {
  clockHandler.loop();
  bellManager.loop();
  checkButtonsCalls();
}
