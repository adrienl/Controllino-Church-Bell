#include <SPI.h>
#include <Controllino.h>
#include <time.h>
#include "Display.hpp"
#include "RTCManager.hpp"
#include "ClockHandler.hpp"

#define BT1     CONTROLLINO_A0
#define BT2     CONTROLLINO_A1
#define RING_BT CONTROLLINO_IN0

#define SYNC_RTC_EVERY_XMIN 1//Update every 6 hours
#define DEFAULT_TIMEZONE 1

Display display = Display::build2X16();
RTCManager rtcManager = RTCManager();
TimeZone tz = TimeZone::buildEuropeParisTimezone();
ClockHandler clockhandler = ClockHandler(tz);

void initInterrupts(void){
  PCICR |= (1 << PCIE1);
  PCMSK1 |= (1 << PCINT8 | 1 << PCINT9);
  sei();
}

void initInputs(){
  pinMode(INPUT, BT1);
  pinMode(INPUT, BT2);
  pinMode(INPUT, RING_BT);
}

unsigned long lastmls = 0;

void displayDate(DateTime * dateTimeObj){
  char strDate[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  dateTimeObj->fillShortDateStringBuffer(strDate, 9);
  display.printStringAt(0, 0, strDate);
}

void displayTime(DateTime * dateTimeObj){
  char strTime[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  dateTimeObj->fillTimeStringBuffer(strTime, 9);
  display.printStringAt(0, 1, strTime);
}

void displayTimeZone(){
  char dstName[5] = {0, 0, 0, 0, 0};
  if (clockhandler.isDST()){
    tz.getRegionalShortDSTName(dstName, 5);
  }else{
    tz.getRegionalShortName(dstName, 5);
  }
  Serial.println(dstName);
  display.clearAt(9, 1, 4);
  display.printStringAt(9, 1, dstName);
}

void displayTimeDate(DateTime * dateTimeObj){
  displayDate(dateTimeObj);
  displayTime(dateTimeObj);
  displayTimeZone();
}

void tick(unsigned long tmstp){
  DateTime dateTime = clockhandler.getCurrentDateTime();
  displayTimeDate(&dateTime);
}

void rtcUpdateRequest(){
  Serial.println("RTC Update Requested");
  unsigned long ts = rtcManager.getTimestamp();
  clockhandler.setTimestamp(ts);
  clockhandler.updateDSTState();
}

void setup() {
  Serial.begin(115200);
  display.init();
  rtcManager.init();
  rtcUpdateRequest();
  clockhandler.onTick(tick);//Called Every Seconds
  clockhandler.setRTCUpdateRequestFrequency(SYNC_RTC_EVERY_XMIN);
  clockhandler.onRTCUpdateRequest(rtcUpdateRequest);
  initInputs();
  initInterrupts();
  DateTime dateTime = clockhandler.getCurrentDateTime();
  displayTimeDate(&dateTime);
}

void loop() {
  clockhandler.loop(millis());
}
