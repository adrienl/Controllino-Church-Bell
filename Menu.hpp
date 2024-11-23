#ifndef __MENU
#define __MENU

#include "Const.h"
#include "Display.hpp"
#include "MenuItem.hpp"

class Menu{
    private:
        Display * _display;
        int (*_valueProvider)(eMenuSubItemType);
        void (*_valueChanged)(eMenuSubItemType, int);
        void (*_beginEdition)(eMenuItemType);
        void (*_endEdition)(eMenuItemType);
        unsigned char _menuIndex;
        unsigned char _submenuIndex;
        void updateDisplay();
        bool _displayUpdateGranted;
        bool _enteredMenuItem;
        unsigned char _nbMenuItems;
    public:
        Menu(Display * display);
        void onRequestValue(int (*valueProvider)(eMenuSubItemType));
        void onValueChanged(void (*valueChanged)(eMenuSubItemType, int));
        void onBeginEdition(void (*beginEdition)(eMenuItemType));
        void onEndEdition(void (*endEdition)(eMenuItemType));
        void diplayFirstMenu();
        void end();
        void increase();
        void decrease();
        void enableDisplayUpdate(bool enable);
};

static MenuItem MENUS[] = {
    MenuItem("Date", eMenu_Date, eMenu_Date_Day|eMenu_Date_Month|eMenu_Date_Year),
    MenuItem("Time", eMenu_Date, eMenu_Time_Hour|eMenu_Time_Minute)
};

#endif