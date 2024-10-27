#ifndef BUTTONSTATE_H
#define BUTTONSTATE_H

#include <Arduino.h>

class buttonState
{
  private:
    bool _currentState = 0;
    bool _prevState = 0;
    unsigned long _currentStateStartTime = 0;
    unsigned long _currentStateTime = 0;
    unsigned long _prevStateTime = 0;

  public:
    buttonState();

    bool getCurrentState();
    bool getPrevState();
    void updateState(String newState="");
    bool stateChanged();
    unsigned long getTimeInCurrentState();
    unsigned long getTimeInPreviousState();
};

#endif