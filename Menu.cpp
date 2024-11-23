#include "Menu.hpp"

Menu::Menu(Display * display):_display(display),_valueProvider(NULL),_valueChanged(NULL),_beginEdition(NULL),_endEdition(NULL),_submenuIndex(0),_menuIndex(0),_displayUpdateGranted(false),_enteredMenuItem(false),_nbMenuItems(0){
    _nbMenuItems = sizeof(MENUS) / sizeof(MenuItem);
}

void Menu::onValueChanged(void (*valueChanged)(eMenuSubItemType, int)){
    _valueChanged = valueChanged;
}

void Menu::onRequestValue(int (*valueProvider)(eMenuSubItemType)){
    _valueProvider = valueProvider;
}

void Menu::onBeginEdition(void (*beginEdition)(eMenuItemType)){
    _beginEdition = beginEdition;
}

void Menu::onEndEdition(void (*endEdition)(eMenuItemType)){
    _endEdition = endEdition;
}

void Menu::updateDisplay(){
    if (_displayUpdateGranted){
        char indexStr[2] = {0, 0};
        char nbItemsStr[2] = {0, 0};
        MenuItem * item = &(MENUS[_menuIndex]);
        char * title = item->getTitle();
        _display->clearAll();
        _display->printStringAt(0, 0, "Edit");
        _display->printStringAt(5, 0, title);
        snprintf(indexStr, 2, "%d\0", _menuIndex + 1);
        _display->printStringAt(0, 1, indexStr);
        _display->printStringAt(1, 1, "/\0");
        snprintf(nbItemsStr, 2, "%d\0", _nbMenuItems);
        _display->printStringAt(2, 1, nbItemsStr);
    }
}

void Menu::diplayFirstMenu(){
    _submenuIndex = 0;
    _menuIndex = 0;
    updateDisplay();
}

void Menu::end(){

}

void Menu::increase(){
    if (_enteredMenuItem == false){
        _menuIndex += 1;
        if (_menuIndex >= _nbMenuItems){
            _menuIndex = 0;
        }
    }
    updateDisplay();
}

void Menu::decrease(){
    if (_enteredMenuItem == false){
        if (_menuIndex == 0){
            _menuIndex = _nbMenuItems - 1;
        }else{
            _menuIndex -= 1;
        }
    }
    updateDisplay();
}

void Menu::enableDisplayUpdate(bool enable){
    _displayUpdateGranted = enable;
}