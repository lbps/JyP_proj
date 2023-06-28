#include "buttonControl.h"

#include <Bounce2.h>

buttonControl::buttonControl(eventsQueue *eventsQueue1){
    this->_eventsQueue = eventsQueue1;
};

void buttonControl::addNewButton(String buttonID, uint8_t pin, uint8_t pinMode, uint16_t debounceInterval_ms){

    Bounce2::Button newButton = Bounce2::Button();
    newButton.attach( pin, pinMode ); 
    newButton.interval(debounceInterval_ms); 

    if(pinMode==INPUT_PULLUP){
        newButton.setPressedState(LOW); 
    }else{
        newButton.setPressedState(HIGH); 
    };
    Serial.println("Se ha añadido nuevo boton");
    _buttonArray[_numButtons]=newButton;
    _buttonIDsArray[_numButtons]=buttonID;
    _numButtons++;
    Serial.println("_numButtons: "+String(_numButtons));
};

void buttonControl::updateButtons (){

    for (int i=0; i<(_numButtons); i++){
        Bounce2::Button *button_i = &_buttonArray[i];
        String buttonID = _buttonIDsArray[i];
        button_i->update();
        
        String eventType="";
        if (button_i->pressed() ){
            eventType="PR";
        }else if(button_i->released()){
            eventType="RL";
        };

        if (eventType!=""){
            String eventJson_str = _eventsQueue->createSerializedEventJson(buttonID, eventType);
            _eventsQueue->enqueueEvent(eventJson_str);
        };
    };

    return;
};