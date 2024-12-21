#include "MenuItemDate.hpp"
#include <stdio.h>
#include <string.h>

MenuItemDate::MenuItemDate():_year(0),_month(0),_day(0){
    _type = eMenu_Date;
    _nbParts = 3;
    strncpy(_title, "Date\0\0\0\0", 8);
}

void MenuItemDate::updateDisplay(){
    char str[16] = {0};
    eMenuValueType params[] = {MP_DAY, MP_MONTH, MP_YEAR};
    snprintf(str, 16, "%02d/%02d/%04d", _day, _month, _year);
    parentUpdateDisplay(params, 3, str);
}

int * MenuItemDate::getValueRef(eMenuValueType type){
    if (type == MP_DAY){
        return &_day;
    }
    if (type == MP_MONTH){
        return &_month;
    }
    if (type == MP_YEAR){
        return &_year;
    }
    return NULL;
}

int MenuItemDate::getYear(){
    return _year;
}

int MenuItemDate::getMonth(){
    return _month;
}

int MenuItemDate::getDay(){
    return _day;
}

void MenuItemDate::setYear(int year){
    _year = year;
}

void MenuItemDate::setMonth(int month){
    _month = month;
}

void MenuItemDate::setDay(int day){
    _day = day;
}