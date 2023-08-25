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
    //Se actualiza el estado anterior:
    _prevState=_currentState;

    //Si no hay ningun evento nuevo de control se sale:
    if(eventValue.length()==0) return;

    //Se actualiza el estado:
    if(eventValue=="UP"){
        _currentState=1;
    }else if(eventValue=="DN"){
        _currentState=0;
    }

    //Se actualiza el tiempo de inicio de estado actual:
    if(stateChanged()){
        _currentStateStartTime = millis();
    }
    return;
}

bool buttonState::stateChanged(){
    if(_currentState==_prevState){
     return 1;
    }else{ 
     return 0;
    }
}

unsigned long buttonState::getTimeInCurrentState(){
    //Se devuelve el tiempo en milisegundos que ha pasado desde inicio de ultimo estado:
    return millis()-_currentStateStartTime;
}