#ifndef BTHC_H
#define BTHC_H

#include <Arduino.h>

class BTHC
{
private:
    Stream *_serialPort;
    String _dataInProgress = "";

public:
    //Constructor:
    BTHC (Stream *serialPort);

    //Declaracion de funcinoes:
    String readNextData(char endChar = '\n');
    bool waitBTconfigACK(unsigned long timeout_ms = 0);
    bool configureBTmaster(String name, String password, String baudRate);
    bool configureBTslave(bool isHC05, String name, String password, String baudRate);
};


#endif