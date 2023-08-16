#include "ringLED.h"

ringLED::ringLED (uint8_t numLeds, uint8_t dataPin)
                : ledStripBase(numLeds, dataPin){
}

ringLED::~ringLED(){
}

void ringLED::followCurrentPixel(uint16_t lightTrailSize){

  //si es primera iteraccion se reinicia todos los pixels
  bool firstSequence = firstEffectSequence();
  if(firstSequence){
    clear();
    setCurrentPixel(0);   
  }else{
    //Se oscurecen todos pixels:
    if(lightTrailSize>0){
      uint8_t fadeStep=255/lightTrailSize;
      if(fadeStep<=0) fadeStep=1;
      fadeDarkAll(fadeStep);
    }else{
      clear();
    } 
  }

  //Si el pixel actual es distinto del anterior, se ilumina dicho pixel:
  if((getCurrentPixel() != getPrevPixel()) or firstSequence){
    setPixelColor(getCurrentPixel(), ColorHSV(map8to16bit(getMainHue())));
  }
}