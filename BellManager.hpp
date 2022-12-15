#pragma once

class BellManager {
private:
  unsigned int  _hammerTimeMaintainedMS;
  bool          _isRinging;
  unsigned int  _isRingingSince;
  unsigned char _bellOutputPort;
  unsigned long _lastMLS;

  void handleRingings();
  void setHammerOff();
  void setHammerOn();
public:
  BellManager(unsigned char outputPort);
  void init();
  bool ring();
  void loop();
};
