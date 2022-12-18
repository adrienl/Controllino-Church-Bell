#pragma once

#include "RelayAction.hpp"

class RelayManager {

private:
  RelayAction * _firstNodeAction;
  RelayAction * _nodeAction;
  unsigned long _actOnSince;
  unsigned long _actOffSince;
  bool _relayIsOn;
  unsigned long _loopLastMls;
  void (*_onActionStarted)(RelayAction *);
  void (*_onActionChanged)(RelayAction *);
  void (*_onActionEnded)(RelayAction *);
  void setRelayOn(unsigned char relayId);
  void setRelayOff(unsigned char relayId);
  
public:
  RelayManager();
  bool startFromAction(RelayAction * act);
  void loop();
  void setOnActionStarted(void (*func)(RelayAction *));
  void setOnActionChanged(void (*func)(RelayAction *));
  void setOnActionEnded(void (*func)(RelayAction *));
  
};
