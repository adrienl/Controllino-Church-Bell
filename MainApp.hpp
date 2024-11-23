#ifndef __MAINAPP
#define __MAINAPP

#include "Const.h"
#include "Display.hpp"
#include "Event.hpp"
#include "Schedule.hpp"
#include "ClockHandler.hpp"

static Schedule BELL_SCHEDULES[] = {
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

class MainApp{
    public:
        MainApp(Display * display, ClockHandler * clockHandler);
        void init();
        void setTriggerEvent(void (*)(E_EventType));
        void handleMinuteChange();
        void handleHourChange();
        void updateDisplay();
        void enableDisplayUpdate(bool enable);
    private:
        Display * _display;
        ClockHandler * _clockHandler;
        void (*_triggerEvent) (E_EventType event);
        Event   * _nextBellEvent = NULL;
        bool _displayUpdateGranted;
        void triggerEvent(E_EventType event);
        void displayBellEvent(Event * bellEvent);
        void checkBellEvent();
        void replaceWithNewBellEvent(Event * e);
        void setNextBellEventScheduled(unsigned char timeShiftSec);
        void displayDate(DateTime * dateTimeObj);
        void displayTime(DateTime * dateTimeObj);
};

#endif