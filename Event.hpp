#pragma once

#include "DateTime.hpp"
#include "Const.h"

class Event {

private:
    DateTime    _datetime;
    eEventType _eventType;
    bool        _isInternal;
public:
    Event(eEventType eventType, DateTime dt);
    DateTime getDateTime();
    eEventType getEventType();
    unsigned long getUTCTimestamp();
};
