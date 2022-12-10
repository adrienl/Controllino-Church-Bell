#include <SPI.h>
#include <Controllino.h>
#include <time.h>
#include "Display.hpp"
#include "RTCManager.hpp"
#include "DateTimeManager.hpp"

#define BT1     CONTROLLINO_A0
#define BT2     CONTROLLINO_A1
#define RING_BT CONTROLLINO_IN0

#define SYNC_RTC_EVERY_MIN 60
#define DEFAULT_TIMEZONE 1

Display display = Display::build2X16();
RTCManager rtcManager = RTCManager();
DateTimeManager dateTimeManager = DateTimeManager();

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

void displayTimeDate(DateTime * dateTimeObj){
  displayDate(dateTimeObj);
  displayTime(dateTimeObj);
}

void tick(unsigned long tmstp){
  DateTime dateTime = dateTimeManager.getCurrentDateTime();
  displayTimeDate(&dateTime);
}

void rtcUpdateRequest(){
  Serial.println("RTC Update Requested");
  unsigned long ts = rtcManager.getTimestamp();
  dateTimeManager.setTimestamp(ts);
}

void setup() {
  Serial.begin(115200);
  display.init();
  rtcManager.init();
  rtcUpdateRequest();
  dateTimeManager.setTimezone(DEFAULT_TIMEZONE);
  dateTimeManager.onTick(tick);//Called Every Seconds
  dateTimeManager.setRTCUpdateRequestFrequency(SYNC_RTC_EVERY_MIN);
  dateTimeManager.onRTCUpdateRequest(rtcUpdateRequest);
  initInputs();
  initInterrupts();
  DateTime dateTime = dateTimeManager.getCurrentDateTime();
  displayTimeDate(&dateTime);
}

void loop() {
  dateTimeManager.loop(millis());
}
