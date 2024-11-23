#include <SPI.h>
#include <Controllino.h>
#include <time.h>
#include "Const.h"
#include "RTCManager.hpp"
#include "ClockHandler.hpp"
#include "Event.hpp"
#include "RelayManager.hpp"
#include "ButtonsManager.hpp"
#include "MainApp.hpp"
#include "Display.hpp"

//TODO
// - Remove Timezone
// - When time is changed, check the most appropriate event
// - Build Time / Date Editor

#define SYNC_RTC_EVERY_XMIN 1440//Update every 6 hours

RTCManager rtcManager = RTCManager();
TimeZone tz = TimeZone::buildEuropeParisTimezone();
RelayManager relayManager = RelayManager();
ButtonsManager buttonsManager = ButtonsManager();
ClockHandler clockHandler = ClockHandler(tz);
Display display = Display::build2X16();
MainApp mainApp = MainApp(&display, &clockHandler);
bool setTimeMode = false;

/* Others calls */

void updateMCUClockFromRTC(){
  unsigned long ts = rtcManager.getTimestamp();
  clockHandler.setTimestamp(ts);
  mainApp.updateDisplay();
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

void everyHours(unsigned long tmstp){
  mainApp.handleHourChange();
}

void everyMinutes(unsigned long tmstp){
  mainApp.handleMinuteChange();
}

void onButtonPushed(unsigned int button){
  if(ButtonsManager::BT_ONE_PULSE == button){
    startBell(EET_One);
  }else if(ButtonsManager::BT_ANGELUS == button){
    startBell(EET_Angelus);
  }else if(ButtonsManager::BT_PLUS == button){
    rtcManager.addOneMinute();
    updateMCUClockFromRTC();
  }else if(ButtonsManager::BT_MINUS == button){
    rtcManager.subtractOneMinute();
    updateMCUClockFromRTC();
  }else if(ButtonsManager::BT_SET == button){
    setTimeMode = !setTimeMode;
    mainApp.updateDisplay();
  }
}

/* --- Relay Action CallBack --- */

void relayActionStarted(RelayAction * action){}
void relayActionChanged(RelayAction * action){}

void relayActionEnded(RelayAction * action){
  RelayAction::deleteAllNodes(action);
}

/* -------------------------- */

void setup() {
  display.init();
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
  mainApp.init();
  mainApp.updateDisplay();
  buttonsManager.setOnButtonPushed(&onButtonPushed);
  mainApp.setTriggerEvent(&startBell);
}

void loop() {
  clockHandler.loop();
  relayManager.loop();
  buttonsManager.loop();
}
