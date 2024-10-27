
#include <Arduino.h>
#include "ESP32BTSerial.h"
#include "eventsQueue.h"
#include "controlsManager.h"

controlsManager controlsManager1;

//INICIALIAZCION DE MODULO BLUETOOTH
ESP32BTSerial SerialBT; //MAC del ESCLAVO JyP_lights: A8:42:E3:CA:CB:3C
//String slaveAddress = "A8:42:E3:CA:CB:3C"; //MAC del ESCLAVO JyP_lights: A8:42:E3:CA:CB:3C
String BTmasterName = "JyP_control";
String BTslaveName = "JyP_lights";

//Inicializacion de pin del LED de la placa para avisar de desconexion BT
const int disconnectionLedPin = 2; // Pin del LED, ajusta según tu placa

void setup()
{
    Serial.begin(9600);
    Serial.println("Inicio programa JyP_control");

    controlsManager1.addNewButton("BW1", 26, INPUT_PULLUP, 10);
    controlsManager1.addNewEncoder("ERE", 32,33);

    //Se inicializa el pin de conexion
    pinMode(disconnectionLedPin, OUTPUT); // Configura el pin del LED de la placacomo salida
    digitalWrite(disconnectionLedPin, LOW); // Se apaga el LED inicialmente

    //Se inicializa conexion con esclavo BT:
    Serial.println("Inicio intento conexion con el esclavo: "+BTslaveName);
    SerialBT.begin(BTmasterName, 1); // Nombre del dispositivo Bluetooth
    SerialBT.connect(BTslaveName); // Intentar conectar al esclavo usando su dirección MAC

    // Esperar a que la conexión se establezca
    while (!SerialBT.connected()) {
        Serial.println("Esperando a conectar con '"+BTslaveName+"' ...");
        delay(1000);
        SerialBT.connect(BTslaveName);
    }

    //Se apaga el led indicador de desconexion
    digitalWrite(disconnectionLedPin, HIGH); // Se enciende el LED indicador de conexion.
    Serial.println("Conectado a: "+BTslaveName);
}

void loop()
{
    //Comprobacion de conexion con esclavo BT:
    if(!SerialBT.connected()){
        //Se enciende el led indicador de desconexion
        digitalWrite(disconnectionLedPin, LOW); // Se apa

        //Se intenta reconectar:
        while (!SerialBT.connected()) {
            Serial.println("Intentando reconectar con '"+BTslaveName+"' ...");
            SerialBT.connect(BTslaveName); // Intentar reconectar al esclavo
            delay(500);
        }

         //Se apaga el led indicador de desconexion
        digitalWrite(disconnectionLedPin, HIGH); // Se enciende el LED indicador de conexion.
        Serial.println("Se ha reconectado a : "+BTslaveName);
    }
   
    //Actualizacion eventos de boton:
    controlsManager1.updateControls();
    
    //Se respeta tiempo minimo entre envio de eventos:;
    controlsManager1.sendEventsToSerial(SerialBT, 500);
};