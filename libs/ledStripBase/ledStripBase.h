#ifndef LEDSTRIPBASE_H
#define LEDSTRIPBASE_H

#include <Arduino.h>
#include <string.h>
#include <Adafruit_NeoPixel.h>

class ledStripBase: public Adafruit_NeoPixel
{
private:
    uint16_t _numLeds;
    uint8_t _dataPin;

    //variables necesarias para realizar los efectos:
    uint8_t _mainHue = 0;
    uint16_t _currentPixel = 0; 
    uint16_t _sequenceIdx = 0;
    unsigned long _lastUpdateTime = millis();

public:

    //Constructor:
    ledStripBase (uint16_t numLeds, uint8_t dataPin);

    //Destructor:
    ~ledStripBase();

    //getters:
    uint8_t getMainHue ();

    //setters:
    void setMainHue (uint8_t mainHue);

    //Metodo para inicializar la tira con todos leds apagados:
    void initStrip(uint8_t initialBrightness = 50);

    //Metodos apra cambiar entre efectos automaticos

    //Definicion de metodos de efectos automaticos comunes:
    void rainbowEffect(uint8_t colorStep = 1);

    void colorWipeEffect(bool randomColor = 1, bool forwardDirection = 1, uint8_t colorStep = 1);

    void theaterChaseEffect(bool forwardDir = 1, bool autoColorChange = 0, uint16_t numSequencesToChangeColor = 10, uint16_t spaceBetweenLeds = 3, uint16_t numAdjacentLedsOn = 1);



};

#endif