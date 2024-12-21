#include "MenuList.hpp"


MenuList::MenuList(Display * display):_display(display){
    
    _editingIndex = 0;
    _isEditing = false;
    _nbMenuItems = 0;
    _displayUpdateGranted = false;
    _submenuIndex = 0;
    _menuIndex = 0;
    _endEdition = NULL;
    _beginEdition = NULL;
    _menuItems = NULL;
    _menuItemsLen = 0;
    _currentItem = NULL;
}

MenuList::~MenuList(){
    if (_menuItems != NULL){
        free(_menuItems);
        _menuItems = NULL;
    }
}

void MenuList::updateDisplay(){
    if (_menuIndex >= 0 && _menuIndex < _menuItemsLen){
        MenuItem * menuItem = _menuItems[_menuIndex];
        char indexStr[2] = {0, 0};
        char nbItemsStr[2] = {0, 0};
        char * title = menuItem->getTitle();
        _display->clearAll();
        _display->printStringAt(0, 0, "Edit");
        _display->printStringAt(5, 0, title);
        snprintf(indexStr, 2, "%d\0", _menuIndex + 1);
        _display->printStringAt(0, 1, indexStr);
        _display->printStringAt(1, 1, "/\0");
        snprintf(nbItemsStr, 2, "%d\0", _menuItemsLen);
        _display->printStringAt(2, 1, nbItemsStr);
    }
}

void MenuList::initMenuItem(){
    if (_menuIndex >= 0 && _menuIndex < _menuItemsLen){
        MenuItem * menuItem = _menuItems[_menuIndex];
        menuItem->init();
        _currentItem = menuItem;
    }
}

void MenuList::init(){
    _display->setView(DV_MENULIST);
    _submenuIndex = 0;
    _menuIndex = 0;
    updateDisplay();
}

void MenuList::btPlus(){
    if (_display->getView() == DV_MENULIST){
        if (_menuIndex < _menuItemsLen - 1){
            _menuIndex += 1;
            updateDisplay();
        }
    } else if (_display->getView() == DV_MENUITEM && _currentItem != NULL){
        _currentItem->btPlus();
    }
}

void MenuList::btMinus(){
    if (_display->getView() == DV_MENULIST){
        if (_menuIndex > 0){
            _menuIndex -= 1;
            updateDisplay();
        }
    } else if (_display->getView() == DV_MENUITEM && _currentItem != NULL){
        _currentItem->btMinus();
    }
}

void MenuList::btEnter(){
    if (_display->getView() == DV_MENULIST){
        this->initMenuItem();
        _display->setView(DV_MENUITEM);
    } else if (_display->getView() == DV_MENUITEM && _currentItem != NULL){
        _currentItem->btEnter();
    }
}

void MenuList::btExit(){
    if (_display->getView() == DV_MENUITEM){
        if (_currentItem != NULL && _currentItem->isReadyToExit() == true){
            _currentItem->leave();
            _display->setView(DV_MENULIST);
            updateDisplay();
            _currentItem = NULL;
        }
        if (_currentItem != NULL && _currentItem->isReadyToExit() == false){
            _currentItem->btExit();
        }
    }
}

void MenuList::addMenuItem(MenuItem * menuItem){
    MenuItem ** tmpMenuItems = NULL;
    if (_menuItemsLen >= 1 && _menuItems != NULL){
        tmpMenuItems = (MenuItem **)malloc((_menuItemsLen) * sizeof(MenuItem *));
        for (unsigned int i = 0; i < _menuItemsLen; i++){
            tmpMenuItems[i] = _menuItems[i];
        }
        free(_menuItems);
        _menuItems = NULL;
    }
    _menuItems = (MenuItem **)malloc((_menuItemsLen + 1) * sizeof(MenuItem *));
    if (tmpMenuItems != NULL){
        for (unsigned int i = 0; i < _menuItemsLen; i++){
            _menuItems[i] = tmpMenuItems[i];
        }
    }
    _menuItems[_menuItemsLen] = menuItem;
    _menuItemsLen += 1;
}