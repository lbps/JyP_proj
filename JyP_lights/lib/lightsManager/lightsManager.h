#ifndef LIGHTSMANAGER_H
#define LIGHTSMANAGER_H

#include "eventsQueue.h"
#include "BTHC.h"
#include "ringLED.h"
#include "letterLED.h"

#pragma GCC diagnostic ignored "-pedantic"

class lightsManager{
private:
    eventsQueue _eventsQueue = eventsQueue(20);

    ringLED ringLED_e = ringLED(110, 2, 0);
    // ringLED ringLED_i = ringLED(80, 3, 50);
    // letterLED letterLED_J = letterLED(16, 4, 0);  
    // letterLED letterLED_Y = letterLED(6, 5, 0);  
    // letterLED letterLED_P = letterLED(17, 6, 0);  

public:
    lightsManager ();

    void receiveEventsFromBT (BTHC &BTobj);

    void updateLights ();
};

#endif