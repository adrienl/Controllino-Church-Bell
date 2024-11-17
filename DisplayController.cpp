#include "DisplayController.hpp"

DisplayController::DisplayController(){

}

void DisplayController::displayDate(DateTime * dateTimeObj){
  uint8_t arrayLength = 11;
  char strDate[arrayLength] = {0};
  dateTimeObj->fillShortDateStringBuffer(strDate, arrayLength);
  _display.printStringAt(0, 0, strDate);
}

void DisplayController::displayTime(DateTime * dateTimeObj){
  uint8_t arrayLength = 6;
  char strTime[arrayLength] = {0};
  dateTimeObj->fillShortTimeStringBuffer(strTime, arrayLength);
  _display.printStringAt(11, 0, strTime);
}

void DisplayController::displayBellEvent(Event * bellEvent){
  if (bellEvent != NULL){
      char title[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
      E_EventType eventType = bellEvent->getEventType();
      if (eventType == EET_One){strcpy(title, "Simple\0");}
      if (eventType == EET_Three){strcpy(title, "Triple\0");}
      if (eventType == EET_Five){strcpy(title, "Cinq\0");}
      if (eventType == EET_Angelus){strcpy(title, "Angelus\0");}
      unsigned int len = strlen(title);
      _display.clearAt(0, 1, 16);
      _display.printStringAt(0, 1, title);
      DateTime dt = bellEvent->getDateTime();
      unsigned char hour = dt.getHour();
      unsigned char minute = dt.getMinute();
      char dateBuff[6] = {0, 0, 0, 0, 0, 0};
      snprintf(dateBuff, 6, "%02d:%02d:00\0", hour, minute);
      _display.printStringAt(11, 1, dateBuff);
  }
}

void DisplayController::init(){
    _display.init();  
}
