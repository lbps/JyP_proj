#include "potentiometerControl.h"

potentiometerControl::potentiometerControl(eventsQueue *eventsQueue1){
    this->_eventsQueue = eventsQueue1;
};

void potentiometerControl::addNewPotentiometer(String potentiometerID, uint8_t pinAnalogInput, uint16_t maxAnalogReadValue){
    
    pinMode(pinAnalogInput, INPUT);

    Serial.println("Se ha añadido nuevo potenciometro: "+potentiometerID);
    _pinAnalogInputArray[_numPotentiometers]=pinAnalogInput;
    _maxAnalogReadValueArray[_numPotentiometers]=maxAnalogReadValue;
    _potentiometerIDsArray[_numPotentiometers]=potentiometerID;
    _potentiometerValueArray[_numPotentiometers]=255; //se inicializa con valor nulo al que no va a llegar nunca (ya que max es 100 que es en %)
    _numPotentiometers++;
    Serial.println("_numPotentiometers: "+String(_numPotentiometers));
};

void potentiometerControl::updatePotentiometers (){
    
    for (int i=0; i<(_numPotentiometers); i++){
        uint8_t pinAnalogInput = _pinAnalogInputArray[i];
        uint8_t prevPotentiometerValue = _potentiometerValueArray[i];
        String potentiometerID = _potentiometerIDsArray[i];

        //Se lee el valor del ADC y se pasa a %
        uint8_t newPotentiometerValue = uint8_t((analogRead(pinAnalogInput)/ 4095.0) * 100.0);

        String eventValue="";
        uint8_t diffValue = abs(newPotentiometerValue-prevPotentiometerValue);
        if(diffValue>0 && (newPotentiometerValue == 0 || newPotentiometerValue==100 || diffValue>5)){
            // Serial.println("prevPotentiometerValue: "+String(prevPotentiometerValues)); //DEBUG
            // Serial.println("newPotentiometerValue: "+String(newPotentiometerValue)); //DEBUG
            
            //Se manda como info de evento el valor en %
            eventValue=String(newPotentiometerValue);

            _potentiometerValueArray[i]=newPotentiometerValue;
        }
        
        if (eventValue!=""){
            String controlEvent = potentiometerID+"_"+eventValue;
            Serial.println("updatePotentiometers-> nuevo evento: "+controlEvent); //DEBUG
            _eventsQueue->enqueueEvent(controlEvent);
        };
    };

    return;
};