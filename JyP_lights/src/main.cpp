#include <Arduino.h>
#include <SoftwareSerial.h>

// #include "BTHC.h"
// #include "lightsManager.h"
// #include "ringLED.h"
#include "ledStripBase.h"

#pragma GCC diagnostic ignored "-pedantic"

// SoftwareSerial SerialSlave(8,9);
// BTHC BTslave(&SerialSlave);
// lightsManager lightsManager1;

ledStripBase ledStrip1 = ledStripBase(110, 6);
// Adafruit_NeoPixel strip = Adafruit_NeoPixel(110, 6, NEO_GRB + NEO_KHZ800);

void setup() {
   Serial.begin(9600);

  //Se confiugra el BTslave.
    // SerialSlave.begin(38400); 
    // BTslave.configureBTslave(0, "BTslave", "1234", "38400");
    // Serial.println("Inicio escucha...");

    Serial.println("Inicio programa...");

    ledStrip1.initStrip(50);
}



void loop() {
  // lightsManager1.receiveEventsFromBT(BTslave);

  // ledStrip1.rainbowEffect(3);
  ledStrip1.colorWipeEffect(0, 0, 40);

  //Si esta listo para visualizar de nuevo se visualiza cambios:
  if(ledStrip1.canShow()) ledStrip1.show();
  

  // ledStrip1.fill(ledStrip1.ColorHSV(map(millis() % 256, 0, 255, 0, 65535)));
// strip.show();
  // ledStrip1.clear();
  // ledStrip1.show();
  // ledStrip1.rainbowEffect();

  // ledStrip1.show();

  delay(1);
  
}