#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <Arduino.h>

class stateMachine
{
  private:
    String _currentState = "undefined";
    String _prevState = "undefined";
  public:
    stateMachine();

    String getCurrentState();
    String getPrevState();
    void setCurrentState(String newState);
    void updatePrevState();
    bool stateChanged();
};

#endif