#include "ringLED.h"

ringLED::ringLED (uint8_t numLeds, uint8_t dataPin)
                : ledStripBase(numLeds, dataPin){
}

ringLED::~ringLED(){
}

void ringLED::followCurrentPixel(uint16_t lightTrailSize){

  bool firstSequence = firstEffectSequence();
  if(firstSequence){
    clear();
    setCurrentPixel(0);   
  } else {
    // Oscurecer todos los píxeles gradualmente
    if(lightTrailSize > 0){
      uint8_t fadeStep = 255 / lightTrailSize;
      if(fadeStep <= 0) fadeStep = 1;
      fadeDarkAll(fadeStep);
    } else {
      clear();
    } 
  }

  // Aplicar efecto con estela de brillo decreciente
  if(getCurrentPixel() != getPrevPixel() || firstSequence){
    int from = getPrevPixel();
    int to = getCurrentPixel();
    int num = numPixels();

    // Calcular la distancia recorrida y dirección (siempre positiva)
    int distanceCW = (to - from + num) % num;
    int distanceCCW = (from - to + num) % num;

    int step = (distanceCW <= distanceCCW) ? 1 : -1;
    int distance = min(distanceCW, distanceCCW);

    // Siempre pintamos todos los píxeles entre prev y current
    for(int i = 0; i <= distance; i++) {
        int idx = (from + i * step + num) % num;

        // Brillo proporcional: el más reciente más brillante
        uint8_t brightness = map(i, 0, distance, 255, 30); // puedes ajustar el valor mínimo
        setPixelColor(idx, ColorHSV(map8to16bit(getMainHue()), getMainSat(), brightness));
    }

    updatePreviousPixel();
  }
}