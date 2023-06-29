
#include "BTHC.h"

BTHC::BTHC (Stream *serialPort){
    _serialPort=serialPort;
};

String BTHC::readNextData(char endChar = '\n'){

    String newData=""; //Por defecto, si no hay dato completo se saca string vacio
    
    while (_serialPort->available() > 0) {
        char charReceived = _serialPort->read();

        if (charReceived != endChar) {
            _dataInProgress = _dataInProgress+String(charReceived);
        }else {
            newData=_dataInProgress;
            _dataInProgress="";
        }
    }

    return newData;
};

bool BTHC::waitBTconfigACK(unsigned long timeout_ms = 0){

    //Inicializaicon de dato leido: 
    String response="";

    //Extraccion de respuesta del modulo BT.
    unsigned long t1=millis();
    unsigned long t2=millis();
    while((t2-t1)<timeout_ms or timeout_ms==0){
        response=this->readNextData('\n');
        if(response!=""){
            break;
        }
        t2=millis();
    };

    //Comprobacion de respuesta de modulo BT
    if(response==String("OK\r")){
        return true;
    }else{
        return false;
    };

}


bool BTHC::configureBTmaster(String name, String password, String baudRate){

    //ATENCION!! PARA CONFIGURAR HC05 HAY QUE CONFIGURAR BAUDRATE DE SERIAL A 38400
    
    //inicializacion de variable ACK de envio de comandos AT2:
    bool ACK = false;
    
    //Espera para avisar de pulsar boton del modulo:
    Serial.println("Pulsar boton modulo antes de 4 seg...");
    delay(4000);

    //Envio de comandos de configuracion al modulo BT:
    Serial.println("Comprobacion conexion en configuracion AT:");
    _serialPort->print("AT\r\n"); //Para comprobar que modulo responde a comandos AT. Hay que indicarle salto de linea para que detecte final de comando.
    ACK=this->waitBTconfigACK();
    if(ACK==true){
        Serial.println("OK");
    }else{
        Serial.println("Modulo BT en modo conectado/comunicacion. No se configura al no estar en modo AT.");
        return 0;
    }
    
    //- Configuracion de ROL modulo BT
    //--------------------------------
    Serial.println("Configuracion ROL modulo BT:");  
    _serialPort->print("AT+ROLE=1\r\n"); //MODO MAESTRO (1)
    ACK=this->waitBTconfigACK(); 
    if(ACK==true) Serial.println("OK");
    
    //- Configuracion de NOMBRE modulo BT
    //-----------------------------------
    Serial.println("Configuracion nombre modulo BT:");
    _serialPort->print("AT+NAME="+name+"\r\n");
    ACK=waitBTconfigACK(); 
    if(ACK==true) Serial.println("OK");

    //- Configuracion de PASSWORD modulo BT
    //--------------------------------------
    Serial.println("Configuracion password modulo BT:");
    _serialPort->print("AT+PSWD=\""+password+"\"\r\n");
    ACK=waitBTconfigACK(); 
    if(ACK==true) Serial.println("OK");

    //Configuracion de VELOCIDAD modulo BT
    //-------------------------------------
    Serial.println("Configuracion velocidad UART modulo BT:");
    _serialPort->print("AT+UART="+baudRate+",0,0\r\n");
    ACK=waitBTconfigACK(); 
    if(ACK==true) Serial.println("OK");

    //Configuracion MODO CONEXION modulo BT
    //-------------------------------------
    Serial.println("Configuracion modo conexion con esclavos:");
    _serialPort->print("AT+CMODE=1\r\n"); //conexion con cualquier esclavo que sepa PASSWORD
    ACK=waitBTconfigACK(); 
    if(ACK==true) Serial.println("OK");

    //Configuracion de MAC de modulo BT esclavo:
    //------------------------------------------
    // Serial.println("Configuracion direccion del esclavo:");
    // BTserial->print("AT+BIND=\"FCA8,9A,002F9A\"\r\n"); //conexion con unicamente el esclavo especificado en BIND  
    // ACK=waitBTconfigACK(); 
    // if(ACK==true) Serial.println("OK");
    
    //- Se resetea el modulo y se sale del modo configuracion:
    //--------------------------------------------------------
    Serial.println("Saliendo del modo de configuracion AT2. Soltar boton modulo en 4 segundos...");  
    delay(4000);
    _serialPort->print("AT+RESET\r\n"); 
    ACK=waitBTconfigACK(); 
    if(ACK==true) Serial.println("OK");

    //Modulo BT configurado 
    // --------------------
    Serial.println("\nModulo BT master configurado.");

    return 1;
}

bool BTHC::configureBTslave(bool isHC05,String name, String password, String baudRate){

    //ATENCION!! PARA CONFIGURAR HC05 HAY QUE CONFIGURAR BAUDRATE DE SERIAL A 38400
    //           PARA CONFIGURAR HC06 HAY QUE CONFIGURAR BAUDRATE DE SERIAL A 9600

    //inicializacion de variable ACK de envio de comandos AT:
    bool ACK = false;
    
    //Espera para avisar de pulsar boton del modulo:
    Serial.println("Inicio configuracion AT slave");

    //Envio de comandos de configuracion al modulo BT:
    Serial.println("Comprobacion conexion en configuracion AT:");
    _serialPort->print("AT\r\n"); //Para comprobar que modulo responde a comandos AT. Hay que indicarle salto de linea para que detecte final de comando.
    ACK=this->waitBTconfigACK();
    if(ACK==true){
        Serial.println("OK");
    }else{
        Serial.println("Modulo BT en modo conectado/comunicacion. No se configura al no estar en modo AT.");
        return 0;
    }
    
    //- Configuracion de ROL modulo BT
    //--------------------------------
    if(isHC05){
        Serial.println("Configuracion ROL modulo BT:");  
        _serialPort->print("AT+ROLE=0\r\n"); //MODO ESCLAVO (0)
        ACK=this->waitBTconfigACK(100); 
        if(ACK==true) Serial.println("OK");
    }
    

    //- Configuracion de NOMBRE modulo BT
    //-----------------------------------
    Serial.println("Configuracion nombre modulo BT:");
    _serialPort->print("AT+NAME="+name+"\r\n");
    ACK=waitBTconfigACK(); 
    if(ACK==true) Serial.println("OK");

    //- Configuracion de PASSWORD modulo BT
    //--------------------------------------
    Serial.println("Configuracion password modulo BT:");
    _serialPort->print("AT+PSWD=\""+password+"\"\r\n");
    ACK=waitBTconfigACK(); 
    if(ACK==true) Serial.println("OK");

    //Configuracion de VELOCIDAD modulo BT
    //-------------------------------------
    Serial.println("Configuracion velocidad UART modulo BT:");
    _serialPort->print("AT+UART="+baudRate+",0,0\r\n");
    ACK=waitBTconfigACK(); 
    if(ACK==true) Serial.println("OK");
    
    //- Se resetea el modulo y se sale del modo configuracion:
    //--------------------------------------------------------
    Serial.println("Saliendo del modo de configuracion AT2. Soltar boton modulo en 4 segundos...");  
    delay(4000);
    _serialPort->print("AT+RESET\r\n"); 
    ACK=waitBTconfigACK(); 
    if(ACK==true) Serial.println("OK");

    //Modulo BT configurado 
    // --------------------
    Serial.println("\nModulo BT slave configurado.");

    return 1;
}