#include "RelayManager.hpp"
#include <Arduino.h>

RelayManager::RelayManager() : _firstNodeAction(NULL), _nodeAction(NULL), _relayIsOn(false), _actOnSince(0), _actOffSince(0), _loopLastMls(0), _onActionStarted(NULL), _onActionChanged(NULL), _onActionEnded(NULL) {
  
}

void RelayManager::setRelayOn(unsigned char relayId){
  digitalWrite(relayId, HIGH);
}

void RelayManager::setRelayOff(unsigned char relayId){
  digitalWrite(relayId, LOW);
}

void RelayManager::setOnActionStarted(void (*f)(RelayAction *)){
  _onActionStarted = f;
}

void RelayManager::setOnActionChanged(void (*f)(RelayAction *)){
  _onActionChanged = f;
}

void RelayManager::setOnActionEnded(void (*f)(RelayAction *)){
  _onActionEnded = f;
}

void RelayManager::loop(){
  unsigned long currentTime = millis();
  if (currentTime != _loopLastMls){
    _loopLastMls = currentTime;
    if (_firstNodeAction != NULL){
      if (_actOnSince == 0){
        setRelayOn(_nodeAction->getOutputId());
        _actOnSince = currentTime;
      }else if (_actOffSince == 0 && currentTime >= _actOnSince + _nodeAction->getOnTiming() ){
        _actOffSince = currentTime;
        setRelayOff(_nodeAction->getOutputId());
      }else if(_actOnSince > 0 && _actOffSince > 0 && currentTime >= _actOffSince + _nodeAction->getOffTiming()){
        _nodeAction = _nodeAction->getNextAction();
        _actOnSince = 0;
        _actOffSince = 0;
        if (_nodeAction != NULL && _onActionChanged != NULL){
          (*_onActionChanged)(_nodeAction);
        }
      }
    }
    if (_nodeAction == NULL && _firstNodeAction != NULL){
      if (_onActionEnded != NULL){
        (*_onActionEnded)(_firstNodeAction);  
      }
      _firstNodeAction = NULL;
      _actOnSince = 0;
      _actOffSince = 0;
      
    }
  }
}

bool RelayManager::startFromAction(RelayAction * act){
  if (_firstNodeAction != NULL){ return false; }
  _firstNodeAction = act;
  _nodeAction = act;
  if (_onActionStarted != NULL){
    (*_onActionStarted)(_firstNodeAction);
  }
  return true;
}
