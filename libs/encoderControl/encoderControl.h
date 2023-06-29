#ifndef ENCODERCONTROL_H
#define ENCODERCONTROL_H

#include <Arduino.h>
#include <Encoder.h>

#include "eventsQueue.h"

class encoderControl{
private:
    Encoder *_encoderArray [5];
    int32_t _encoderPosArray [5];
    String _encoderIDsArray [5];
    uint8_t _numEncoders = 0;
    eventsQueue *_eventsQueue;

public:
    encoderControl (eventsQueue *eventsQueue1);
    void addNewEncoder(String encoderID, uint8_t pinCHA, uint8_t pinCHB);
    void updateEncoders ();
};

#endif