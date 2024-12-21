#include "Event.hpp"

Event::Event(eEventType et, DateTime dt) : _eventType(et), _datetime(dt){
  
}

DateTime Event::getDateTime(){
  return _datetime;
}

unsigned long Event::getUTCTimestamp(){
  return _datetime.getUTCTimestamp();
}

eEventType Event::getEventType(){
  return _eventType;  
}
