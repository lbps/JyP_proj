#include <Arduino.h>
#include "ringLED.h"
#include "ledStripBase.h"
#include "ESP32BTSerial.h"
#include "stateMachine.h"

//INICIALIAZCION DE MODULO BLUETOOTH
ESP32BTSerial SerialBT;

//INICIALIZACION DE TIRAS LED:
ringLED ringE = ringLED(110, 25);
ringLED ringI = ringLED(80, 26);
ledStripBase letterLED_J = ledStripBase(16, 27);
ledStripBase letterLED_Y = ledStripBase(6, 28);
ledStripBase letterLED_P = ledStripBase(17, 29);

//CREACION DE OBJETOS PARA ALMACENAR MAQUINAS DE ESTADO. 


void setup() {

  //Se confiugra el BTslave.
  // SerialBT.setPin("1234");
  SerialBT.begin("JyP_lights");

  //Se inicializan todas las tiras LED.
  ringE.initStrip(100);
  ringI.initStrip(100);
  letterLED_J.initStrip(100);
  letterLED_Y.initStrip(100);
  letterLED_P.initStrip(100);

  //DEBUG: Se inicializa Serial para logs
  Serial.begin(9600);

  //DEBUG: Se saca log de inicion de programa:
  Serial.println("Inicio programa...");
}

unsigned long t_start = millis();

void loop() {



  //Recopilacion de nuevos eventos:

  //Actualizacion de maquinas de estado:

  //Actualizacion de valor de pixel anterior de cada tira:
  ringE.updatePreviousPixel();
  ringI.updatePreviousPixel();
  letterLED_J.updatePreviousPixel();
  letterLED_Y.updatePreviousPixel();
  letterLED_P.updatePreviousPixel();

  //Actualizacion y ejecucion de efecto seleccionado:

  //Actualizacion de visualizacion de cada tira:
  ringE.updateDisplay();
  ringI.updateDisplay();
  letterLED_J.updateDisplay();
  letterLED_Y.updateDisplay();
  letterLED_P.updateDisplay();

}