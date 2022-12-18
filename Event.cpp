#include "Event.hpp"

Event::Event(E_EventType et, DateTime dt) : _eventType(et), _datetime(dt){
  
}

DateTime Event::getDateTime(){
  return _datetime;
}

unsigned long Event::getUTCTimestamp(){
  return _datetime.getUTCTimestamp();
}

E_EventType Event::getEventType(){
  return _eventType;  
}
