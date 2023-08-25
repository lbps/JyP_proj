#include "encoderState.h"

encoderState::encoderState(){
    _currentPosStartTime = millis();
}

int16_t encoderState::getCurrentPosition(){
    return _currentPosition;
}
int16_t encoderState::getPrevPosition(){
    return _prevPosition;
}

int16_t encoderState::getIncrement(bool unitary){
    int16_t increment = _currentPosition-_prevPosition;

    if(unitary){
        if(increment>0){
            increment = 1;
        }else if(increment<0){
            increment = -1;
        } 
    }
    return increment;
}

void encoderState::updateState(String eventValue){
    //Se actualiza el estado anterior:
    _prevPosition=_currentPosition;

    //Si no hay ningun evento nuevo de control se sale:
    if(eventValue.length()==0) return;

    //Se actualiza el estado:
    if(eventValue=="CW"){
        _currentPosition++;
    }else if(eventValue=="CCW"){
        _currentPosition--;
    }

    //Se actualiza el tiempo de inicio de estado actual:
    if(stateChanged()){
        _currentPosStartTime = millis();
    }
    return;
}

bool encoderState::stateChanged(){
    if(_currentPosition==_prevPosition){
     return 1;
    }else{ 
     return 0;
    }
}

unsigned long encoderState::getTimeInCurrentState(){
    //Se devuelve el tiempo en milisegundos que ha pasado desde inicio de ultimo estado:
    return millis()-_currentPosStartTime;
}