#ifndef BUTTONCONTROL_H
#define BUTTONCONTROL_H

#include <Arduino.h>
#include <Bounce2.h>

#include "eventsQueue.h"

class buttonControl{
private:
    Bounce2::Button _buttonArray [30];
    String _buttonIDsArray [30];
    uint8_t _numButtons = 0;
    eventsQueue *_eventsQueue;

public:
    buttonControl (eventsQueue *eventsQueue1);
    void addNewButton(String buttonID, uint8_t pin, uint8_t pinMode, uint16_t debounceInterval_ms);
    void updateButtons ();
};

#endif