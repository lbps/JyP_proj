#ifndef ENCODERSTATE_H
#define ENCODERSTATE_H

#include <Arduino.h>

class encoderState
{
  private:
    int16_t _prevPosition = 0;
    int16_t _currentPosition = 0;
    unsigned long _currentPosStartTime = 0;
    uint8_t _eventsFilter_groupSize;
    int8_t _eventsFilter_sum=0;
    uint8_t _eventsFilter_cnt=0; 
  public:
    encoderState(uint8_t eventsFilter_groupSize = 1);

    int16_t getCurrentPosition();
    int16_t getPrevPosition();
    int16_t getIncrement(bool unitary = 0);
    void updateState(String eventValue="");
    bool stateChanged();
    unsigned long getTimeInCurrentState();
};

#endif