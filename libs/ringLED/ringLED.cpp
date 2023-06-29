#include "ringLED.h"

ringLED::ringLED (uint8_t numLeds, uint8_t dataPin, uint8_t mainHue)
  : ledStripBase(numLeds, dataPin, mainHue){
};

ringLED::~ringLED(){
};

void ringLED::rainbow() 
{
  Adafruit_NeoPixel* LEDstrip = this->getLedsStrip();
  uint8_t numLeds = this->getNumLeds();
  uint8_t mainHue = this->getMainHue();

  // implementacion rainbow Neopixel
  for (uint16_t i = 0; i < numLeds; i++) {
    LEDstrip->setPixelColor(i, CHSV(mainHue + i * (255 / numLeds));
  };
};

uint32_t Wheel(Adafruit_NeoPixel* LEDstrip, byte WheelPos) {

  if(WheelPos < 85) {
    return LEDstrip->Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
    return LEDstrip->Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return LEDstrip->Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}





// void ringLED::addGlitter( fract8 chanceOfGlitter) 
// {
//   CRGB* ledsArray = this->getLedsArray();
//   uint8_t numLeds = this->getNumLeds();

//   if( random8() < chanceOfGlitter) {
//     ledsArray[ random16(numLeds) ] += CRGB::White;
//   }
// };

// void ringLED::rainbowWithGlitter() 
// {
//   // built-in FastLED rainbow, plus some random sparkly glitter
//   rainbow();
//   addGlitter(80);
// };

// void ringLED::confetti() 
// {
//   CRGB* ledsArray = this->getLedsArray();
//   uint8_t numLeds = this->getNumLeds();
//   uint8_t mainHue = this->getMainHue();

//   // random colored speckles that blink in and fade smoothly
//   fadeToBlackBy(ledsArray, numLeds, 10);
//   int pos = random16(numLeds);
//   ledsArray[pos] = ledsArray[pos] + CHSV( mainHue + random8(64), 200, 255);
// };

// void ringLED::sinelon()
// {
//   CRGB* ledsArray = this->getLedsArray();
//   uint8_t numLeds = this->getNumLeds();
//   uint8_t mainHue = this->getMainHue();
  
//   // a colored dot sweeping back and forth, with fading trails
//   fadeToBlackBy(ledsArray, numLeds, 20);
//   int pos = beatsin16( 13, 0, numLeds-1 );
//   ledsArray[pos] = ledsArray[pos] + CHSV( mainHue, 255, 192);
// };

// void ringLED::bpm()
// {
//   CRGB* ledsArray = this->getLedsArray();
//   uint8_t numLeds = this->getNumLeds();
//   uint8_t mainHue = this->getMainHue();

//   // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
//   uint8_t BeatsPerMinute = 62;
//   CRGBPalette16 palette = PartyColors_p;
//   uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
//   for( int i = 0; i < numLeds; i++) { //9948
//     ledsArray[i] = ColorFromPalette(palette, mainHue+(i*2), beat-mainHue+(i*10));
//   }
// };

// void ringLED::juggle() {
//   CRGB* ledsArray = this->getLedsArray();
//   uint8_t numLeds = this->getNumLeds();

//   // eight colored dots, weaving in and out of sync with each other
//   fadeToBlackBy(ledsArray, numLeds, 20);
//   byte dothue = 0;
//   for( int i = 0; i < 8; i++) {
//     ledsArray[beatsin16( i+7, 0, numLeds-1 )] = ledsArray[beatsin16( i+7, 0, numLeds-1 )] | CHSV(dothue, 200, 255);
//     dothue += 32;
//   }
// };

void ringLED::runEffect(){

  uint8_t visModeIdx = this->getVisualizationModeIdx();

  switch (visModeIdx)
  {
  case 0:
    ringLED::rainbow();
    break;
  case 1:
    ringLED::rainbowWithGlitter();
    break;
  case 2:
    ringLED::confetti();
    break;
  case 3:
    ringLED::sinelon();
    break;
  case 4:
    ringLED::bpm();
    break;
  case 5:
    ringLED::juggle();
    break;

  default:
    Serial.print("Efecto no disponible: ");
    Serial.println(visModeIdx);
    break;
  }
};

void ringLED::ringWheel(uint8_t ledIdx, uint8_t fadeToBlackPercent)
{
  CRGB* ledsArray = this->getLedsArray();
  uint8_t numLeds = this->getNumLeds();
  uint8_t mainHue = this->getMainHue();

  //Se mapea el porecntaje de reduccion de intensidad a un numero entre 0 y 255
  uint8_t fadeToBlackValue = int(map(fadeToBlackPercent, 0, 100, 0, 255));

  // Se apaga la intensidad de los leds encendidos en llamadas anteriores
  fadeToBlackBy(ledsArray, numLeds, fadeToBlackValue);

  //Se enciende el LED principal (indicado con ledIdx)
  ledsArray[ledIdx] = CHSV( mainHue, 255, 192);

};

void ringLED::ringFlash(bool newFlash, CRGB rgbFlash, uint8_t fadeToBlackPercent){

  CRGB* ledsArray = this->getLedsArray();
  uint8_t numLeds = this->getNumLeds();

  if(newFlash){
    for(uint8_t i = 0; i < numLeds; ++i) {
      ledsArray[i] = rgbFlash; 
    }
  }else{
    //Se mapea el porecntaje de reduccion de intensidad a un numero entre 0 y 255
    uint8_t fadeToBlackValue = int(map(fadeToBlackPercent, 0, 100, 0, 255));

    // Se apaga la intensidad de los leds encendidos en llamadas anteriores
    fadeToBlackBy(ledsArray, numLeds, fadeToBlackValue);
  }

};
