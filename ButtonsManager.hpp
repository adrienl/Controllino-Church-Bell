#ifndef __BUTTONS_MANAGER
#define __BUTTONS_MANAGER

#include <Controllino.h>

#define NB_INPUT 6

class ButtonsManager {

public:
    static const unsigned char BT_MINUS = CONTROLLINO_A0;
    static const unsigned char BT_PLUS = CONTROLLINO_A1;
    static const unsigned char BT_SEL = CONTROLLINO_A2;
    static const unsigned char BT_MENU = CONTROLLINO_A3;
    static const unsigned char BT_ONE_PULSE = CONTROLLINO_IN0;
    static const unsigned char BT_ANGELUS = CONTROLLINO_IN1;
    ButtonsManager();
    void loop();
    void setOnButtonPushed(void (*onButtonPushed)(unsigned int bt));
    void setOnButtonReleased(void (*onButtonReleased)(unsigned int bt));
    void initButtons();

private:
  unsigned char _bts[NB_INPUT] = {BT_MINUS, BT_PLUS, BT_SEL, BT_MENU, BT_ONE_PULSE, BT_ANGELUS};
  bool _bts_pushed[NB_INPUT] = {false, false, false, false, false, false};
  void (*_onButtonPushed) (unsigned int bt);
  void (*_onButtonReleased) (unsigned int bt);
};

#endif
