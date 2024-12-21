#ifndef __MENU
#define __MENU

#include "Const.h"
#include "Display.hpp"
#include "MenuItem.hpp"
#include "MenuItemDate.hpp"
#include "MenuItemTime.hpp"

class MenuList{
    private:
        Display * _display;
        void (*_beginEdition)(eMenuItemType);
        void (*_endEdition)(eMenuItemType);
        unsigned char _menuIndex;
        unsigned char _submenuIndex;
        void updateDisplay();
        bool _displayUpdateGranted;
        unsigned char _nbMenuItems;
        bool _isEditing;
        unsigned char _editingIndex;
        MenuItem ** _menuItems;
        unsigned char _menuItemsLen;
        void initMenuItem();
        MenuItem * _currentItem;
    public:
        MenuList(Display * display);
        ~MenuList();
        void init();
        void btMinus();
        void btPlus();
        void btEnter();
        void btExit();
        void addMenuItem(MenuItem * menuItem);
};


#endif