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
        
        String eventType="";
        if(newPosition != prevPosition){
            // Serial.println("prevPosition: "+String(prevPosition)); //DEBUG
            // Serial.println("newPosition: "+String(newPosition)); //DEBUG
            if (prevPosition<newPosition){
                eventType="CW"; //clockWise
            }else if(prevPosition>newPosition ){
                eventType="CCW"; //counterClockwise
            };
            _encoderPosArray[i]=newPosition;
        }
        
        if (eventType!=""){
            String eventJson_str = _eventsQueue->createSerializedEventJson(encoderID, eventType);
            _eventsQueue->enqueueEvent(eventJson_str);
        };
    };

    return;
};