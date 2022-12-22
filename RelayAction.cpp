#include "RelayAction.hpp"

RelayAction::RelayAction(unsigned char outputId, unsigned int onTiming, unsigned int offTiming) : _outputId(outputId), _onTiming(onTiming), _offTiming(offTiming), _nextAction(NULL), _previousAction(NULL){

}

RelayAction * RelayAction::getLastOfTheList(){
  RelayAction * node = this;
  while (node->_nextAction != NULL){
    node = node->_nextAction;
  }
  return node;
}

RelayAction * RelayAction::getFirstOfTheList(){
  RelayAction * node = this;
  while (node->_previousAction != NULL){
    node = node->_previousAction;
  }
  return node;
}

void  RelayAction::setNextAction(RelayAction * action){
  _nextAction = action;
}

RelayAction * RelayAction::getNextAction(){
  return _nextAction;  
}

void  RelayAction::setPreviousAction(RelayAction * action){
  _previousAction = action;
}

RelayAction * RelayAction::getPreviousAction(){
  return _previousAction;  
}

unsigned char RelayAction::getOutputId(){
  return _outputId;
}

unsigned int RelayAction::getOnTiming(){
  return _onTiming;
}

unsigned int RelayAction::getOffTiming(){
  return _offTiming;  
}

void RelayAction::setOnTiming(unsigned int timing){
  _onTiming = timing;
}

void RelayAction::setOffTiming(unsigned int timing){
  _offTiming = timing;
}

/* Static */

static RelayAction * RelayAction::buildAngelusActions(){
  RelayAction * act11 = RelayAction::buildAnyActions(3);
  RelayAction * act13 = act11->getLastOfTheList();
  act13->setOffTiming(14000);

  RelayAction * act21 = RelayAction::buildAnyActions(3);
  act21->setPreviousAction(act13);
  act13->setNextAction(act21);
  RelayAction * act23 = act21->getLastOfTheList();
  act23->setOffTiming(14000);

  RelayAction * act31 = RelayAction::buildAnyActions(3);
  act31->setPreviousAction(act23);
  act23->setNextAction(act31);
  RelayAction * act33 = act31->getLastOfTheList();
  act33->setOffTiming(15000);

  RelayAction * v = RelayAction::buildAnyActions(10);
  act33->setNextAction(v);
  v->setPreviousAction(act33);
  v->getLastOfTheList()->setOffTiming(200);
  return act11;
}

static void RelayAction::deleteAllNodes(RelayAction * act){
  act = act->getFirstOfTheList();
  RelayAction * nextNode;
  while (act != NULL){
    nextNode = act->getNextAction();
    delete act;
    act = nextNode;
  }
}

static RelayAction * RelayAction::buildAnyActions(unsigned int len){
  int i = 1;
  if (len == 0){return NULL;}
  RelayAction * lastAction = new RelayAction(CONTROLLINO_D0, BELL_TIMING, 2000);
  lastAction->setPreviousAction(NULL);
  while (i < len){
    RelayAction * act = new RelayAction(CONTROLLINO_D0, BELL_TIMING, 2000);
    lastAction->setNextAction(act);
    act->setPreviousAction(lastAction);
    act->setNextAction(NULL);
    lastAction = act;
    i++;
  }
  RelayAction * backToFirst = lastAction->getFirstOfTheList();
  return backToFirst;
}

static RelayAction * RelayAction::buildSimpleAction(){
  RelayAction * act = buildAnyActions(1);
  act->setOffTiming(200);
  return act;
}

static RelayAction * RelayAction::buildTripleAction(){
  RelayAction * act = RelayAction::buildAnyActions(3);
  act->getLastOfTheList()->setOffTiming(200);
  return act;
}

static RelayAction * RelayAction::buildFiveAction(){
  RelayAction * act = RelayAction::buildAnyActions(5);
  act->getLastOfTheList()->setOffTiming(200);
  return act;
}
