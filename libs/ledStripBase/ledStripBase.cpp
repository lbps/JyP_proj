#include "ledStripBase.h"

ledStripBase::ledStripBase (uint16_t numLeds, uint8_t dataPin):
                            Adafruit_NeoPixel(numLeds, dataPin, NEO_GRB + NEO_KHZ800){
    _numLeds = numLeds; 
    _dataPin = dataPin;
}

ledStripBase::~ledStripBase(){
}

void ledStripBase::initStrip(uint8_t initialBrightness = 50){
  if(initialBrightness<0) initialBrightness=0;
  if(initialBrightness>100) initialBrightness=100;
  this->begin();
  this->setBrightness(initialBrightness);
  this->show();

  return;
}

uint8_t ledStripBase::getMainHue(){
  return _mainHue;
}

 void ledStripBase::setMainHue (uint8_t mainHue){
  _mainHue = mainHue;
  return;
}

void ledStripBase::rainbowEffect(uint8_t colorStep = 1) {
  this->rainbow(map(_mainHue, 0, 255, 0, 65535));
  if(colorStep<1) colorStep=1;
  if(colorStep>10) colorStep=10;
  _mainHue+=colorStep;
  return;
}

void ledStripBase::colorWipeEffect(bool randomColor = 1, bool forwardDirection = 1, uint8_t colorStep = 1){

  //Se comprueba si hay que actualizar color:
  if(_currentPixel>=(this->numPixels()-1)){
    if(randomColor){
      _mainHue=random(256);
    }else{ 
      _mainHue+=colorStep;
    }
  }

  //se actualiza valor del pixel actual:
  this->setPixelColor(_currentPixel, this->ColorHSV(map(_mainHue, 0, 255, 0, 65535)));

  //Se actualiza el pixel actual:
  if(forwardDirection){
    if(_currentPixel<(this->numPixels()-1)){
      _currentPixel++;
    }else{
      _currentPixel=0;
    };
  }else{
    if(_currentPixel>0){
      _currentPixel--;
    }else{
      _currentPixel=this->numPixels()-1;
    };
  }
  return;
}

void ledStripBase::theaterChaseEffect(bool forwardDir = 1, bool autoColorChange = 0, uint16_t numSequencesToChangeColor = 10, uint16_t spaceBetweenLeds = 3, uint16_t numAdjacentLedsOn = 1){

  //Se apagan todos los led.
  this->clear();

  //Se pinta 1 un led de cada tres de la tira
  if(forwardDir){
    for (int i=(0+(_sequenceIdx%spaceBetweenLeds)); i<this->numPixels(); i+=spaceBetweenLeds){
      for (int j=0; j<numAdjacentLedsOn; j++){
        if((i+j)>=numPixels()) break;
        this->setPixelColor(i+j, this->ColorHSV(map(_mainHue,0, 255, 0, 65535)));
      }
    }
  }else{
    for (int i=((this->numPixels()-1-(_sequenceIdx%spaceBetweenLeds))); i>=0; i-=spaceBetweenLeds){
      this->setPixelColor(i, this->ColorHSV(map(_mainHue,0, 255, 0, 65535)));
      for (int j=0; j<numAdjacentLedsOn; j++){
        if((i-j)<0) break;
        this->setPixelColor(i-j, this->ColorHSV(map(_mainHue,0, 255, 0, 65535)));
      }
    }
  }
  

  //Se comprueba si hay que cambiar color automaticamente:
  _sequenceIdx++;

  if(autoColorChange && _sequenceIdx>=numSequencesToChangeColor && _sequenceIdx%spaceBetweenLeds==0){ 
    _sequenceIdx = 0;

    _mainHue = random(255);
  };
  
  
}
