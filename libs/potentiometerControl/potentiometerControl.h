#ifndef POTENTIOMETERCONTROL_H
#define POTENTIOMETERCONTROL_H

#include <Arduino.h>
#include "eventsQueue.h"

class potentiometerControl{
private:
    uint8_t _pinAnalogInputArray [5];
    uint16_t _maxAnalogReadValueArray [5];
    String _potentiometerIDsArray [5];
    uint8_t _potentiometerValueArray [5];
    uint8_t _numPotentiometers = 0;
    eventsQueue *_eventsQueue;

public:
    potentiometerControl (eventsQueue *eventsQueue1);
    void addNewPotentiometer(String potentiometerID, uint8_t pinAnalogInput, uint16_t maxAnalogReadValue = 4095);
    void updatePotentiometers ();
};

#endif