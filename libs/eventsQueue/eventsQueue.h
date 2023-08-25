#ifndef EVENTSQUEUE_H
#define EVENTSQUEUE_H

#include <Arduino.h>
#include <ArduinoQueue.h>

class eventsQueue{
private:
    ArduinoQueue<String> _eventsQueue;

public:
    eventsQueue (unsigned int maxItemsInQueue);
    uint8_t getNumEvents ();
    bool eventsAvailable ();
    void enqueueEvent (String serializedEventJson);
    String dequeueEvent();
};

#endif