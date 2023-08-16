
#include "ESP32BTSerial.h"

ESP32BTSerial::ESP32BTSerial ():
               BluetoothSerial(){
                
}

String ESP32BTSerial::readNextData(char endChar){

    String newData=""; //Por defecto, si no hay dato completo se saca string vacio
    
    while (available()) {
        char charReceived = char(read());
        if (charReceived != endChar) {
            _dataInProgress = _dataInProgress+String(charReceived);
        }else {
            newData=_dataInProgress;
            _dataInProgress="";
        }
    }

    return newData;
};