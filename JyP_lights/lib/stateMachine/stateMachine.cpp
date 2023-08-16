#include "stateMachine.h"

stateMachine::stateMachine(){
    
}

String stateMachine::getCurrentState(){
    return _currentState;
}
String stateMachine::getPrevState(){
    return _prevState;
}
void stateMachine::setCurrentState(String newState){
    _currentState=newState;
    return;
}
void stateMachine::updatePrevState(){
    _prevState=_currentState;
    return;
}
bool stateMachine::stateChanged(){
    if(_currentState==_prevState){
     return 1;
    }else{ 
     return 0;
    }
}
