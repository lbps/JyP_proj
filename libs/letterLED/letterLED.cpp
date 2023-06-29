#include "letterLED.h"

letterLED::letterLED (uint8_t numLeds, uint8_t dataPin, uint8_t mainHue)
  : ledStripBase(numLeds, dataPin, mainHue){
};

letterLED::~letterLED(){
};

void letterLED::rainbow() 
{
  Adafruit_NeoPixel* LEDstrip = this->getLedsStrip();
  uint8_t numLeds = this->getNumLeds();
  uint8_t mainHue = this->getMainHue();

  // FastLED's built-in rainbow generator
  
};

void letterLED::addGlitter( fract8 chanceOfGlitter) 
{
  CRGB* ledsArray = this->getLedsArray();
  uint8_t numLeds = this->getNumLeds();

  if( random8() < chanceOfGlitter) {
    ledsArray[ random16(numLeds) ] += CRGB::White;
  }
};

void letterLED::rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
};

void letterLED::confetti() 
{
  CRGB* ledsArray = this->getLedsArray();
  uint8_t numLeds = this->getNumLeds();
  uint8_t mainHue = this->getMainHue();

  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy(ledsArray, numLeds, 10);
  int pos = random16(numLeds);
  ledsArray[pos] = ledsArray[pos] + CHSV( mainHue + random8(64), 200, 255);
};

void letterLED::sinelon()
{
  CRGB* ledsArray = this->getLedsArray();
  uint8_t numLeds = this->getNumLeds();
  uint8_t mainHue = this->getMainHue();
  
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy(ledsArray, numLeds, 20);
  int pos = beatsin16( 13, 0, numLeds-1 );
  ledsArray[pos] = ledsArray[pos] + CHSV( mainHue, 255, 192);
};

void letterLED::bpm()
{
  CRGB* ledsArray = this->getLedsArray();
  uint8_t numLeds = this->getNumLeds();
  uint8_t mainHue = this->getMainHue();

  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < numLeds; i++) { //9948
    ledsArray[i] = ColorFromPalette(palette, mainHue+(i*2), beat-mainHue+(i*10));
  }
};

void letterLED::juggle() {
  CRGB* ledsArray = this->getLedsArray();
  uint8_t numLeds = this->getNumLeds();

  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy(ledsArray, numLeds, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    ledsArray[beatsin16( i+7, 0, numLeds-1 )] = ledsArray[beatsin16( i+7, 0, numLeds-1 )] | CHSV(dothue, 200, 255);
    dothue += 32;
  }
};

void letterLED::runEffect(){

  uint8_t visModeIdx = this->getVisualizationModeIdx();

  switch (visModeIdx)
  {
  case 0:
    letterLED::rainbow();
    break;
  case 1:
    letterLED::rainbowWithGlitter();
    break;
  case 2:
    letterLED::confetti();
    break;
  case 3:
    letterLED::sinelon();
    break;
  case 4:
    letterLED::bpm();
    break;
  case 5:
    letterLED::juggle();
    break;

  default:
    Serial.print("Efecto no disponible: ");
    Serial.println(visModeIdx);
    break;
  }
};