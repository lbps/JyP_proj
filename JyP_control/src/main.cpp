
#include <Arduino.h>
#include "ESP32BTSerial.h"
#include "eventsQueue.h"
#include "controlsManager.h"

controlsManager controlsManager1 = controlsManager();

//INICIALIAZCION DE MODULO BLUETOOTH
ESP32BTSerial SerialBT; //MAC del ESCLAVO JyP_lights: A8:42:E3:CA:CB:3C
//String slaveAddress = "A8:42:E3:CA:CB:3C"; //MAC del ESCLAVO JyP_lights: A8:42:E3:CA:CB:3C
String BTmasterName = "JyP_control";
String BTslaveName = "JyP_lights";

//Se indican los pines de LEDs de encendido, y de conexión
const uint8_t LED_power = 1;
const uint8_t LED_BTcomunnication = 3;

void setup()
{
    //Se inicia configuraion
    Serial.begin(9600);
    Serial.println("Inicio programa JyP_control");

    //Se añaden al manager de control todos los botones y encoders
    controlsManager1.addNewButton("BR1", 36, INPUT_PULLUP, 10);
    controlsManager1.addNewButton("BB1", 39, INPUT_PULLUP, 10);
    controlsManager1.addNewButton("BG1", 34, INPUT_PULLUP, 10);
    controlsManager1.addNewButton("BY1", 35, INPUT_PULLUP, 10);
    controlsManager1.addNewButton("BW1", 4, INPUT_PULLUP, 10);
    controlsManager1.addNewButton("BF1", 15, INPUT_PULLUP, 10);
    controlsManager1.addNewButton("BR2", 25, INPUT_PULLUP, 10);
    controlsManager1.addNewButton("BB2", 26, INPUT_PULLUP, 10);
    controlsManager1.addNewButton("BG2", 27, INPUT_PULLUP, 10);
    controlsManager1.addNewButton("BY2", 14, INPUT_PULLUP, 10);
    controlsManager1.addNewButton("BW2", 12, INPUT_PULLUP, 10);
    controlsManager1.addNewButton("BF2", 13, INPUT_PULLUP, 10);
    controlsManager1.addNewButton("BRE", 22, INPUT_PULLUP, 10);
    controlsManager1.addNewButton("BRI", 23, INPUT_PULLUP, 10);
    controlsManager1.addNewButton("BC", 2, INPUT_PULLUP, 10);
    controlsManager1.addNewEncoder("ERE", 5,18);
    controlsManager1.addNewEncoder("ERI", 19,21);
    controlsManager1.addNewEncoder("EC", 16,17);
    // controlsManager1.addNewPotentiometer("P1", 32);
    // controlsManager1.addNewPotentiometer("P2", 33);

    //Se configuran los LEDs de estado:
    // setup pin 5 as a digital output pin
    pinMode (LED_power, OUTPUT);
    pinMode (LED_BTcomunnication, OUTPUT);
    digitalWrite (LED_power, HIGH);	
    digitalWrite (LED_BTcomunnication, LOW);

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

    //Se enciende LED de estado de comunicacion BT:
    digitalWrite (LED_BTcomunnication, HIGH);
    
    Serial.println("Conectado a: "+BTslaveName);
}

void loop()
{
    // //Comprobacion de conexion con esclavo BT:
    if(!SerialBT.connected()){
        //Se apaga LED de estado de comunicacion BT:
        digitalWrite (LED_BTcomunnication, LOW);

    //     //Se intenta reconectar:
        while (!SerialBT.connected()) {
            Serial.println("Intentando reconectar con '"+BTslaveName+"' ...");
            SerialBT.connect(BTslaveName); // Intentar reconectar al esclavo
            delay(500);
        }

        //Se enciende LED de estado de comunicacion BT:
        digitalWrite (LED_BTcomunnication, HIGH);

        Serial.println("Se ha reconectado a : "+BTslaveName);
    }
   
    //Actualizacion eventos de boton:
    controlsManager1.updateControls();

    
    //Se respeta tiempo minimo entre envio de eventos:;
    controlsManager1.sendEventsToSerial(SerialBT, 500);
};