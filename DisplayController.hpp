#ifndef __DISPLAY_CONTROLLER
#define __DISPLAY_CONTROLLER

#include "Event.hpp"
#include "Display.hpp"

class DisplayController {
  private:
    Display _display = Display::build2X16();
  public:
    DisplayController();
    void displayBellEvent(Event * event);
    void init();
    void displayTime(DateTime * dateTimeObj);
    void displayDate(DateTime * dateTimeObj);
};

#endif
