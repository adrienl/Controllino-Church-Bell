#ifndef __MENUITEM
#define __MENUITEM

#include "Display.hpp"
#include "Const.h"
#include "Arduino.h"

#define NB_MENU_PARTS 5

const static sMenuComposition __menuComposition[NB_MENU_PARTS] = {
    {0, MP_HOUR, eMenu_Time, {'H','o','u','r',0,0,0,0}, 1, 0, 23},
    {1, MP_MINUTE, eMenu_Time, {'M','i','n','u','t','e',0,0}, 4, 0, 59},
    
    {0, MP_DAY, eMenu_Date, {'D','a','y',0,0,0,0,0}, 1, 1, 31},
    {1, MP_MONTH, eMenu_Date, {'M','o','n','t','h',0,0,0}, 4, 1, 12},
    {2, MP_YEAR, eMenu_Date, {'Y','e','a','r',0,0,0,0}, 9, 2000, 2099}
};

class MenuItem{
    protected:
        MenuItem(){
            _changingValue = false;
            _showInvalidValue = false;
            _pfValueCheck = NULL;
            _tickCount = 0;
        };
        Display *       _display;
        uint8_t         _idx;
        uint8_t         _nbParts;
        bool            _changingValue;
        char            _title[8];
        eMenuItemType   _type;
        bool _showInvalidValue;
        void (* _fcRequestValues)(MenuItem *);
        void (* _fcSendNewValue)(MenuItem *, eMenuValueType, int);
        unsigned char _tickCount;
        bool (*_pfValueCheck)(MenuItem *);
        void (*_pfLeavingValueEdition)(MenuItem *);
        void parentUpdateDisplay(eMenuValueType params[], unsigned char paramLen, char * stringVal){
            uint8_t underlineAt = 0;
            uint8_t margin = 0;
            _display->clearAt(0, 0, 16);
            if (_changingValue == false){
                _display->printStringAt(0, 0, "Select");
                margin = 7;
            }else{
                _display->printStringAt(0, 0, "Set");
                margin = 4;
            }
            for (unsigned char i = 0; i < paramLen; i++){
                eMenuValueType param = params[i];
                if (_idx == this->getIndexOfParam(param)){
                    char * val = (char *)getMenuCompositionForType(param)->name;
                    _display->printStringAt(margin, 0, val);
                    underlineAt = getMenuCompositionForType(param)->cursorPos;
                }
            }
            if (_showInvalidValue == true){
                _display->clearAt(0, 0, 16);
                _display->printStringAt(0, 0, "Invalid Value");
            }
            _display->printStringAt(0, 1, stringVal);
            _display->setCursor(underlineAt, 1);
            _display->toggleCursor(true);
            _display->toggleBlink(_changingValue);
        }

        void parentBtPlus(){
            if (_changingValue == false){
                if (_idx < _nbParts - 1){
                    _idx++;
                }
            }else{
                eMenuValueType valType = this->getParamForIndex(_idx, _type);
                int * valRef = this->getValueRef(valType);
                const sMenuComposition * menuComp = getMenuCompositionForType(valType);
                if ((*valRef) < menuComp->maxVal){
                    (*valRef)++;
                }
            }
            this->updateDisplay();
        }

        bool leaveEdition(){
            if (_pfValueCheck != NULL){
                bool dateIsValid = _pfValueCheck(this);
                if (dateIsValid){
                    _showInvalidValue = false;
                    _changingValue = false;
                    this->updateDisplay();
                    return true;
                }else{
                    _showInvalidValue = true;
                    _tickCount = 0;
                    this->updateDisplay();
                    return false;
                }
            }else{
                _changingValue = false;
                _showInvalidValue = false;
                this->updateDisplay();
                return true;
            }
        }

    public:
        // Virtual
        virtual void updateDisplay(){};

        void init(){
            _fcRequestValues(this);
            _display->clearAll();
            updateDisplay();
        };

        void btPlus(){
            if (_changingValue == false){
                if (_idx < _nbParts - 1){
                    _idx++;
                }
            }else{
                eMenuValueType valType = getParamForIndex(_idx, _type);
                int * valRef = this->getValueRef(valType);
                const sMenuComposition * menuComp = getMenuCompositionForType(valType);
                if ((*valRef) < menuComp->maxVal){
                    (*valRef)++;
                }
            }
            this->updateDisplay();
        };

        void tick(){
            if (_showInvalidValue == true){
                _tickCount++;
                if (_tickCount >= 4){
                    _tickCount = 0;
                    _showInvalidValue = false;
                    this->updateDisplay();
                }
            }
        };

        void btMinus(){
            if (_changingValue == false){
                if (_idx > 0){
                    _idx--;
                }
            }else{
                eMenuValueType valType = getParamForIndex(_idx, _type);
                int * valRef = this->getValueRef(valType);
                const sMenuComposition * menuComp = getMenuCompositionForType(valType);
                if ((*valRef) > menuComp->minVal){
                    (*valRef)--;
                }
            }
            this->updateDisplay();
        };

        void btEnter(){
            if (_changingValue == false){
                _changingValue = true;
                this->updateDisplay();
            }
        };
        virtual int * getValueRef(eMenuValueType){};
        // Implemented
        void setDisplay(Display * display) {_display = display;};
        char * getTitle(){return &(_title[0]);};
        eMenuItemType getType(){return _type;};
        void onRequestValues(void (* fc)(MenuItem *)){_fcRequestValues = fc;};
        void onNewValueSet(void (* fc)(MenuItem *, eMenuValueType type, int value)){_fcSendNewValue = fc;};
        bool isReadyToExit(){return _changingValue == false;};
        void leave(){
            if (_display){
                _display->toggleCursor(false);
                _display->setCursor(0, 0);
                _idx = 0;
                _changingValue = false;
            }
        };
        uint8_t getPartsLength(){return _nbParts;};
        char getIndexOfParam(eMenuValueType type){
            for (unsigned char i = 0; i < NB_MENU_PARTS; i++){
                sMenuComposition comp = __menuComposition[i];
                if (comp.partType == type){
                    return comp.idx;
                }
            }
            return -1;
        }
        eMenuValueType getParamForIndex(unsigned char idx, eMenuItemType menuType){
            for (unsigned char i = 0; i < NB_MENU_PARTS; i++){
                sMenuComposition comp = __menuComposition[i];
                if (comp.menuType == menuType && comp.idx == idx){
                    return comp.partType;
                }
            }
            return MP_UNDEFINED;
        }

        const sMenuComposition * getMenuCompositionForType(eMenuValueType type){
            for (unsigned char i = 0; i < NB_MENU_PARTS; i++){
                sMenuComposition comp = __menuComposition[i];
                if (comp.partType == type){
                    return &(__menuComposition[i]);
                }
            }
            return NULL;
        }
        void onRequestValueCheck(bool (*pf)(MenuItem *)){
            _pfValueCheck = pf;
        }

        void onLeaveValueEdition(void (*pf)(MenuItem *)){
            _pfLeavingValueEdition = pf;
        }

        void btExit(){
            if (_changingValue == true){
                if (leaveEdition() == true){
                    if (_fcSendNewValue != NULL){
                        eMenuValueType valType = getParamForIndex(_idx, _type);
                        int * valRef = this->getValueRef(valType);
                        _fcSendNewValue(this, valType, *valRef);
                    }
                    if (_pfLeavingValueEdition != NULL){
                        _pfLeavingValueEdition(this);
                    }
                }
            }
        };
};

#endif