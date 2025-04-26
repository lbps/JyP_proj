#ifndef CONTROLSMANAGER_H
#define CONTROLSMANAGER_H

#include "eventsQueue.h"
#include "buttonControl.h"
#include "encoderControl.h"
#include "potentiometerControl.h"

class controlsManager{
private:
    eventsQueue _eventsQueue = eventsQueue(20);
    
    //Se genera controlador de botones:
    buttonControl _buttonControl = buttonControl(&_eventsQueue);

    //Se genera controlador de encoders:
    encoderControl _encoderControl = encoderControl(&_eventsQueue);

    //Se genera controlador de potenciometros:
    potentiometerControl _potentiometerControl = potentiometerControl(&_eventsQueue);

    //Se inicializa ultimo timestamp de envio de dato
    unsigned long _timeLastEventSent_us = micros();
public:
    controlsManager ();
    void addNewButton(String buttonID, uint8_t pin, uint8_t pinMode, uint16_t debounceInterval_ms);
    void addNewEncoder(String encoderID, uint8_t pinCHA, uint8_t pinCHB);
    void addNewPotentiometer(String potentiometerID, uint8_t pinAnalogInput, uint16_t maxAnalogReadValue = 4095);
    void updateControls ();
    void sendEventsToSerial (Stream &serialport, unsigned long minEventSendingPeriod_us);
};

#endif