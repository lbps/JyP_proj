#ifndef ENCODERSTATE_H
#define ENCODERSTATE_H

#include <Arduino.h>

class encoderState
{
  private:
    int16_t _prevPosition = 0;
    int16_t _currentPosition = 0;
    unsigned long _currentPosStartTime = 0;
  public:
    encoderState();

    int16_t getCurrentPosition();
    int16_t getPrevPosition();
    int16_t getIncrement(bool unitary = 0);
    void updateState(String newState="");
    bool stateChanged();
    unsigned long getTimeInCurrentState();
};

#endif