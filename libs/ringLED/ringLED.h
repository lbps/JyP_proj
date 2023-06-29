#ifndef RINGLED_H
#define RINGLED_H

#include <Arduino.h>
#include <string.h>

#include "ledStripBase.h"

class ringLED: ledStripBase
{
private:
    String _visList [6] = {"rainbow"};
public:

    //Constructor:
    ringLED (uint8_t numLeds, uint8_t dataPin, uint8_t mainHue);
        
    //Destructor:
    ~ringLED();

    //Metodos Efectos automaticos:
    void rainbow() ;

    //Efectos JyP interaccion con control:
    // void ringWheel(uint8_t ledIdx, uint8_t fadeToBlackPercent);
    // void ringFlash(bool newFlash, CRGB rgbFlash, uint8_t fadeToBlackPercent);
};



#endif