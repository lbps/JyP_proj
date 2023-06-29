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
    String createSerializedEventJson (String sourceID, String eventType);
    void enqueueEvent (String serializedEventJson);
    String dequeueEvent();
    bool sendEventsToSerial(Stream &serialport);
};

#endif