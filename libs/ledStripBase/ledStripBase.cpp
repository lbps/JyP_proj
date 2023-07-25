#include "ledStripBase.h"

ledStripBase::ledStripBase (uint16_t numLeds, uint8_t dataPin):
                            Adafruit_NeoPixel(numLeds, dataPin, NEO_GRB + NEO_KHZ800){
    _numLeds = numLeds; 
    _dataPin = dataPin;
}

ledStripBase::~ledStripBase(){
}

void ledStripBase::initStrip(uint8_t initialBrightness){
  if(initialBrightness<0) initialBrightness=0;
  if(initialBrightness>100) initialBrightness=100;
  this->begin();
  this->setBrightness(initialBrightness);
  this->show();

  return;
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

uint8_t ledStripBase::getMainHue(){
  return _mainHue;
}

 void ledStripBase::setMainHue (uint8_t mainHue){
  _mainHue = mainHue;
  return;
}

void ledStripBase::rainbowEffect(uint8_t colorStep) {
  this->rainbow(map(_mainHue, 0, 255, 0, 65535));
  if(colorStep<1) colorStep=1;
  if(colorStep>10) colorStep=10;
  _mainHue+=colorStep;
  return;
}

void ledStripBase::colorWipeEffect(bool randomColor, bool forwardDirection, uint8_t colorStep){

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

void ledStripBase::theaterChaseEffect(bool forwardDir, bool autoColorChange, uint16_t numSequencesToChangeColor, uint16_t spaceBetweenLeds, uint16_t numAdjacentLedsOn){

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
  
  //Se actualiza el numero de secuencia
  _sequenceIdx++;

  //Se comprueba si hay que cambiar color automaticamente:
  if(autoColorChange && _sequenceIdx>=numSequencesToChangeColor && _sequenceIdx%spaceBetweenLeds==0){ 
    _sequenceIdx = 0;

    _mainHue = random(255);
  };
  
  return;
}

void ledStripBase::theaterChaseRainbowEffect(bool forwardDir, uint16_t numSequencesToChangeColor, uint16_t spaceBetweenLeds, uint16_t numAdjacentLedsOn){
  //Se apagan todos los led.
  this->clear();

  uint8_t deltaHue = 256 / this->numPixels();
  
  //Se pinta 1 un led de cada tres de la tira
  if(forwardDir){
    for (int i=(0+(_sequenceIdx%spaceBetweenLeds)); i<this->numPixels(); i+=spaceBetweenLeds){

      //Se comprueba si hay que cambiar color:
      if(_sequenceIdx>=numSequencesToChangeColor && _sequenceIdx%spaceBetweenLeds==0){ 
        _sequenceIdx = 0;

        _mainHue=_mainHue+deltaHue;
        if(_mainHue>255) _mainHue=0+(_mainHue-256);
      };

      //Se encienden todos los leds conjuntos necesarios: 
      for (int j=0; j<numAdjacentLedsOn; j++){
        if((i+j)>=numPixels()) break;

        uint32_t pixelHue = _mainHue + (i+j)*deltaHue; 
        if(pixelHue>255) pixelHue=256-pixelHue;

        this->setPixelColor(i+j, this->ColorHSV(map(pixelHue,0, 256, 0, 65535)));
      }
    }
  }else{
    for (int i=((this->numPixels()-1-(_sequenceIdx%spaceBetweenLeds))); i>=0; i-=spaceBetweenLeds){

      //Se comprueba si hay que cambiar color:
      if(_sequenceIdx>=numSequencesToChangeColor && _sequenceIdx%spaceBetweenLeds==0){ 
        _sequenceIdx = 0;

        _mainHue=_mainHue-(deltaHue*spaceBetweenLeds);
        if(_mainHue<0) _mainHue=256+_mainHue;
      };

      //Se encienden todos los leds conjuntos necesarios: 
      for (int j=0; j<numAdjacentLedsOn; j++){
        if((i-j)<0) break;

        uint32_t pixelHue = _mainHue + (i+j)*deltaHue; 
        if(pixelHue>255) pixelHue=256-pixelHue;
        
        this->setPixelColor(i-j, this->ColorHSV(map(pixelHue,0, 256, 0, 65535)));
      }
    }
  }

  //Se actualiza el numero de secuencia
  _sequenceIdx++;
}

void ledStripBase::fadeDarkAll(uint8_t fadeStep){
  uint32_t prevPixelValue;
  uint32_t newPixelValue;
  for(int i=0;i<(this->numPixels()); i++){
    prevPixelValue= this->getPixelColor(i);

    
    //Se obtienen los valores HSV:
    uint16_t H;
    uint8_t S;
    uint8_t V;
    this->rgbToHsv(prevPixelValue, H, S, V);

    //Se reduce el canal V:
    if(V>(fadeStep)){
      V-=fadeStep;
    }else{
      V = 0;
    }
    newPixelValue = this->ColorHSV(H,S,V);

    //se actualizan los valores de cada canal:
    this->setPixelColor(i, newPixelValue);


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
    // this->setPixelColor(i, this->Color(uint8_t(red), uint8_t(green), uint8_t(blue)));
  }
}

void ledStripBase::flashEffect(uint32_t color, bool newFlash, uint8_t fadeStep){
  if(newFlash){
    this->fill(color);
  }else{
    this->fadeDarkAll(fadeStep);
  }
  return;
}