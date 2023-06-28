#include "controlsManager.h"

controlsManager::controlsManager(){
};

void controlsManager::addNewButton(String buttonID, uint8_t pin, uint8_t pinMode, uint16_t debounceInterval_ms){
    _buttonControl.addNewButton(buttonID, pin, pinMode, debounceInterval_ms);
};

void controlsManager::addNewEncoder(String encoderID, uint8_t pinCHA, uint8_t pinCHB){
    _encoderControl.addNewEncoder(encoderID, pinCHA, pinCHB);
};

void controlsManager::updateControls(){
    _buttonControl.updateButtons();
    _encoderControl.updateEncoders();
};

void controlsManager::sendEventsToSerial(Stream &serialport, unsigned long minEventSendingPeriod_us){

    //Se respeta tiempo minimo entre envio de eventos:
    unsigned long timeElapsed_us = micros() - _timeLastEventSent_us;
    if(timeElapsed_us>minEventSendingPeriod_us){

        //Se envian eventos que haya disponibles:
        bool eventSent = _eventsQueue.sendEventsToSerial(serialport);
        if(eventSent){
            _timeLastEventSent_us = micros();
        };
    };
};