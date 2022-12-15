#include <SPI.h>
#include <Controllino.h>
#include <time.h>
#include "Display.hpp"
#include "RTCManager.hpp"
#include "ClockHandler.hpp"

#define BT1     CONTROLLINO_A2
static bool     BT1_PUSHED = false;
#define BT2     CONTROLLINO_A3
static bool     BT2_PUSHED = false;
#define SYNC_RTC_EVERY_XMIN 1440//Update every 6 hours
#define DEFAULT_TIMEZONE 1

Display display = Display::build2X16();
RTCManager rtcManager = RTCManager();
TimeZone tz = TimeZone::buildEuropeParisTimezone();
ClockHandler clockhandler = ClockHandler(tz);

void initInputs(){
  pinMode(INPUT, BT1);
  pinMode(INPUT, BT2);
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
  display.clearAt(9, 1, 4);
  display.printStringAt(9, 1, dstName);
}

void displayTimeDate(DateTime * dateTimeObj){
  displayDate(dateTimeObj);
  displayTime(dateTimeObj);
  displayTimeZone();
}

void updateDisplay(){
  DateTime dateTime = clockhandler.getCurrentDateTime();
  displayTimeDate(&dateTime);
}

void tick(unsigned long tmstp){
  updateDisplay();
}

void rtcUpdateRequest(){
  unsigned long ts = rtcManager.getTimestamp();
  clockhandler.setTimestamp(ts);
  clockhandler.updateDSTState();
}

void setup() {
  Serial.begin(115200);
  display.init();
  rtcManager.init();
  //rtcManager.setFromTimestamp(1671066354);
  rtcUpdateRequest();
  clockhandler.onTick(tick);//Called Every Seconds
  clockhandler.setRTCUpdateRequestFrequency(SYNC_RTC_EVERY_XMIN);
  clockhandler.onRTCUpdateRequest(rtcUpdateRequest);
  initInputs();
  updateDisplay();
}

void onPushed(unsigned int button){
  if (BT1 == button){//Add one second button
    unsigned long ts = rtcManager.getTimestamp();
    rtcManager.setFromTimestamp(ts + 1);
    rtcUpdateRequest();
  }else if (BT2 == button){//Add one second button
    unsigned long ts = rtcManager.getTimestamp();
    rtcManager.setFromTimestamp(ts - 1);
    rtcUpdateRequest();
  }
}

void onReleased(unsigned int button){
 
}

void loop() {
  clockhandler.loop(millis());
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
