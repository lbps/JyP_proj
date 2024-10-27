#include "buttonState.h"

buttonState::buttonState(){
    _currentStateStartTime = millis();
}

bool buttonState::getCurrentState(){
    return _currentState;
}
bool buttonState::getPrevState(){
    return _prevState;
}

void buttonState::updateState(String eventValue){

    //Se actualiza el tiempo en estado actual:
    _currentStateTime= millis()-_currentStateStartTime;

    //Se actualiza el estado anterior:
    _prevState=_currentState;

    //Si no hay ningun evento nuevo de control se sale:
    if(eventValue.length()==0) return;

    //Se actualiza el estado:
    if(eventValue=="UP"){
        _currentState=0;
    }else if(eventValue=="DN"){
        _currentState=1; //estado es 1 cunado esta presionado el boton
    }

    //En caso de que se acabe de cambiar de estado:
    if(stateChanged()){
        //Se actualiza info de estado previo: 
        _prevStateTime=_currentStateTime; 

        //se actualiza info de estado actual:
        _currentStateStartTime = millis();
        _currentStateTime=0;
    }
    return;
}

bool buttonState::stateChanged(){
    if(_currentState==_prevState){
     return 0;
    }else{ 
     return 1;
    }
}

unsigned long buttonState::getTimeInCurrentState(){
    //Se devuelve el tiempo en milisegundos que ha pasado desde inicio de ultimo estado:
    return _currentStateTime;
}

unsigned long buttonState::getTimeInPreviousState(){
    //Se devuelve el tiempo en milisegundos que ha pasado desde inicio de ultimo estado:
    return _prevStateTime;
}