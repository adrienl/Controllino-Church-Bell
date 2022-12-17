#include "Event.hpp"

Event::Event(String title, DateTime dt) : _title(title), _datetime(dt){
  
}

DateTime Event::getDateTime(){
  return _datetime;
}

unsigned long Event::getUTCTimestamp(){
  return _datetime.getUTCTimestamp();
}

String Event::getTitle(){
  return _title;  
}
