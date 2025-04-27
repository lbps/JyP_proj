#include "encoderState.h"

encoderState::encoderState(uint8_t eventsFilter_groupSize){
    _currentPosStartTime = millis();
    _eventsFilter_groupSize = eventsFilter_groupSize;
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
    unsigned long timeSinceLastEvent =millis() - _currentPosStartTime; 
    if(eventValue.length()==0) return;

    //Se filtran eventos agrupando varios:
    if(_eventsFilter_groupSize>1){
        if(eventValue=="CW"){
            _eventsFilter_sum++;
            
        }else if(eventValue=="CCW"){
            _eventsFilter_sum--;
        }
        _eventsFilter_cnt++;
        if(_eventsFilter_cnt>=_eventsFilter_groupSize){
            //Se actualiza eventValue segun el tipo de evento mas votado:
            if(_eventsFilter_sum>0){
                eventValue="CW";
            }else if(_eventsFilter_sum<0){
                eventValue="CCW";
            }else{
                //Se indica evento vacio para que no actualice posicion actual:
                eventValue="";
            }

            //Se reinician variables de filtro:
            _eventsFilter_cnt=0;
            _eventsFilter_sum=0;
        }else{
            //Se indica evento vacio para que no actualice posicion actual:
            eventValue="";
        }
    }

    //Se actualiza el estado:
    if(eventValue=="CW"){
        _currentPosition++;
    }else if(eventValue=="CCW"){
        _currentPosition--;
    }
    
    //Se actualiza el tiempo de inicio de estado actual:
    if(stateChanged()){
        // Serial.println("newCurrentPosition: "+String(_currentPosition));
        _currentPosStartTime = millis();
    }
    return;
}

bool encoderState::stateChanged(){
    if(_currentPosition==_prevPosition){
     return 0;
    }else{ 
     return 1;
    }
}

unsigned long encoderState::getTimeInCurrentState(){
    //Se devuelve el tiempo en milisegundos que ha pasado desde inicio de ultimo estado:
    return millis()-_currentPosStartTime;
}