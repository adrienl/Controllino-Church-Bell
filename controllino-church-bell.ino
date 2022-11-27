#include <SPI.h>
#include <Controllino.h>
#include <time.h>
#include "Display.hpp"
#include "RTCManager.hpp"

#define BT1     CONTROLLINO_A0
#define BT2     CONTROLLINO_A1
#define RING_BT CONTROLLINO_IN0

Display display = Display::build2X16();
RTCManager rtcManager = RTCManager();

ISR (PCINT1_vect){
  
}

void initInterrupts(void){
  PCICR |= (1 << PCIE1);
  PCMSK1 |= (1 << PCINT8 | 1 << PCINT9);
  sei();
}

void initInputs(){
  pinMode(INPUT, BT1);
  pinMode(INPUT, BT2);
  pinMode(INPUT, RING_BT);
}

unsigned long lastmls = 0;

void displayDate(){
  char strDate[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  rtcManager.fillDateStringBuffer(strDate, 9);
  display.printStringAt(0, 0, strDate);
}

void displayTime(){
  char strTime[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  rtcManager.fillTimeStringBuffer(strTime, 9);
  display.printStringAt(0, 1, strTime);
}

void displayTimeDate(){
  displayDate();
  displayTime();
}

void setup() {
  display.init();
  rtcManager.init();
  initInputs();
  initInterrupts();
  displayTimeDate();
}

void loop() {

  unsigned long mls = millis();

  if (mls % 1000 == 0 && mls != lastmls){
    lastmls = mls;
    TMS++;
    displayTimeDate();
  }

}
