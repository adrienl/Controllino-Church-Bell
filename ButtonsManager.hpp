#ifndef __BUTTONS_MANAGER
#define __BUTTONS_MANAGER

#include <Controllino.h>

#define NB_INPUT 4

class ButtonsManager {

public:
    static const unsigned char BT_MIN_PLUS = CONTROLLINO_A1;
    static const unsigned char BT_MIN_MINUS = CONTROLLINO_A0;
    static const unsigned char BT_ONE_PULSE = CONTROLLINO_IN0;
    static const unsigned char BT_ANGELUS = CONTROLLINO_IN1;
    ButtonsManager();
    void loop();
    void setOnButtonPushed(void (*onButtonPushed)(unsigned int bt));
    void setOnButtonReleased(void (*onButtonReleased)(unsigned int bt));
    void initButtons();

private:
  unsigned char _bts[NB_INPUT] = {BT_ONE_PULSE, BT_ANGELUS, BT_MIN_PLUS, BT_MIN_MINUS};
  bool _bts_pushed[NB_INPUT] = {false, false, false, false};
  void (*_onButtonPushed) (unsigned int bt);
  void (*_onButtonReleased) (unsigned int bt);
};

#endif