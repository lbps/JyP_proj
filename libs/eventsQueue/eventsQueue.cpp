#include "eventsQueue.h"

eventsQueue::eventsQueue(unsigned int maxItemsInQueue){
    _eventsQueue = ArduinoQueue<String> (maxItemsInQueue);
};

uint8_t eventsQueue::getNumEvents (){
    return _eventsQueue.item_count();
};

bool eventsQueue::eventsAvailable (){
    return !_eventsQueue.isEmpty();
};

void eventsQueue::enqueueEvent (String newEvent){
    if (newEvent!=""){

        if(_eventsQueue.isFull()){
            // Serial.println("Cola llena, se borra mas antiguo."); //DEBUG
            String _ = _eventsQueue.dequeue();
        };

        _eventsQueue.enqueue(newEvent);
        //Serial.println("Queue size: "+String(this->getNumEvents())); //DEBUG
    };
};

String eventsQueue::dequeueEvent(){
    uint8_t numEvents = this->getNumEvents();
    String eventData = "";
    if(numEvents>0){
        eventData = _eventsQueue.dequeue();
        // Serial.println("new event: "+eventData); //DEBUG
    };
    return eventData;
};