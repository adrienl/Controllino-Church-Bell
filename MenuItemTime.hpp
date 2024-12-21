#ifndef __MENUITEMTIME
#define __MENUITEMTIME

#include "MenuItem.hpp"

class MenuItemTime : public MenuItem{
    private:
        int _minute;
        int _hour;

    public:
        MenuItemTime();

        void updateDisplay();
        int * getValueRef(eMenuValueType);

        void setHour(int hour);
        void setMinute(int minute);
        int getHour();
        int getMinute();
};

#endif