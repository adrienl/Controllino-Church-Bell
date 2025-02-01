#pragma once

#include <Controllino.h>
#define BELL_TIMING 60

class RelayAction{
  private:
    unsigned char _outputId;
    unsigned int _offTiming;
    RelayAction * _previousAction;
    RelayAction * _nextAction;
  public:
    RelayAction(unsigned char ouputId, unsigned int offTiming);
    void setOffTiming(unsigned timing);
    void setOnTiming(unsigned timing);
    void setNextAction(RelayAction * action);
    void setPreviousAction(RelayAction * action);
    RelayAction * getNextAction();
    unsigned char getOutputId();
    unsigned int getOffTiming();
    RelayAction * getLastOfTheList();
    RelayAction * getFirstOfTheList();
    static RelayAction * buildAngelusActions();
    static RelayAction * buildSimpleAction();
    static RelayAction * buildTripleAction();
    static RelayAction * buildFiveAction();
    static RelayAction * buildAnyActions(unsigned int len);
    static void deleteAllNodes(RelayAction * act);
};
