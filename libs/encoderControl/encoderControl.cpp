#include "encoderControl.h"

encoderControl::encoderControl(eventsQueue *eventsQueue1){
    this->_eventsQueue = eventsQueue1;
};

void encoderControl::addNewEncoder(String encoderID, uint8_t pinCHA, uint8_t pinCHB){

    Encoder *newEncoder = new Encoder(pinCHA, pinCHB);

    Serial.println("Se ha añadido nuevo encoder");
    _encoderArray[_numEncoders]=newEncoder;
    _encoderPosArray[_numEncoders]=0;
    _encoderIDsArray[_numEncoders]=encoderID;
    _numEncoders++;
    Serial.println("_numEncoders: "+String(_numEncoders));
};

void encoderControl::updateEncoders (){
    
    for (int i=0; i<(_numEncoders); i++){
        Encoder* encoder_i = _encoderArray[i];
        int32_t prevPosition = _encoderPosArray[i];
        String encoderID = _encoderIDsArray[i];

        int32_t newPosition = encoder_i->read();
        
        String eventValue="";
        if(newPosition != prevPosition){
            // Serial.println("prevPosition: "+String(prevPosition)); //DEBUG
            // Serial.println("newPosition: "+String(newPosition)); //DEBUG
            if (prevPosition<newPosition){
                eventValue="CW"; //clockWise
            }else if(prevPosition>newPosition ){
                eventValue="CCW"; //counterClockwise
            };
            _encoderPosArray[i]=newPosition;
        }
        
        if (eventValue!=""){
            String controlEvent = encoderID+"_"+eventValue;
            Serial.println("updateEncoders-> nuevo evento: "+controlEvent); //DEBUG
            _eventsQueue->enqueueEvent(controlEvent);
        };
    };

    return;
};