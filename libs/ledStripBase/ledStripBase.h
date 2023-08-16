#ifndef LEDSTRIPBASE_H
#define LEDSTRIPBASE_H

#include <Arduino.h>
#include <string.h>
#include <Adafruit_NeoPixel.h>
// #include <ArduinoJson.h>

class ledStripBase: public Adafruit_NeoPixel
{
private:
    uint16_t _numLeds;
    uint8_t _dataPin;

    //variables necesarias para realizar los efectos:
    uint8_t _mainHue = 0;
    uint16_t _currentPixel = 0;
    uint16_t _prevPixel = 0;
    uint16_t _sequenceIdx = 0;
    bool _firstEffectSequence = 1;
    unsigned long _lastUpdateTime = millis();

    // StaticJsonDocument<200> doc(); 

public:

    //Constructor:
    ledStripBase (uint16_t numLeds, uint8_t dataPin);

    //Destructor:
    ~ledStripBase();

    //getters:
    uint8_t getMainHue ();
    uint16_t getCurrentPixel();
    uint16_t getPrevPixel();
    uint16_t getSequenceIdx();
    bool firstEffectSequence();

    //setters:
    void setMainHue (uint8_t mainHue);
    void setCurrentPixel(uint16_t newCurrentPixel);
    void setSequenceIdx(uint16_t newSequenceIdx);

    //utils:
    uint16_t map8to16bit(uint8_t in);
    uint8_t map16to8bit(uint16_t in);
    uint16_t mapPixelIdxToRingStrip(int16_t pixelIdxToMap);
    void rgbToHsv(uint32_t color, uint16_t& hue, uint8_t& saturation, uint8_t& value);

    //Metodo para inicializar la tira con todos leds apagados:
    void initStrip(uint8_t initialBrightness = 50);

    //Metodo para actualizar pixel previo:
    void updatePreviousPixel();

    //Metodo para actualizar visualizacion de tira LED:
    void updateDisplay();

    //Definicion de metodos de efectos:
    void rainbowEffect(uint8_t colorStep = 1);

    void colorWipeEffect(bool randomColor = 1, bool forwardDirection = 1, uint8_t colorStep = 1);

    void theaterChaseEffect(bool forwardDir = 1, bool autoColorChange = 0, uint16_t numSequencesToChangeColor = 10, uint16_t spaceBetweenLeds = 3, uint16_t numAdjacentLedsOn = 1);
   
    void theaterChaseRainbowEffect(bool forwardDir = 1, uint16_t numSequencesToChangeColor = 10, uint16_t spaceBetweenLeds = 3, uint16_t numAdjacentLedsOn = 1);
    
    void sparkleEffect(uint16_t numLedsOn = 1); 

    void runningLightsEffect(bool forwardDirection = 1);

    void fadeDarkPixel(uint16_t pixelIdx, uint8_t fadeStep=1);

    void fadeDarkAll(uint8_t fadeStep = 1);

    void flashEffect(bool newFlash, uint8_t fadeSpeed = 80); 

    void basicKITTeffect(uint16_t lightTrailSize = 4, bool initialForwardDirection = 1, bool meteorTrailEffect = 0);

    void newKITTeffect(uint16_t lightTrailSize = 4, bool initialForwardDirection = 1, bool meteorTrailEffect = 0);
};

#endif