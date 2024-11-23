#include "MenuItem.hpp"

MenuItem::MenuItem(char * title, eMenuItemType menuType, unsigned char parts):_title({0,0,0,0,0,0,0,0}),_menuType(menuType),_parts(parts){
    for (unsigned char i; i < 7; i++){
        char c = title[i];
        if (c <= 0){
            break;
        }
        _title[i] = c;
    }
}

char * MenuItem::getTitle(){
    return _title;
}