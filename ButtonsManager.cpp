#include "ButtonsManager.hpp"

ButtonsManager::ButtonsManager() : _onButtonReleased(NULL), _onButtonPushed(NULL){

}

void ButtonsManager::loop(){
  for (unsigned char i = 0; i < NB_INPUT; i++){
    bool bt = digitalRead(_bts[i]);
    if (bt > 0 && _bts_pushed[i] == false){
      this->_bts_pushed[i] = true;
      if (_onButtonPushed != NULL){
        _onButtonPushed(_bts[i]);
      }
    }else if (bt < 1 && _bts_pushed[i] == true) {
      _bts_pushed[i] = false;
      if (_onButtonReleased != NULL){
        _onButtonReleased(_bts[i]);
      }
    }
  }
}

void ButtonsManager::setOnButtonPushed(void (*onButtonPushed)(unsigned int bt)){
  if (onButtonPushed == NULL){
    return;
  }
  _onButtonPushed = onButtonPushed;
}

void ButtonsManager::setOnButtonReleased(void (*onButtonReleased)(unsigned int bt)){
  if (onButtonReleased == NULL){
    return;
  }
  _onButtonReleased = onButtonReleased;
}

void ButtonsManager::initButtons(){
  pinMode(BT_ONE_PULSE, INPUT);
  pinMode(BT_ANGELUS, INPUT);
  pinMode(BT_PLUS, INPUT);
  pinMode(BT_MINUS, INPUT);  
  pinMode(BT_SET, INPUT);
  pinMode(BT_SEL, INPUT);
}
