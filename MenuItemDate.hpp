#ifndef __MENUITEMDATE
#define __MENUITEMDATE

#include "MenuItem.hpp"

class MenuItemDate : public MenuItem{
    private:
        int     _year;
        int     _month;
        int     _day;
        
    public:
        MenuItemDate();
        void updateDisplay();
        int * getValueRef(eMenuValueType);

        int getYear();
        int getMonth();
        int getDay();
        void setYear(int year);
        void setMonth(int month);
        void setDay(int day);
};

#endif