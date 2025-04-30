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
    // Serial.println("addNewButton-> Se ha añadido nuevo boton"); //DEBUG
    _buttonArray[_numButtons]=newButton;
    _buttonIDsArray[_numButtons]=buttonID;
    _numButtons++;
    // Serial.println("addNewButton-> _numButtons: "+String(_numButtons)); //DEBUG
};

void buttonControl::updateButtons (){

    for (int i=0; i<(_numButtons); i++){
        Bounce2::Button *button_i = &_buttonArray[i];
        String buttonID = _buttonIDsArray[i];
        button_i->update();
        
        String eventValue="";
        if (button_i->pressed() ){
            eventValue="DN";
        }else if(button_i->released()){
            eventValue="UP";
        };

        if (eventValue!=""){
            String controlEvent = buttonID+"_"+eventValue;
            // Serial.println("updateButtons-> nuevo evento: "+controlEvent); //DEBUG
            _eventsQueue->enqueueEvent(controlEvent);
        };
    };

    return;
};