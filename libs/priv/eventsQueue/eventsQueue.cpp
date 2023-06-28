#include "eventsQueue.h"

#include <ArduinoJson.h>

eventsQueue::eventsQueue(unsigned int maxItemsInQueue){
    this->_eventsQueue = ArduinoQueue<String> (maxItemsInQueue);
};

uint8_t eventsQueue::getNumEvents (){
    return _eventsQueue.item_count();
};

bool eventsQueue::eventsAvailable (){
    return !_eventsQueue.isEmpty();
};

String eventsQueue::createSerializedEventJson (String sourceID, String eventType){
    StaticJsonDocument<100> eventMessage;
    eventMessage["srcID"]=sourceID;
    eventMessage["type"]=eventType;
    String eventMessage_str;
    serializeJson(eventMessage, eventMessage_str);
    return eventMessage_str;
};

void eventsQueue::enqueueEvent (String serializedEventJson){
    if (serializedEventJson!=""){

        if(_eventsQueue.isFull()){
            Serial.println("Cola llena, se borra mas antiguo."); //DEBUG
            String _ = _eventsQueue.dequeue();
        };

        _eventsQueue.enqueue(serializedEventJson);
        //Serial.println("Queue size: "+String(this->getNumEvents())); //DEBUG
    };
};

String eventsQueue::dequeueEvent(){
    uint8_t numEvents = this->getNumEvents();
    String eventJson = "";
    if(numEvents>0){
        eventJson = _eventsQueue.dequeue();
        // Serial.println("new event: "+eventJson); //DEBUG
    };
    return eventJson;
};

bool eventsQueue::sendEventsToSerial(Stream &serialport){
    bool eventSent = 0;
    if(this->eventsAvailable()){
        String eventJson = this->dequeueEvent();
        serialport.println (eventJson);
        eventSent = 1;
        Serial.println("Se ha enviado dato: "+ eventJson); //DEBUG
    };

    return eventSent;
};