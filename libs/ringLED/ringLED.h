#ifndef RINGLED_H
#define RINGLED_H

#include <Arduino.h>
#include <string.h>

#include "ledStripBase.h"

class ringLED: public ledStripBase
{
private:

public:

    //Constructor:
    ringLED (uint8_t numLeds, uint8_t dataPin);
        
    //Destructor:
    ~ringLED();

    //Metodos Efectos:
    void followCurrentPixel(uint16_t lightTrailSize = 20);
};



#endif