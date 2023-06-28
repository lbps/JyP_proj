#ifndef LETTERLED_H
#define LETTERLED_H

#include <Arduino.h>
#include <string.h>
#include <FastLED.h>

#include "ledStripBase.h"

class letterLED: ledStripBase
{
private:
    String _visList [6] = {"rainbow", "rainbowWithGlitter", "confetti", "sinelon", "bpm", "juggle"};
public:

    //Constructor:
    letterLED (uint8_t numLeds, uint8_t dataPin, uint8_t mainHue);
        
    //Destructor:
    ~letterLED();

    //getters:
    uint8_t getNumLeds ();
    String getVisualizationMode ();
    uint8_t getVisualizationModeIdx ();
    String getRunState ();
    uint8_t getMainHue ();
    CRGB getLedValue (uint8_t led_idx);
    
    //setters:
    void setMainHue (uint8_t mainHue);

    //Metodos Efectos (DemoReel100):
    void rainbow() ;
    void rainbowWithGlitter() ;
    void addGlitter( fract8 chanceOfGlitter);
    void confetti();
    void sinelon();
    void bpm();
    void juggle();

    //Metodos apra cambiar entre efectos automaticos
    void runEffect();
};



#endif