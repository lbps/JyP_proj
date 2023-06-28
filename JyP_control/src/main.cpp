
#include <Arduino.h>
#include <SoftwareSerial.h>

#include "controlsManager.h"
#include "BTHC.h"

#pragma GCC diagnostic ignored "-pedantic"

controlsManager controlsManager1;

// SoftwareSerial SerialMaster(6,7);
BTHC BTmaster(&Serial3);

void setup()
{
    Serial.begin(9600);

    Serial3.begin(38400);

    // BTmaster.configureBTmaster("BTmaster", "1234", "38400");
    
    controlsManager1.addNewButton("BUT1", 22, INPUT_PULLUP, 10);
    controlsManager1.addNewEncoder("ENC1", 18,19);
    Serial.println("Inicio envio...");
}

void loop()
{
    //Actualizacion eventos de boton:
    controlsManager1.updateControls();
    
    //Se respeta tiempo minimo entre envio de eventos:;
    controlsManager1.sendEventsToSerial(Serial3, 500);
};