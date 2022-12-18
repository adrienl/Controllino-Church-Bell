#pragma once

#include <Arduino.h>
#include "DateTime.hpp"
#include "Const.h"

class Event {

private:
    DateTime    _datetime;
    E_EventType _eventType;
    bool        _isInternal;
public:
    Event(E_EventType eventType, DateTime dt);
    DateTime getDateTime();
    E_EventType getEventType();
    unsigned long getUTCTimestamp();
};
