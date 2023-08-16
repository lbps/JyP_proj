#ifndef ESP32BTSERIAL_H
#define ESP32BTSERIAL_H

#include <Arduino.h>
#include "BluetoothSerial.h"

/* Check if Bluetooth configurations are enabled in the SDK */
/* If not, then you have to recompile the SDK */
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

class ESP32BTSerial: public BluetoothSerial
{
private:
    String _dataInProgress = "";

public:
    //Constructor:
    ESP32BTSerial ();

    //Declaracion de funcinoes:
    String readNextData(char endChar = '\n');
};


#endif