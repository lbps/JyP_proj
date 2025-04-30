#include <Arduino.h>
#include "ESP32BTSerial.h"
#include "eventsQueue.h"
#include "ringLED.h"
#include "ledStripBase.h"
#include "buttonState.h"
#include "encoderState.h" 

//INICIALIAZCION DE MODULO BLUETOOTH
ESP32BTSerial SerialBT;
String BTmasterName = "JyP_control";
String BTslaveName = "JyP_lights";

//Inicializacion de pin del LED de la placa para avisar de desconexion BT
const int disconnectionLedPin = 2; // Pin del LED, ajusta según tu placa

//INICIALIZACION DE COLA DE EVENTOS
eventsQueue controlEventsQueue = eventsQueue(200);

//CREACION DE MAQUINAS DE ESTADO CONTROLES. 
buttonState BR1 = buttonState(); //BOTON ROJO 1
buttonState BG1 = buttonState(); //BOTON VERDE 1
buttonState BB1 = buttonState(); //BOTON AZUL 1
buttonState BY1 = buttonState(); //BOTON AMARILLO 1
buttonState BW1 = buttonState(); //BOTON BLANCO 1
buttonState BF1 = buttonState(); //BOTON NEGRO/FANTASIA 1
buttonState BR2 = buttonState(); //BOTON ROJO 2
buttonState BG2 = buttonState(); //BOTON VERDE 2
buttonState BB2 = buttonState(); //BOTON AZUL 2
buttonState BY2 = buttonState(); //BOTON AMARILLO 2
buttonState BW2 = buttonState(); //BOTON BLANCO 2
buttonState BF2 = buttonState(); //BOTON NEGRO/FANTASIA 2
buttonState BRE = buttonState(); //BOTON ENCODER ANILLO EXTERIOR
buttonState BRI = buttonState(); //BOTON ENCODER ANILLO INTERIOR
buttonState BC = buttonState(); //BOTON ENCODER CONTROL EFECTOS
encoderState ERE = encoderState(); //ENCODER ANILLO EXTERIOR
encoderState ERI = encoderState(); //ENCODER ANILLO INTERIOR
encoderState EC = encoderState(4); //ENCODER CONTROL EFECTOS
uint8_t intensityValue = 80; 
uint8_t speedEfectValue = 65; 


//INICIALIZACION DE TIRAS LED:
ringLED ringE = ringLED(110, 32);
ringLED ringI = ringLED(80, 33);
ledStripBase letterLED_J = ledStripBase(16, 25);
ledStripBase letterLED_Y = ledStripBase(6, 26);
ledStripBase letterLED_P = ledStripBase(17, 27);

//INICIALIZACION DE VARIABLES DE EFECTOS:
uint8_t effectsEditionMode = 0;
int8_t ringE_effectIdx = 0;
int8_t ringI_effectIdx = 0;
int8_t letters_effectIdx = 0;
unsigned long lastUpdateTime = millis();

//DECLARACION DE FUNCIONES UTILIZADAS:
void updateControlEvents();
void updateControlStates();
void updateLightEffects();
void runLightEffects();
void runLightEffect_ringE();
void runLightEffect_ringI();
void runLightEffect_letters();
uint8_t getHueFromColorButtons (bool buttonsPressed[6]);

void setup() {

  Serial.begin(9600);
  Serial.println("Inicio programa JyP_lights");

  //Se inicializa el pin de conexion
  pinMode(disconnectionLedPin, OUTPUT); // Configura el pin del LED de la placacomo salida
  digitalWrite(disconnectionLedPin, LOW); // Se apaga el LED inicialmente
    
  //Se confiugra el BTslave.
  Serial.println("Inicializacion de BT");
  SerialBT.begin(BTslaveName); // Nombre del dispositivo Bluetooth

  //Se inicializan todas las tiras LED.
  ringE.initStrip(intensityValue);
  ringI.initStrip(intensityValue);
  letterLED_J.initStrip(intensityValue);
  letterLED_Y.initStrip(intensityValue);
  letterLED_P.initStrip(intensityValue);

  Serial.println("Comienzo bucle");
}

unsigned long t_start = millis();
bool BTconnected = 0;
void loop() {

  //Comprobacion de conexion con maestro BT:
  if(!SerialBT.connected()){
    if(BTconnected){
      //Se enciende el led indicador de desconexion
      digitalWrite(disconnectionLedPin, LOW); // Enciende el LED inicialmente
      Serial.println("Se ha desconectado BT de: "+BTmasterName);
      BTconnected=0;
    }

  }else{
    if(!BTconnected){
      //Se enciende el led indicador de desconexion
      digitalWrite(disconnectionLedPin, HIGH); // Enciende el LED inicialmente
      Serial.println("Se ha conectado BT a: "+BTmasterName);
      BTconnected=1;
    }
  }

  //Recopilacion de nuevos eventos BT de control:
  updateControlEvents();
  
  //Actualizacion de maquinas de estado de controles:
  updateControlStates();

  //Actualizacion de efectos a visualizar
  updateLightEffects();

  //Ejecucion de efectos seleccionados:
  runLightEffects();

  //Actualizacion de visualizacion de cada tira:
  ringE.updateDisplay();
  ringI.updateDisplay();
  letterLED_J.updateDisplay();
  letterLED_Y.updateDisplay();
  letterLED_P.updateDisplay();
}

void updateControlEvents(){
  while (SerialBT.available()){
    String controlEvent = SerialBT.readNextData();
    // Serial.println("nEv: "+controlEvent);
    controlEventsQueue.enqueueEvent(controlEvent);
  }
}

void updateControlStates(){
  //Primero se actualizan todos los controles con evento vacio para que se
  //actualice el evento anterior
  BR1.updateState();
  BG1.updateState();
  BB1.updateState();
  BY1.updateState();
  BW1.updateState();
  BF1.updateState();
  BR2.updateState();
  BG2.updateState();
  BB2.updateState();
  BY2.updateState();
  BW2.updateState();
  BF2.updateState();
  BRE.updateState();
  BRI.updateState();
  BC.updateState();
  ERE.updateState();
  ERI.updateState();
  EC.updateState();

  //Si no hay nuevos eventos de control se sale:
  if(!controlEventsQueue.eventsAvailable()) return;

  //Inicializamos los booleanos de evento registrado, ya que si hay mas 
  //de un evento de un mismo boton en la cola no queremos que un segundo evento machaque
  //el estado del primero ya que perderiamos el primer evento:
  bool prevEvent_BR1 = 0;
  bool prevEvent_BG1 = 0;
  bool prevEvent_BB1 = 0;
  bool prevEvent_BY1 = 0;
  bool prevEvent_BW1 = 0;
  bool prevEvent_BF1 = 0;
  bool prevEvent_BR2 = 0;
  bool prevEvent_BG2 = 0;
  bool prevEvent_BB2 = 0;
  bool prevEvent_BY2 = 0;
  bool prevEvent_BW2 = 0;
  bool prevEvent_BF2 = 0;
  bool prevEvent_BRE = 0;
  bool prevEvent_BRI = 0;
  bool prevEvent_BC = 0;
  bool prevEvent_ERE = 0;
  bool prevEvent_ERI = 0;
  bool prevEvent_EC = 0;

  //Si hay eventos se actualiza el estado de los controles correspondientes:
  uint16_t numEvents = controlEventsQueue.getNumEvents();
  for (int i=0; i<numEvents; i++){
    //Se obtiene nuevo evento de control:
    String newEvent = controlEventsQueue.dequeueEvent();
    // Serial.println("newEvent: "+newEvent);

    //Se divide la fuente del evento y el valor del evento:
    int splitPos = newEvent.indexOf('_');
    if(splitPos == -1) continue; 
    String eventSource = newEvent.substring(0, splitPos);
    String eventValue = newEvent.substring(splitPos+1);
    // Serial.println("eventSource: "+eventSource+"; eventValue: "+eventValue);
    
    //Se actualiza la maquina de estados de los controles:
    //*** BOTON ROJO 1
    if(eventSource=="BR1"){
      if(prevEvent_BR1) controlEventsQueue.enqueueEvent(newEvent);
      BR1.updateState(eventValue);
      prevEvent_BR1 = 1;
    //*** BOTON VERDE 1
    }else if(eventSource=="BG1"){
      if(prevEvent_BG1){
        controlEventsQueue.enqueueEvent(newEvent);
        continue;
      }
      BG1.updateState(eventValue);
      prevEvent_BG1 = 1;
    //*** BOTON AZUL 1
    }else if(eventSource=="BB1"){
      if(prevEvent_BB1){
        controlEventsQueue.enqueueEvent(newEvent);
        continue;
      }
      BB1.updateState(eventValue);
      prevEvent_BB1 = 1;
    //*** BOTON AMARILLO 1
    }else if(eventSource=="BY1"){
      if(prevEvent_BY1){
        controlEventsQueue.enqueueEvent(newEvent);
        continue;
      }
      BY1.updateState(eventValue);
      prevEvent_BY1 = 1;
    //*** BOTON BLANCO 1
    }else if(eventSource=="BW1"){
      if(prevEvent_BW1){
        controlEventsQueue.enqueueEvent(newEvent);
        continue;
      }
      BW1.updateState(eventValue);
      prevEvent_BW1 = 1;
    //*** BOTON NEGRO/FANTASIA 1
    }else if(eventSource=="BF1"){
      if(prevEvent_BF1){
        controlEventsQueue.enqueueEvent(newEvent);
        continue;
      }
      BF1.updateState(eventValue);
      prevEvent_BF1 = 1;
    //*** BOTON ROJO 2
    }else if(eventSource=="BR2"){
      if(prevEvent_BR2){
        controlEventsQueue.enqueueEvent(newEvent);
        continue;
      }
      BR2.updateState(eventValue);
      prevEvent_BR2 = 1;
    //*** BOTON VERDE 2
    }else if(eventSource=="BG2"){
      if(prevEvent_BG2){
        controlEventsQueue.enqueueEvent(newEvent);
        continue;
      }
      BG2.updateState(eventValue);
      prevEvent_BG2 = 1;
    //*** BOTON AZUL 2
    }else if(eventSource=="BB2"){
      if(prevEvent_BB2){
        controlEventsQueue.enqueueEvent(newEvent);
        continue;
      }
      BB2.updateState(eventValue);
      prevEvent_BB2 = 1;
    //*** BOTON AMARILLO 2
    }else if(eventSource=="BY2"){
      if(prevEvent_BY2){
        controlEventsQueue.enqueueEvent(newEvent);
        continue;
      }
      BY2.updateState(eventValue);
      prevEvent_BY2 = 1;
    //*** BOTON BLANCO 2
    }else if(eventSource=="BW2"){
      if(prevEvent_BW2){
        controlEventsQueue.enqueueEvent(newEvent);
        continue;
      }
      BW2.updateState(eventValue);
      prevEvent_BW2 = 1;
    //*** BOTON NEGRO/FANTASIA 2
    }else if(eventSource=="BF2"){
      if(prevEvent_BF2){
        controlEventsQueue.enqueueEvent(newEvent);
        continue;
      }
      BF2.updateState(eventValue);
      prevEvent_BF2 = 1;
    //*** BOTON ENCODER ANILLO EXTERIOR
    }else if(eventSource=="BRE"){
      if(prevEvent_BRE){
        controlEventsQueue.enqueueEvent(newEvent);
        continue;
      }
      BRE.updateState(eventValue);
      prevEvent_BRE = 1;
    //*** BOTON ENCODER ANILLO INTERIOR
    }else if(eventSource=="BRI"){
      if(prevEvent_BRI){
        controlEventsQueue.enqueueEvent(newEvent);
        continue;
      }
      BRI.updateState(eventValue);
      prevEvent_BRI = 1;
    //*** BOTON ENCODER CONTROL EFECTOS
    }else if(eventSource=="BC"){
      if(prevEvent_BC){
        controlEventsQueue.enqueueEvent(newEvent);
        continue;
      }
      BC.updateState(eventValue);
      prevEvent_BC = 1;
    //*** ENCODER ANILLO EXTERIOR
    }else if(eventSource=="ERE"){
      if(prevEvent_ERE){
        controlEventsQueue.enqueueEvent(newEvent);
        continue;
      }
      ERE.updateState(eventValue);
      prevEvent_ERE = 1;
    //*** ENCODER ANILLO INTERIOR
    }else if(eventSource=="ERI"){
      if(prevEvent_ERI){
        controlEventsQueue.enqueueEvent(newEvent);
        continue;
      }
      ERI.updateState(eventValue);
      prevEvent_ERI = 1;
    //*** ENCODER CONTROL EFECTOS
    }else if(eventSource=="EC"){
      if(prevEvent_EC){
        controlEventsQueue.enqueueEvent(newEvent);
        continue;
      }
      EC.updateState(eventValue);
      prevEvent_EC = 1;
    //*** POTENCIOMETRO INTENSIDAD
    }else if(eventSource=="P1"){
      intensityValue=uint8_t(eventValue.toInt()*255.0/100.0);

      ringE.setBrightness(intensityValue);
      ringI.setBrightness(intensityValue);
      letterLED_J.setBrightness(intensityValue);
      letterLED_Y.setBrightness(intensityValue);
      letterLED_P.setBrightness(intensityValue);

      //*** POTENCIOMETRO VELOCIDAD
    }else if(eventSource=="P2"){
      speedEfectValue=uint8_t(eventValue.toInt());
    }
  }
}

void updateLightEffects(){
  //-------------------------------------------------------
  // ACTUALIZACIÓN DE EFECTOS A VISUALIZAR
  //-------------------------------------------------------
  uint8_t prevEffectsEditionMode = effectsEditionMode;
  int8_t prev_ringE_effectIdx =ringE_effectIdx; 
  int8_t prev_ringI_effectIdx =ringI_effectIdx; 
  int8_t prev_letters_effectIdx =letters_effectIdx; 

  //Se actualiza el estado del modo edición de efectos
  switch(effectsEditionMode){   

    //Modo de no edicion
    case 0: 
      if(BC.getCurrentState()==1 && BC.getTimeInCurrentState()>1000){
        effectsEditionMode=1;
      }
      break;

    //Modo de espera a soltar boton para comenzar modo edicion de efectos:
    case 1:

      if(BC.stateChanged() && BC.getCurrentState()==0){
          effectsEditionMode=3;

          //Se igualan los efectos para que todos los led comiencen en el mismo efecto:
          ringI_effectIdx=ringE_effectIdx;
          letters_effectIdx=ringE_effectIdx;
      }
      break;

    //Modo de espera a soltar boton para finalizar modo edicion de efectos:
    case 2:

      if(BC.stateChanged() && BC.getCurrentState()==0){
          effectsEditionMode=0;
      }
      break;

    //Modo edicion de todos leds a la vez. 
    case 3:

      //Si se ha vuelto a pretar boton de control,
      //se cabia de estado:
      if(BC.stateChanged() && BC.getCurrentState()==1){
        effectsEditionMode=4;

        //Se igualan los efectos de los dos anillos para que sea el mismo efecto:
        ringI_effectIdx=ringE_effectIdx;

      //Si se lleva pulsando el boton de control de efectos mas de un 
      //segundo, se pasa a estado espera a suelta de boton
      }else if(BC.getCurrentState()==1 && BC.getTimeInCurrentState()>1000){
        effectsEditionMode=2;

      //Se comprueba si se ha modificado el encoder de control:
      }else if(EC.stateChanged()){

        int8_t increment = EC.getIncrement(true);
        ringE_effectIdx+=increment;
        ringI_effectIdx+=increment;
        letters_effectIdx+=increment;  
      }
      
      break;

    //Modo edicion de los dos anillos. 
    case 4:

      //Si se ha vuelto a pretar boton de control,
      //se cabia de estado:
      if(BC.stateChanged() && BC.getCurrentState()==1){
        effectsEditionMode=5;

      //Si se lleva pulsando el boton de control de efectos mas de un 
      //segundo, se pasa a estado de no edicion:
      }else if(BC.getCurrentState()==1 && BC.getTimeInCurrentState()>1000){
        effectsEditionMode=2;

      //Se comprueba si se ha modificado el encoder de control:
      }else if(EC.stateChanged()){

        int8_t increment = EC.getIncrement(true);
        ringE_effectIdx+=increment;
        ringI_effectIdx+=increment;
       }
      
      break;

    //Modo edicion de anillo exterior. 
    case 5:

      //Si se ha vuelto a pretar boton de control,
      //se cabia de estado:
      if(BC.stateChanged() && BC.getCurrentState()==1){
        effectsEditionMode=6;

      //Si se lleva pulsando el boton de control de efectos mas de un 
      //segundo, se pasa a estado de no edicion:
      }else if(BC.getCurrentState()==1 && BC.getTimeInCurrentState()>1000){
        effectsEditionMode=2;

      //Se comprueba si se ha modificado el encoder de control:
      }else if(EC.stateChanged()){
        int8_t increment = EC.getIncrement(true);
        ringE_effectIdx+=increment;
      }
      
      break;

    //Modo edicion de anillo interior. 
    case 6:

      //Si se ha vuelto a pretar boton de control,
      //se cabia de estado:
      if(BC.stateChanged() && BC.getCurrentState()==1){
        effectsEditionMode=7;

      //Si se lleva pulsando el boton de control de efectos mas de un 
      //segundo, se pasa a estado de no edicion:
      }else if(BC.getCurrentState()==1 && BC.getTimeInCurrentState()>1000){
        effectsEditionMode=2;

      //Se comprueba si se ha modificado el encoder de control:
      }else if(EC.stateChanged()){
        int8_t increment = EC.getIncrement(true);
        ringI_effectIdx+=increment;
      }
      
      break;

    //Modo edicion de letras. 
    case 7:

      //Si se ha vuelto a pretar boton de control,
      //se cabia de estado:
      if(BC.stateChanged() && BC.getCurrentState()==1){
        effectsEditionMode=3;

      //Si se lleva pulsando el boton de control de efectos mas de un 
      //segundo, se pasa a estado de no edicion:
      }else if(BC.getCurrentState()==1 && BC.getTimeInCurrentState()>1000){
        effectsEditionMode=2;

      //Se comprueba si se ha modificado el encoder de control:
      }else if(EC.stateChanged()){
        int8_t increment = EC.getIncrement(true);
        letters_effectIdx+=increment;
      }
      
      break;

    //En cualquier otro caso
    default:
        break;
  }

  //Se asegura que ninguno de los efectos se ha salido del rango:
  uint8_t numEffects = 11;
  if(ringE_effectIdx>(numEffects-1)) ringE_effectIdx=0; else if (ringE_effectIdx<0) ringE_effectIdx=numEffects-1;
  if(ringI_effectIdx>(numEffects-1)) ringI_effectIdx=0; else if (ringI_effectIdx<0) ringI_effectIdx=numEffects-1;
  if(letters_effectIdx>(numEffects-1)) letters_effectIdx=0; else if (letters_effectIdx<0) letters_effectIdx=numEffects-1;

  //Se saca por el log cuando ha cambiado modo edicion:
  if(prevEffectsEditionMode!=effectsEditionMode){
    //Se actualiza como estado previo el nuevo: 
    // Serial.println("nuevo effectsEditionMode: "+String(effectsEditionMode)+"prevState: "+String(prevEffectsEditionMode));
  }

  //Se sacan logs de nuevo efecto por cada led:
  if(prev_ringE_effectIdx!=ringE_effectIdx ||
     prev_ringI_effectIdx!=ringI_effectIdx ||
     prev_letters_effectIdx!=letters_effectIdx){
      // Serial.println("Nuevos efectos: RE: "+String(ringE_effectIdx)+"; RI: "+String(ringI_effectIdx)+"; LET: "+String(letters_effectIdx));
  }
  

  return;
}

void runLightEffects () {

  //Se actualizan las cosas que no necesitan retardos de actualizacion
  // - Se comprueba si hay evento del encoder de anillo exterior
  if(ringE_effectIdx==9 && ERE.stateChanged()){
    //Se actualiza el pixel actual:
    int8_t increment = ERE.getIncrement(false);
    int16_t currentPixel = ringE.getCurrentPixel();
    // Serial.println("ERE currentPixel: "+String(currentPixel)+"increment: "+String(increment));
    currentPixel += int8_t(double(increment)*2.0);
    if(currentPixel>ringE.numPixels()){
      currentPixel=0;
    }else if(currentPixel<0){
      currentPixel=ringE.numPixels()-1;
    }
    ringE.setCurrentPixel(currentPixel);
  }

  // - Se comprueba si hay evento del encoder de anillo interior
  //Se comprueba si hay evento del encoder de anillo exterior
  if(ringI_effectIdx==9 && ERI.stateChanged()){
    //Se actualiza el pixel actual:
    int8_t increment = ERI.getIncrement(false);
    int16_t currentPixel = ringI.getCurrentPixel();
    // Serial.println("ERE currentPixel: "+String(currentPixel)+"increment: "+String(increment));
    currentPixel += int8_t(double(increment)*2.0);
    if(currentPixel>ringI.numPixels()){
      currentPixel=0;
    }else if(currentPixel<0){
      currentPixel=ringI.numPixels()-1;
    }
    ringI.setCurrentPixel(currentPixel);
  }

  //Se comprueba si se ha cumplido con el tiempo de actualizacion segun la velocidad especificada:
  unsigned long maxUpdatePeriod_ms = 90;
  unsigned long updatePeriod = ulong(maxUpdatePeriod_ms*(1.0-(double(speedEfectValue)/100.0)));
  unsigned long elapsedTimeSinceUpdate = millis() - lastUpdateTime; 

  //En caso de haber cumplido con el tiempo de actualizacion, se actualiza los efectos:
  if(elapsedTimeSinceUpdate>=updatePeriod){
    //-------------------------------------------------------
    // EJECUCION DE EFECTOS A VISUALIZAR
    //-------------------------------------------------------
    //Actualizacion de valor de pixel anterior de cada tira:
    if(ringE_effectIdx!=9){
      ringE.updatePreviousPixel();
    }
    if(ringE_effectIdx!=9){
      ringI.updatePreviousPixel();
    }
    letterLED_J.updatePreviousPixel();
    letterLED_Y.updatePreviousPixel();
    letterLED_P.updatePreviousPixel();


    // ANILLO EXTERIOR
    //--------------------
    runLightEffect_ringE();

    // ANILLO INTERIOR
    //--------------------
    runLightEffect_ringI();

    // LETRAS
    //---------
    runLightEffect_letters();

    //-------------------------------------------------------
    // ACTUALIZACIÓN DE VARIABLES DE ACTUALIZACION DE EFECTO
    //-------------------------------------------------------
    lastUpdateTime=millis();
  }
}

void runLightEffect_ringE(){

  //-------------------------------------------------------
  // COMPROBACION DE BOTONES PARA MODO FLASH O CAMBIO COLOR
  //-------------------------------------------------------
  //Se comprueba si alguno de los botones de color de anillo exterior ha sido pulsado y el tiempo
  //que ha estado pulsado:
  buttonState buttonsArray [] = {BR1, BG1, BB1, BY1, BW1, BF1};
  uint8_t arrayLength = (sizeof(buttonsArray)/sizeof(buttonsArray[0]));
  bool buttonsPressed [arrayLength];
  bool anyButtonPressed = 0;
  unsigned long minTimeButtonPressed = -1;
  for(int i=0; i<arrayLength; i++){
    buttonsPressed[i] = buttonsArray[i].getCurrentState();
    
    if(buttonsPressed[i]){
      anyButtonPressed = 1;
      // Serial.println("buttonPressed "+String(i)+" pressed: "+int(buttonsPressed[i]));
      unsigned long timeButtonPressed = buttonsArray[i].getTimeInCurrentState();
      if(timeButtonPressed<minTimeButtonPressed or minTimeButtonPressed==-1){
        minTimeButtonPressed=timeButtonPressed;
      }
    } 
  }

  //Si se ha pulsado alguno de los botones de flash, enciende todas las luces del color seleccionado:
  if(anyButtonPressed){
    //Se almacena hue y sat que habia antes de pulsar:
    uint8_t prevMainHue = ringE.getMainHue();
    uint8_t prevMainSat = ringE.getMainSat();

    //Si se ha pulsado boton blanco se hace flash blanco: 
    if(buttonsPressed[4]){
      ringE.setMainSat(0);
      ringE.flashEffect(1);

    //Si se ha pulsado boton negro/fantasia se hace flash arcoiris:
    }else if(buttonsPressed[5]){
      ringE.setMainSat(255);
      ringE.rainbow(random(65535));

    //Si se pulsa cualquiera de los demas botones:
    }else{
      //Se extrae el color de los botones presionados:
      uint8_t newHue = getHueFromColorButtons(buttonsPressed);

      //Se modifica color prinicipal:
      ringE.setMainHue(newHue);
      ringE.setMainSat(255);
      ringE.flashEffect(1);
    }

    //Si la pulsacion es de menos de 4 segundos se vuelve a color anterior al flash
    //para que siga con efecto previo:
    if(minTimeButtonPressed<2500){
      ringE.setMainHue(prevMainHue);
      ringE.setMainSat(prevMainSat);
    }

    //Se sale del procedimiento directmanete.
    return; 
  }

  //Se comprueba si hay evento del encoder de anillo exterior
  //------------------------
  // VISUALIZACION DE EFECTO
  //------------------------
  switch (ringE_effectIdx){
    case 0:
      ringE.sinusoidEffect(1, 0.35, 1);
      break;
    case 1:
      ringE.sinusoidEffect(1, 0.35, 0);
      break;
    case 2:
      ringE.theaterChaseRainbowEffect(0,1,3,3);
      break;
    case 3:
      ringE.theaterChaseEffect(0,0,1,3,3);
      break;
    case 4:
      ringE.newKITTeffect(10, 1, 1);
      break;
    case 5: 
      ringE.basicKITTeffect(10, 1, 1);
      break;
    case 6:
      ringE.sparkleEffect(8);
      break;
    case 7:
      ringE.colorWipeEffect(1, 1, 10);
      break;
    case 8:
      ringE.rainbowEffect(3, 1);
      break;
    case 9:
      ringE.followCurrentPixel(30);
      break;
    case 10:
      //El encendido del flash lo hara la gestino de botones presionado, durante visualizacion efecto
      //solo oscurece:
      ringE.flashEffect(0, 80);
      break;
    default:
      break;
  }

  return;   
}

void runLightEffect_ringI(){

  //-------------------------------------------------------
  // COMPROBACION DE BOTONES PARA MODO FLASH O CAMBIO COLOR
  //-------------------------------------------------------
  //Se comprueba si alguno de los botones de color de anillo exterior ha sido pulsado y el tiempo
  //que ha estado pulsado:
  buttonState buttonsArray [] = {BR2, BG2, BB2, BY2, BW2, BF2};
  uint8_t arrayLength = (sizeof(buttonsArray)/sizeof(buttonsArray[0]));
  bool buttonsPressed [arrayLength];
  bool anyButtonPressed = 0;
  unsigned long minTimeButtonPressed = -1;
  for(int i=0; i<arrayLength; i++){
    buttonsPressed[i] = buttonsArray[i].getCurrentState();
    if(buttonsPressed[i]){
      anyButtonPressed = 1;

      unsigned long timeButtonPressed = buttonsArray[i].getTimeInCurrentState();
      if(timeButtonPressed<minTimeButtonPressed or minTimeButtonPressed==-1){
        minTimeButtonPressed=timeButtonPressed;
      }
    } 
  }

  //Si se ha pulsado alguno de los botones de flash, enciende todas las luces del color seleccionado:
  if(anyButtonPressed){
    //Se almacena hue y sat que habia antes de pulsar:
    uint8_t prevMainHue = ringI.getMainHue();
    uint8_t prevMainSat = ringI.getMainSat();

    //Si se ha pulsado boton blanco se hace flash blanco: 
    if(buttonsPressed[4]){
      ringI.setMainSat(0);
      ringI.flashEffect(1);

    //Si se ha pulsado boton negro/fantasia se hace flash arcoiris:
    }else if(buttonsPressed[5]){
      ringI.setMainSat(255);
      ringI.rainbow(random(65535));

    //Si se pulsa cualquiera de los demas botones:
    }else{
      //Se extrae el color de los botones presionados:
      uint8_t newHue = getHueFromColorButtons(buttonsPressed);

      //Se modifica color prinicipal:
      ringI.setMainHue(newHue);
      ringI.setMainSat(255);
      ringI.flashEffect(1);
    }

    //Si la pulsacion es de menos de 4 segundos se vuelve a color anterior al flash
    //para que siga con efecto previo:
    if(minTimeButtonPressed<2500){
      ringI.setMainHue(prevMainHue);
      ringI.setMainSat(prevMainSat);
    }

    //Se sale del procedimiento directmanete.
    return; 
  }

  
  //------------------------
  // VISUALIZACION DE EFECTO
  //------------------------
  switch (ringI_effectIdx){
    case 0:
      ringI.sinusoidEffect(0, 0.35, 1);
      break;
    case 1:
      ringI.sinusoidEffect(0, 0.35, 0);
      break;
    case 2:
      ringI.theaterChaseRainbowEffect(1,2,3,3);
      break;
    case 3:
      ringI.theaterChaseEffect(1,0,2,3,3);
      break;
    case 4:
      ringI.newKITTeffect(7, 0, 1);
      break;
    case 5: 
      ringI.basicKITTeffect(7, 0, 1);
      break;
    case 6:
      ringI.sparkleEffect(8);
      break;
    case 7:
      ringI.colorWipeEffect(1, 0, 10);
      break;
    case 8:
      ringI.rainbowEffect(3, 0);
      break;
    case 9:     
      ringI.followCurrentPixel(30);
      break;
    case 10:
      //El encendido del flash lo hara la gestino de botones presionado, durante visualizacion efecto
      //solo oscurece:
      ringI.flashEffect(0, 80);
      break;
    default:
      break;
  }

  return;   
}

void runLightEffect_letters(){

  //-------------------------------------------------------
  // COMPROBACION DE BOTONES PARA MODO FLASH O CAMBIO COLOR
  //-------------------------------------------------------
  //Se comprueba si alguno de los botones de color de anillo exterior ha sido pulsado y el tiempo
  //que ha estado pulsado:
  buttonState buttonsArray [] = {BR1, BG1, BB1, BY1, BW1, BF1};
  uint8_t arrayLength = (sizeof(buttonsArray)/sizeof(buttonsArray[0]));
  bool buttonsPressed [arrayLength];
  bool anyButtonPressed = 0;
  unsigned long minTimeButtonPressed = -1;
  for(int i=0; i<arrayLength; i++){
    buttonsPressed[i] = buttonsArray[i].getCurrentState();
    if(buttonsPressed[i]){
      anyButtonPressed = 1;

      unsigned long timeButtonPressed = buttonsArray[i].getTimeInCurrentState();
      if(timeButtonPressed<minTimeButtonPressed or minTimeButtonPressed==-1){
        minTimeButtonPressed=timeButtonPressed;
      }
    } 
  }

  //Si se ha pulsado alguno de los botones de flash, enciende todas las luces del color seleccionado:
  if(anyButtonPressed){
    //Se almacena hue y sat que habia antes de pulsar:
    uint8_t prevMainHue_letterJ = letterLED_J.getMainHue();
    uint8_t prevMainHue_letterY = letterLED_Y.getMainHue();
    uint8_t prevMainHue_letterP = letterLED_P.getMainHue();
    uint8_t prevMainSat_letterJ = letterLED_J.getMainSat();
    uint8_t prevMainSat_letterY = letterLED_Y.getMainSat();
    uint8_t prevMainSat_letterP = letterLED_P.getMainSat();

    //Si se ha pulsado boton blanco se hace flash blanco: 
    if(buttonsPressed[4]){
      letterLED_J.setMainSat(0);
      letterLED_Y.setMainSat(0);
      letterLED_P.setMainSat(0);
    
      letterLED_J.flashEffect(1);
      letterLED_Y.flashEffect(1);
      letterLED_P.flashEffect(1);

    //Si se ha pulsado boton negro/fantasia se hace flash arcoiris:
    }else if(buttonsPressed[5]){
      letterLED_J.setMainSat(255);
      letterLED_Y.setMainSat(255);
      letterLED_P.setMainSat(255);

      letterLED_J.rainbow(random(65535));
      letterLED_Y.rainbow(random(65535));
      letterLED_P.rainbow(random(65535));

    //Si se pulsa cualquiera de los demas botones:
    }else{
      //Se extrae el color de los botones presionados:
      uint8_t newHue = getHueFromColorButtons(buttonsPressed);

      //Se modifica color prinicipal:
      letterLED_J.setMainHue(newHue);
      letterLED_Y.setMainHue(newHue);
      letterLED_P.setMainHue(newHue);

      letterLED_J.setMainSat(255);
      letterLED_Y.setMainSat(255);
      letterLED_P.setMainSat(255);

      letterLED_J.flashEffect(1);
      letterLED_Y.flashEffect(1);
      letterLED_P.flashEffect(1);
    }

    //Si la pulsacion es de menos de 4 segundos, o no se esta en modo flash, 
    // se vuelve a color anterior al flash para que siga con efecto previo:
    if(minTimeButtonPressed<2500 or letters_effectIdx!=10){
      letterLED_J.setMainHue(prevMainHue_letterJ);
      letterLED_Y.setMainHue(prevMainHue_letterY);
      letterLED_P.setMainHue(prevMainHue_letterP);

      letterLED_J.setMainSat(prevMainSat_letterJ);
      letterLED_Y.setMainSat(prevMainSat_letterY);
      letterLED_P.setMainSat(prevMainSat_letterP);
    }

    //Se sale del procedimiento directmanete.
    return; 
  }

  //------------------------
  // VISUALIZACION DE EFECTO
  //------------------------
  switch (letters_effectIdx){
    case 0:
      //Se actualiza efecto
      letterLED_J.sinusoidEffect(1, 0.5, 1);
      letterLED_Y.sinusoidEffect(1, 0.5, 1);
      letterLED_P.sinusoidEffect(0, 0.5, 1);
      break;
    case 1:
      //Se pone a la J y la P el color del anillo exterior, 
      //y a la Y el del anillo interior:
      letterLED_J.setMainHue(ringE.getMainHue());
      letterLED_J.setMainSat(255);
      letterLED_Y.setMainHue(ringI.getMainHue());
      letterLED_Y.setMainSat(255);
      letterLED_P.setMainHue(ringE.getMainHue());
      letterLED_P.setMainSat(255);

      //Se actualiza efecto
      letterLED_J.sinusoidEffect(1, 0.5, 0);
      letterLED_Y.sinusoidEffect(1, 0.5, 0);
      letterLED_P.sinusoidEffect(0, 0.5, 0);
      break;
    case 2:
      letterLED_J.theaterChaseRainbowEffect(0,5,1,3);
      letterLED_Y.theaterChaseRainbowEffect(0,2,0,4);
      letterLED_P.theaterChaseRainbowEffect(1,5,1,3);
      break;
    case 3:
      //Se pone a la J y la P el color del anillo exterior, 
      //y a la Y el del anillo interior:
      letterLED_J.setMainHue(ringE.getMainHue());
      letterLED_J.setMainSat(255);
      letterLED_Y.setMainHue(ringI.getMainHue());
      letterLED_Y.setMainSat(255);
      letterLED_P.setMainHue(ringE.getMainHue());
      letterLED_P.setMainSat(255);

      //Se actualiza efecto
      letterLED_J.theaterChaseEffect(0,0,5,1,3);
      letterLED_Y.theaterChaseEffect(0,0,2,0,4);
      letterLED_P.theaterChaseEffect(1,0,5,1,3);
      break;
    case 4:
      //Se pone a la J y la P el color del anillo exterior, 
      //y a la Y el del anillo interior:
      letterLED_J.setMainHue(ringE.getMainHue());
      letterLED_J.setMainSat(255);
      letterLED_Y.setMainHue(ringI.getMainHue());
      letterLED_Y.setMainSat(255);
      letterLED_P.setMainHue(ringE.getMainHue());
      letterLED_P.setMainSat(255);

      //Se actualiza efecto
      letterLED_J.newKITTeffect(3, 1, 1);
      letterLED_Y.newKITTeffect(2, 1, 1);
      letterLED_P.newKITTeffect(3, 0, 1);
      break;
    case 5: 

      //Se pone a la J y la P el color del anillo exterior, 
      //y a la Y el del anillo interior:
      letterLED_J.setMainHue(ringE.getMainHue());
      letterLED_J.setMainSat(255);
      letterLED_Y.setMainHue(ringI.getMainHue());
      letterLED_Y.setMainSat(255);
      letterLED_P.setMainHue(ringE.getMainHue());
      letterLED_P.setMainSat(255);

      //Se actualiza efecto
      letterLED_J.basicKITTeffect(3, 1, 0);
      letterLED_Y.basicKITTeffect(2, 1, 0);
      letterLED_P.basicKITTeffect(3, 0, 0);
      break;
    case 6:
      //Se pone a la J y la P el color del anillo exterior, 
      //y a la Y el del anillo interior:
      letterLED_J.setMainHue(ringE.getMainHue());
      letterLED_J.setMainSat(255);
      letterLED_Y.setMainHue(ringI.getMainHue());
      letterLED_Y.setMainSat(255);
      letterLED_P.setMainHue(ringE.getMainHue());
      letterLED_P.setMainSat(255);

      //Se actualiza efecto
      letterLED_J.sparkleEffect(5);
      letterLED_Y.sparkleEffect(3);
      letterLED_P.sparkleEffect(5);
      break;
    case 7:
      letterLED_J.colorWipeEffect(1, 1, 5);
      letterLED_Y.colorWipeEffect(1, 1, 5);
      letterLED_P.colorWipeEffect(1, 0, 5);
      break;
    case 8:
    case 9: //Se repite lo mismo que caso 7 (el 8 en los anillos es el de seguir el pixel principal)
      letterLED_J.rainbowEffect(3, 1);
      letterLED_Y.rainbowEffect(3, 1);
      letterLED_P.rainbowEffect(3, 0);
      break;

    case 10:

      letterLED_J.flashEffect(0, 80);
      letterLED_Y.flashEffect(0, 80);
      letterLED_P.flashEffect(0, 80);
      break;
    default:
      break;
  }

  return;   
}

uint8_t getHueFromColorButtons (bool buttonsPressed[6]){
  //***buttonsPressed tiene que contener: {BRX, BGX, BBX, BYX, BWX, BFX}
  uint8_t newHue; 

  //Si esta pulsado el rojo:
  if(buttonsPressed[0]){
    newHue = 0; //rojo
    //Si tambien lo esta el verde:
    if(buttonsPressed[1]){
      newHue = 43; //amarillo
    //Si tambien lo esta el azul:
    }else if(buttonsPressed[2]){
      newHue = 213; //violeta
    //Si tambien lo esta el amarillo:
    }else if(buttonsPressed[3]){
      newHue = 28; //naranja
    }

  //Si esta pulsado el verde:
  }else if(buttonsPressed[1]){
    newHue = 85; //verde
    //Si tambien lo esta el azul:
    if(buttonsPressed[2]){
      newHue = 128; //Turquesa
    //Si tambien lo esta el amarillo:
    }else if(buttonsPressed[3]){
      newHue = 56; //Amarillo verdoso
    }

  //Si esta pulsado el azul:
  }else if(buttonsPressed[2]){
    newHue = 170; //azul

    //Si tambien lo esta el amarillo:
    if(buttonsPressed[3]){
      newHue = 109; //Verde azulado
    }

  //Si esta pulsado el amarillo:
  }else if(buttonsPressed[3]){
    newHue = 40; //Amarillo
  }

  return newHue;
}
