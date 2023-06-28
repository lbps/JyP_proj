
#include "lightsManager.h"

lightsManager::lightsManager(){
};

void lightsManager::receiveEventsFromBT(BTHC &BTobj){

    String newData;
    while(1){
        newData = BTobj.readNextData();

        //Si no hay nuevos eventos se sale directamente:
        if(newData ==""){
            break;

        //Si hay eventos se encolan:
        }else{
            _eventsQueue.enqueueEvent(newData);
            Serial.println("Dato recibido: "+newData); //DEBUG
        } 
    };
};

void lightsManager::updateLights (){

};