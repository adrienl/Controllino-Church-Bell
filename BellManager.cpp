#include "BellManager.hpp"
#include <Arduino.h>
#define KEEP_HAMMER_MS 100

BellManager::BellManager(unsigned char outputPort) : _hammerTimeMaintainedMS(KEEP_HAMMER_MS), _isRinging(false), _isRingingSince(0), _bellOutputPort(outputPort), _lastMLS(0) {

}

void BellManager::init() {
  pinMode(_bellOutputPort, OUTPUT);
}

bool BellManager::ring() {
  if (_isRinging == true){
    return false;  
  }
  _isRinging = true;
  return true;
}

void BellManager::setHammerOn(){
  Serial.println("Hammer On");
   digitalWrite(_bellOutputPort, HIGH);
}

void BellManager::setHammerOff(){
  Serial.println("Hammer Off");
  digitalWrite(_bellOutputPort, LOW);
}

void BellManager::handleRingings(){
  if (_isRinging == false){
    return;
  }
  if (_isRingingSince < KEEP_HAMMER_MS){
    if (_isRingingSince < 1){
      setHammerOn();
    }
    _isRingingSince++;
  }else{
    _isRinging = false;
    _isRingingSince = 0;
    setHammerOff();
  }
}

void BellManager::loop() {
  unsigned long mls = millis();
  if (_lastMLS != mls){
    handleRingings();
    _lastMLS = mls;
  }
}
