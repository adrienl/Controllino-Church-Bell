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
#include "MenuList.hpp"
#include "MenuItemDate.hpp"
#include "MenuItemTime.hpp"
#include "DateTimeTool.hpp"

//TODO
// - Remove Timezone Class (Keep Summer time only)
// - When time is changed, check the most appropriate event
// - Build Time / Date Editor

RTCManager rtcManager = RTCManager();
RelayManager relayManager = RelayManager();
ButtonsManager buttonsManager = ButtonsManager();
ClockHandler clockHandler = ClockHandler();
Display display = Display::build2X16();
MainApp mainApp = MainApp(&display, &clockHandler);
MenuItemDate _menuItemDate;
MenuItemTime _menuItemTime;
MenuList menu = MenuList(&display);
bool showMenu = false;

/* Others calls */

void updateMCUClockFromRTC(){
  unsigned long ts = rtcManager.getTimestamp();
  clockHandler.setTimestamp(ts);
  DateTime dt = clockHandler.getCurrentDateTime();
  mainApp.updateDisplay();
}

void startBell(eEventType event){
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

void pushedPlus(){
  if (display.getView() != DV_HOME){
    menu.btPlus();
  }
}

void pushedMinus(){
  if (display.getView() != DV_HOME){
    menu.btMinus();
  }
}

void pushedEnter(){
  if (display.getView() == DV_HOME){
    mainApp.enableDisplayUpdate(false);
    menu.init();
  }else{
    menu.btEnter();
  }
}

void pushedExit(){
  if (display.getView() == DV_MENULIST){
    showMenu = false;
    display.setView(DV_HOME);
    mainApp.enableDisplayUpdate(true);
    mainApp.updateDisplay();
  }
  if (display.getView() == DV_MENUITEM){
    menu.btExit();
  }
}

void onButtonPushed(unsigned int button){
  if(ButtonsManager::BT_ONE_PULSE == button){
    startBell(EET_One);
  }else if(ButtonsManager::BT_ANGELUS == button){
    startBell(EET_Angelus);
  }else if(ButtonsManager::BT_PLUS == button){
    pushedPlus();
  }else if(ButtonsManager::BT_MINUS == button){
    pushedMinus();
  }else if(ButtonsManager::BT_ENTER == button){
    pushedEnter();
  }else if(ButtonsManager::BT_EXIT == button){
    pushedExit();
  }
}

/* MENU Callabck*/

void menuRequestValues(MenuItem * item){
  if (item->getType() == eMenu_Date){
    MenuItemDate * mi = (MenuItemDate *) item;
    DateTime dateTime = clockHandler.getCurrentDateTime();
    mi->setDay((int)dateTime.getDay());
    mi->setMonth((int)dateTime.getMonth());
    mi->setYear((int)dateTime.getYear());
  } else if (item->getType() == eMenu_Time){
    MenuItemTime * mi = (MenuItemTime *) item;
    DateTime dateTime = clockHandler.getCurrentDateTime();
    mi->setMinute((int)dateTime.getMinute());
    mi->setHour((int)dateTime.getHour());
  }
}

void menuLeaveValueEdition(MenuItem * menuItem){
  eMenuItemType menuType = menuItem->getType();
  if (menuType == eMenu_Date || menuType == eMenu_Time){
    DateTime dt = clockHandler.getCurrentDateTime();
    unsigned char hour = dt.getHour();
    unsigned char minute = dt.getMinute();
    unsigned char second = dt.getSecond();
    unsigned int year = dt.getYear();
    unsigned char month = dt.getMonth();
    unsigned char day = dt.getDay();
    if (menuType == eMenu_Date){
      MenuItemDate * mi = (MenuItemDate *)menuItem;
      year = mi->getYear();
      month = mi->getMonth();
      day = mi->getDay();
    }else if (menuType == eMenu_Time){
      MenuItemTime * mi = (MenuItemTime *)menuItem;
      hour = mi->getHour();
      minute = mi->getMinute();
      second = 0;
    }
    DateTime newDT = DateTime(year, month, day, hour, minute, second, dt.getTimeShift());
    unsigned long newTS = newDT.getUTCTimestamp();
    rtcManager.setFromTimestamp(newTS);
    updateMCUClockFromRTC();
    mainApp.init();
  }
}

bool menuRequestValueCheck(MenuItem * menuItem){
  eMenuItemType menuType = menuItem->getType();
  if (menuType == eMenu_Date){
      MenuItemDate * menuItemDate = (MenuItemDate *)menuItem;
      int year = menuItemDate->getYear();
      int month = menuItemDate->getMonth();
      int day = menuItemDate->getDay();
      if (month > 12 || month < 1){
        return false;
      }
      bool isLeapYear = DateTimeTool::getIsLeapYear(year);
      unsigned char maxMonthDay = 0;
      if (isLeapYear){
        maxMonthDay = DAYS_LPMONTH[month - 1];
      }else{
        maxMonthDay = DAYS_SDMONTH[month - 1];
      }
      if (day > maxMonthDay){
        return false;
      }
      return true;
  }else if (menuType == eMenu_Time){
    return true;
  }
  return false;
}

/* --- Relay Action CallBack --- */

void relayActionStarted(RelayAction * action){}
void relayActionChanged(RelayAction * action){}

void relayActionEnded(RelayAction * action){
  RelayAction::deleteAllNodes(action);
}

/* -------------------------- */

void setupMenu(){
  _menuItemDate.setDisplay(&display);
  _menuItemDate.onLeaveValueEdition(&menuLeaveValueEdition);
  _menuItemDate.onRequestValues(&menuRequestValues);
  _menuItemDate.onRequestValueCheck(&menuRequestValueCheck);
  
  _menuItemTime.setDisplay(&display);
  _menuItemTime.onRequestValues(&menuRequestValues);
  _menuItemTime.onLeaveValueEdition(&menuLeaveValueEdition);
  
  menu.addMenuItem(&_menuItemDate);
  menu.addMenuItem(&_menuItemTime);
}

void setupRelay(){
  relayManager.setOnActionStarted(&relayActionStarted);
  relayManager.setOnActionChanged(&relayActionChanged);
  relayManager.setOnActionEnded(&relayActionEnded);
  pinMode(CONTROLLINO_D0, OUTPUT);
}

void setupClockHandler(){
  clockHandler.onEverySeconds(&everySecondTicked);
  clockHandler.onEveryMinutes(&everyMinutes);
  clockHandler.onEveryHours(&everyHours);
  clockHandler.setRTCUpdateRequestFrequency(SYNC_RTC_EVERY_XMIN);
  clockHandler.onRTCUpdateRequest(&updateMCUClockFromRTC);
}

void setupButtons(){
  buttonsManager.initButtons();
  buttonsManager.setOnButtonPushed(&onButtonPushed);
}

void setupMainApp(){
  updateMCUClockFromRTC();
  mainApp.setTriggerEvent(&startBell);
  mainApp.init();
  mainApp.updateDisplay();
}

void setup() {
  setupMenu();
  display.init();
  rtcManager.init();
  setupClockHandler();
  setupRelay();
  setupButtons();
  setupMainApp();
}

void everySecondTicked(unsigned long tickValue){
  _menuItemDate.tick();
}

void loop() {
  clockHandler.loop();
  relayManager.loop();
  buttonsManager.loop();
}
