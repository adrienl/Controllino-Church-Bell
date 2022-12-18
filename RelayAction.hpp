#pragma once

#include <Controllino.h>
#define BELL_TIMING 100

class RelayAction{
  private:
    int _outputId;
    unsigned int _onTiming;
    unsigned int _offTiming;
    RelayAction * _previousAction;
    RelayAction * _nextAction;
  public:
    RelayAction(unsigned char ouputId, unsigned int onTiming, unsigned int offTiming);
    void setOffTiming(unsigned timing);
    void setOnTiming(unsigned timing);
    void setNextAction(RelayAction * action);
    void setPreviousAction(RelayAction * action);
    RelayAction * getNextAction();
    RelayAction * getPreviousAction();
    unsigned char getOutputId();
    unsigned int getOnTiming();
    unsigned int getOffTiming();
    RelayAction * getLastOfTheList();
    RelayAction * getFirstOfTheList();
    static RelayAction * buildAngelusActions();
    static RelayAction * buildSimpleAction();
    static RelayAction * buildTripleAction();
    static RelayAction * buildAnyActions(unsigned int len);
    static void deleteAllNodes(RelayAction * act);
    static void print(RelayAction * act);
};
