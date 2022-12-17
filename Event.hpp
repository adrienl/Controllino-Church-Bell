#pragma once

#include <Arduino.h>
#include "DateTime.hpp"

class Event {

private:
    DateTime _datetime;
    String _title;
public:
    Event(String title, DateTime dt);
    DateTime getDateTime();
    String getTitle();
    unsigned long getUTCTimestamp();
};
