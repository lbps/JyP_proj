#include "controlsManager.h"

controlsManager::controlsManager(){
    
};

void controlsManager::addNewButton(String buttonID, uint8_t pin, uint8_t pinMode, uint16_t debounceInterval_ms){
    _buttonControl.addNewButton(buttonID, pin, pinMode, debounceInterval_ms);
};

void controlsManager::addNewEncoder(String encoderID, uint8_t pinCHA, uint8_t pinCHB){
    _encoderControl.addNewEncoder(encoderID, pinCHA, pinCHB);
};

void controlsManager::addNewPotentiometer(String potentiometerID, uint8_t pinAnalogInput, uint16_t maxAnalogReadValue){
    _potentiometerControl.addNewPotentiometer(potentiometerID, pinAnalogInput, maxAnalogReadValue);
};

void controlsManager::updateControls(){
    _buttonControl.updateButtons();
    _encoderControl.updateEncoders();
    _potentiometerControl.updatePotentiometers();
};

void controlsManager::sendEventsToSerial(Stream &serialport, unsigned long minEventSendingPeriod_us){

    //Se respeta tiempo minimo entre envio de eventos:
    unsigned long timeElapsed_us = micros() - _timeLastEventSent_us;
    if(timeElapsed_us>minEventSendingPeriod_us){

        //Si hay algún evento se envia:
        bool eventSent = 0;
        if(_eventsQueue.eventsAvailable()){
            String controlEvent = _eventsQueue.dequeueEvent();
            serialport.println(controlEvent);
            eventSent = 1;
            // Serial.println("Enviado evento: "+controlEvent);
        };

        //Se actualiza el tiempo desde ulimo evento enviado:
        if(eventSent){
            _timeLastEventSent_us = micros();
        };
    };
};