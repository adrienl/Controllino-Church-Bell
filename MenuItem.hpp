#ifndef __MENUITEM
#define __MENUITEM

#include "Const.h"

class MenuItem{
    private:
        char _title[8];
        char _separator;
        eMenuItemType _menuType;
        unsigned char _parts;
    public:
        MenuItem(char * title, eMenuItemType menuType, unsigned char parts);
        char * getTitle();
};

#endif