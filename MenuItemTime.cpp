#include "MenuItemTime.hpp"
#include <stdio.h>
#include <string.h>

MenuItemTime::MenuItemTime():_hour(0),_minute(0){
    _type = eMenu_Time;
    _nbParts = 2;
    strncpy(_title, "Time\0\0\0\0", 8);
}

void MenuItemTime::updateDisplay(){
    char str[16] = {0};
    eMenuValueType params[] = {MP_HOUR, MP_MINUTE};
    snprintf(str, 16, "%02d:%02d", _hour, _minute);
    parentUpdateDisplay(params, 2, str);
}

int * MenuItemTime::getValueRef(eMenuValueType type){
    if (type == MP_MINUTE){
        return &_minute;
    }
    if (type == MP_HOUR){
        return &_hour;
    }
    return NULL;
}

void MenuItemTime::setHour(int hour){
    _hour = hour;
}

void MenuItemTime::setMinute(int minute){
    _minute = minute;
}

int MenuItemTime::getHour(){
    return _hour;
}

int MenuItemTime::getMinute(){
    return _minute;
}