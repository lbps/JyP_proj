#include "ledStripBase.h"

ledStripBase::ledStripBase (uint16_t numLeds, uint8_t dataPin):
                            Adafruit_NeoPixel(numLeds, dataPin, NEO_GRB + NEO_KHZ800){
    _numLeds = numLeds; 
    _dataPin = dataPin;
}

ledStripBase::~ledStripBase(){
}

uint16_t ledStripBase::map8to16bit(uint8_t in){
  return map(in, 0, 255, 0, 65535);
};

uint8_t ledStripBase::map16to8bit(uint16_t in){
  return map(in, 0, 65535, 0, 255);
};

void ledStripBase::initStrip(uint8_t initialBrightness){
  if(initialBrightness<0) initialBrightness=0;
  if(initialBrightness>100) initialBrightness=100;
  begin();
  setBrightness(initialBrightness);
  show();

  return;
}

void ledStripBase::updatePreviousPixel(){
  _prevPixel=_currentPixel;
  return;
}

void ledStripBase::updateDisplay(){
  //Si esta listo para visualizar de nuevo se visualiza cambios:
  if(canShow()) show();
}
 

void ledStripBase::rgbToHsv(uint32_t color, uint16_t& hue, uint8_t& saturation, uint8_t& value) {

  // Obtener los componentes RGB individuales
  uint8_t red = (color >> 16) & 0xFF;
  uint8_t green = (color >> 8) & 0xFF;
  uint8_t blue = color & 0xFF;

  // Convertir a HSV
  float r = red / 255.0f;
  float g = green / 255.0f;
  float b = blue / 255.0f;

  float cmax = fmaxf(r, fmaxf(g, b));
  float cmin = fminf(r, fminf(g, b));
  float delta = cmax - cmin;

  // Calculamos el valor (value)
  value = static_cast<uint8_t>(cmax * 255.0f);

  // Si el valor es cero, la saturación y el matiz son 0
  if (cmax == 0) {
    saturation = 0;
    hue = 0;
  } else {
    // Calculamos la saturación (saturation)
    saturation = static_cast<uint8_t>((delta / cmax) * 255.0f);

    // Calculamos el matiz (hue)
    if (delta == 0) {
      hue = 0;
    } else if (cmax == r) {
      hue = static_cast<uint16_t>((60.0f * fmodf((g - b) / delta, 6.0f)) * 65535.0f / 360.0f);
    } else if (cmax == g) {
      hue = static_cast<uint16_t>((60.0f * ((b - r) / delta + 2.0f)) * 65535.0f / 360.0f);
    } else {
      hue = static_cast<uint16_t>((60.0f * ((r - g) / delta + 4.0f)) * 65535.0f / 360.0f);
    }
  }
}

uint16_t ledStripBase::mapPixelIdxToRingStrip(int16_t pixelIdxToMap){
  uint16_t newPixelIdx;
  if(pixelIdxToMap>=numPixels()){
    newPixelIdx= pixelIdxToMap-numPixels();
  }else if(pixelIdxToMap<0){
    newPixelIdx=numPixels()+pixelIdxToMap;
  }else{
    newPixelIdx=pixelIdxToMap;
  }
  return newPixelIdx;
}

uint8_t ledStripBase::getMainHue(){
  return _mainHue;
}

uint8_t ledStripBase::getMainSat(){
  return _mainSat;
}

uint16_t ledStripBase::getCurrentPixel(){
  return _currentPixel;
}

uint16_t ledStripBase::getPrevPixel(){
  return _prevPixel;
}

uint16_t ledStripBase::getSequenceIdx(){
  return _sequenceIdx;
};

bool ledStripBase::firstEffectSequence(){
  if(_firstEffectSequence){
    _firstEffectSequence=0;
    return 1;
  }else{
    return 0;
  }
}

void ledStripBase::setMainHue (uint8_t mainHue){
  _mainHue = mainHue;
  return;
}

void ledStripBase::setMainSat (uint8_t mainSat){
  _mainSat = mainSat;
  return;
}

void ledStripBase::setCurrentPixel(uint16_t newCurrentPixel){
  _currentPixel = newCurrentPixel;
  return;
}

void ledStripBase::setSequenceIdx(uint16_t newSequenceIdx){
  _sequenceIdx = newSequenceIdx;
  return;
}

void ledStripBase::rainbowEffect(uint8_t colorStep) {
  rainbow(map8to16bit(_mainHue));
  if(colorStep<1) colorStep=1;
  if(colorStep>10) colorStep=10;
  _mainHue+=colorStep;
  _mainSat=255;
  return;
}

void ledStripBase::colorWipeEffect(bool randomColor, bool forwardDirection, uint8_t colorStep){

  //Se comprueba si hay que actualizar color:
  if(_currentPixel>=(numPixels()-1)){
    if(randomColor){
      _mainHue=random(256);
      _mainSat=255;
    }else{ 
      _mainHue+=colorStep;
    }
  }

  //se actualiza valor del pixel actual:
  setPixelColor(_currentPixel, ColorHSV(map8to16bit(_mainHue), _mainSat));

  //Se actualiza el pixel actual:
  if(forwardDirection){
    if(_currentPixel<(numPixels()-1)){
      _currentPixel++;
    }else{
      _currentPixel=0;
    };
  }else{
    if(_currentPixel>0){
      _currentPixel--;
    }else{
      _currentPixel=numPixels()-1;
    };
  }
  return;
}

void ledStripBase::theaterChaseEffect(bool forwardDir, bool autoColorChange, uint16_t numSequencesToChangeColor, uint16_t spaceBetweenLeds, uint16_t numAdjacentLedsOn){

  //Se apagan todos los led.
  clear();

  //Se pinta 1 un led de cada tres de la tira
  if(forwardDir){
    for (int i=(0+(_sequenceIdx%spaceBetweenLeds)); i<numPixels(); i+=spaceBetweenLeds){
      for (int j=0; j<numAdjacentLedsOn; j++){
        if((i+j)>=numPixels()) break;
        setPixelColor(i+j, ColorHSV(map8to16bit(_mainHue), _mainSat));
      }
    }
  }else{
    for (int i=((numPixels()-1-(_sequenceIdx%spaceBetweenLeds))); i>=0; i-=spaceBetweenLeds){
      setPixelColor(i, ColorHSV(map8to16bit(_mainHue), _mainSat));
      for (int j=0; j<numAdjacentLedsOn; j++){
        if((i-j)<0) break;
        setPixelColor(i-j, ColorHSV(map8to16bit(_mainHue), _mainSat));
      }
    }
  }
  
  //Se actualiza el numero de secuencia
  _sequenceIdx++;

  //Se comprueba si hay que cambiar color automaticamente:
  if(autoColorChange && _sequenceIdx>=numSequencesToChangeColor && _sequenceIdx%spaceBetweenLeds==0){ 
    _sequenceIdx = 0;

    _mainHue = random(255);
    _mainSat = 255;
  };
  
  return;
}

void ledStripBase::theaterChaseRainbowEffect(bool forwardDir, uint16_t numSequencesToChangeColor, uint16_t spaceBetweenLeds, uint16_t numAdjacentLedsOn) {
  uint8_t deltaHue = 256 / numPixels();

  // Cambiar color si toca, pero SIN reiniciar _sequenceIdx
  if ((_sequenceIdx % numSequencesToChangeColor) == 0) {
    _mainHue = (_mainHue + deltaHue) % 256;
    _mainSat = 255;
  }

  // Limpiar la tira
  for (int i = 0; i < numPixels(); i++) {
    setPixelColor(i, 0);  // Apagado
  }

  int patternLength = numAdjacentLedsOn + spaceBetweenLeds; // Longitud total de patrón

  for (int base = 0; base < numPixels(); base++) {
    int shiftedPos;

    if (forwardDir) {
      shiftedPos = (base + (_sequenceIdx % patternLength)) % patternLength;
    } else {
      shiftedPos = (base - (_sequenceIdx % patternLength) + patternLength) % patternLength;
    }

    if (shiftedPos < numAdjacentLedsOn) {
      uint32_t pixelHue = _mainHue + base * deltaHue;
      pixelHue = pixelHue % 256;
      setPixelColor(base, ColorHSV(map8to16bit(pixelHue), _mainSat));
    }
  }

  _sequenceIdx++;
}

void ledStripBase::sparkleEffect(uint16_t numLedsOn){
  clear();

  uint32_t color = ColorHSV(map8to16bit(_mainHue), _mainSat);
  uint16_t pixel;

  for(uint16_t i = 0; i<numLedsOn; i++){
    pixel = random(numPixels());
    setPixelColor(pixel, color);
  };
  return;
}

void ledStripBase::runningLightsEffect(bool forwardDirection){
  uint32_t mainColor = ColorHSV(map8to16bit(_mainHue), _mainSat);
  int R = (mainColor >> 16) & 0xFF;
  int G = (mainColor >> 8) & 0xFF;
  int B = mainColor & 0xFF;
  float levelScale;
  for(int i=0; i<numPixels(); i++) {
    levelScale = (sin(i+_sequenceIdx) * 127.0 + 128.0)/255.0;
    setPixelColor(i , uint8_t(R*levelScale), uint8_t(G*levelScale), uint8_t(B*levelScale));
  }
  
  if(forwardDirection){
    _sequenceIdx--;

    if(_sequenceIdx<0){
      _sequenceIdx=numPixels();
    }
  }else{
    _sequenceIdx++;

    if(_sequenceIdx>(numPixels()*2)){
      _sequenceIdx=0;
    }
  }

  return;
}

void ledStripBase::fadeDarkPixel(uint16_t pixelIdx, uint8_t fadeStep){

  //Se lee el valor anterior del pixel:
  uint32_t prevPixelValue = getPixelColor(pixelIdx);

  //Se obtienen los valores HSV:
  uint16_t H;
  uint8_t S;
  uint8_t V;
  rgbToHsv(prevPixelValue, H, S, V);

  //Se reduce el canal V:
  if(V>(fadeStep)){
    V-=fadeStep;
  }else{
    V = 0;
  }
  uint32_t newPixelValue = ColorHSV(H,S,V);

  //se actualizan los valores de cada canal:
  setPixelColor(pixelIdx, newPixelValue);


    // OTRA OPCION ACTUANDO DIRECTAMENTE SOBRE CANALES RGB (mas rapido pero cambia de color al tender a 0)
    // // Obtener los componentes RGB individuales
    // int red = (prevPixelValue >> 16) & 0xFF;
    // int green = (prevPixelValue >> 8) & 0xFF;
    // int blue = prevPixelValue & 0xFF;
    
    // //Se reducen sus valores:
    // if(red>(fadeStep)){
    //   red-=fadeStep;
    // }else{
    //   red = 0;
    // }
    // if(green>(fadeStep)){
    //   green-=fadeStep;
    // }else{
    //   green = 0;
    // }
    // if(blue>(fadeStep)){
    //   blue-=fadeStep;
    // }else{
    //   blue = 0;
    // }
    
    // //se actualizan los valores de cada canal:
    // setPixelColor(pixelIdx, Color(uint8_t(red), uint8_t(green), uint8_t(blue)));

}

void ledStripBase::fadeDarkAll(uint8_t fadeStep){
  for(int i=0;i<(numPixels()); i++){
    fadeDarkPixel(i, fadeStep);
  }
}

void ledStripBase::flashEffect(bool newFlash, uint8_t fadeSpeed){

  uint8_t fadeStep = (fadeSpeed/100.0)*255;
  if(newFlash){
    fill(ColorHSV(map8to16bit(_mainHue), _mainSat));
  }else{
    fadeDarkAll(fadeStep);
  }
  return;
}

void ledStripBase::basicKITTeffect(uint16_t lightTrailSize, bool initialForwardDirection, bool meteorTrailEffect){

  //Se inicializa el effecto:
  if(_firstEffectSequence){
    _sequenceIdx=0;
    if(initialForwardDirection){
      _currentPixel=0;
    }else{
      _currentPixel=numPixels()-1;
    }
    _firstEffectSequence=0;
  }

  //Se oscurecen todos los pixels para crear estela
  uint8_t fadeStep=255/lightTrailSize;
  if(!meteorTrailEffect){
    fadeDarkAll(fadeStep);
  }else{
    for(int i=0;i<(numPixels()); i++){
      if(random(10)>3){
        fadeDarkPixel(i, uint8_t(fadeStep/1.5));
      }
    }
  }
  
  //Secuencias pares se va pasando el pixel principal adelante hasta llegar al final y pasar de secuencia:
  if(_sequenceIdx%2==0){
    if(_currentPixel<numPixels()){
      _currentPixel++;
    }else{
      _sequenceIdx++;
    };

  //Secuencias impares se va pasando el pixel principal atras hasta llegar al final y pasar de secuencia:
  }else{
    if(_currentPixel>0){
      _currentPixel--;
    }else{
      _sequenceIdx++;
    };
  }

  //Se enciende a maxima intensidad el pixel principal:
  setPixelColor(_currentPixel, ColorHSV(map8to16bit(_mainHue), _mainSat));

}

void ledStripBase::newKITTeffect(uint16_t lightTrailSize, bool initialForwardDirection, bool meteorTrailEffect){

  //Se inicializa el effecto:
  if(_firstEffectSequence){
    _sequenceIdx=0;
    if(initialForwardDirection){
      _currentPixel=0;
    }else{
      _currentPixel=uint8_t((numPixels()-1)/2);
    }
    _firstEffectSequence=0;
  }

  //Se oscurecen todos los pixels para crear estela
  uint8_t fadeStep=255/lightTrailSize;
  if(!meteorTrailEffect){
    fadeDarkAll(fadeStep);
  }else{
    for(int i=0;i<(numPixels()); i++){
      if(random(10)>3){
        fadeDarkPixel(i, uint8_t(fadeStep/1.5));
      }
    }
  }

  //Secuencias pares se va pasando el pixel principal adelante hasta llegar al final y pasar de secuencia:
  if(_sequenceIdx%2==0){
    if(_currentPixel<numPixels()){
      _currentPixel++;
    }else{
      _sequenceIdx++;
    };

  //Secuencias impares se va pasando el pixel principal atras hasta llegar al final y pasar de secuencia:
  }else{
    if(_currentPixel>0){
      _currentPixel--;
    }else{
      _sequenceIdx++;
    };
  }

  //Se enciende a maxima intensidad el pixel principal y el espejo:
  uint16_t mirrorPixel=(numPixels()-1)-_currentPixel;
  setPixelColor(_currentPixel, ColorHSV(map8to16bit(_mainHue), _mainSat));
  setPixelColor(mirrorPixel, ColorHSV(map8to16bit(_mainHue), _mainSat));
}