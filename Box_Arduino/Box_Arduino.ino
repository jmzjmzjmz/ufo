// make an empty tab called ignore.h and this will run.
#include "ignore.h" 

/* TUESDAY Commenting this out makes the RTClib unhappy */
#include "LPD8806.h"

#include "RTClib.h"
#include <SPI.h>
#include <Wire.h>
#include <FastSPI_LED.h>

struct CRGB { 

  unsigned char g; 
  unsigned char r; 
  unsigned char b; 

};

struct CRGB *leds;

#define PIN 4
#define wireAddress 1 

/* These don't seem to work as #define since they're used in other tabs ... */
unsigned int NUM_ROWS = 64; 
unsigned int NUM_PIXELS = 576;

// variables i took out of the loop.
// _i is because it was yelling at me otherwise?
unsigned long t = 0;
unsigned long m = 0;
int _i = 0, j = 0, k = 0;
unsigned char r = 0, g = 0, b = 0;
struct CRGB color;

struct CRGB NULL_COLOR;

boolean light = false;


// Access to the pixel strip

#define NUM_POLES 6

RTC_DS1307 RTC;

#define NULL_PATTERN 0
#define OFF_PATTERN 68
#define PAUSE_PATTERN 67
#define ADV_PATTERN 101
#define RESET_FRAME 100

#define MAX_FRAME 100000

unsigned int incomingBrightness = 0;
unsigned int incomingRate = 0;
unsigned int rate = 60;
unsigned int patternByte = NULL_PATTERN;

// unix timestamp that the sketch starts at
unsigned long startedAt = 0;
unsigned long lastTime = -1;

unsigned long lastMillis = 0;
unsigned long internalTimeSmoother = 0;

float brightness = 1.0;
int r1 = 127, g1 = 0,   b1 = 0, 
    r2 = 0,   g2 = 127,   b2 = 0, 
    r3 = 0,   g3 = 0,   b3 = 0;

float params[20];
struct CRGB color1, color2, color3;

boolean isOff = false;

long frame = 0;
long lastFrame = -1;

typedef CRGB (*Pattern)(long, int);
Pattern patterns[128];
Pattern pattern;

typedef int (*Mapping)(long, int);
Mapping mapping = &forward;

void setup() {  

  //  pinMode(13, OUTPUT); 

  // Wire.begin(wireAddress);
//  Wire.onReceive(receiveEvent); /* TUESDAY Specifically upset with this. */
  Serial.begin(9600); 

  FastSPI_LED.setLeds(NUM_PIXELS);
  FastSPI_LED.setChipset(CFastSPI_LED::SPI_LPD8806);
  FastSPI_LED.setPin(PIN);
  FastSPI_LED.init();
  FastSPI_LED.start();
  leds = (struct CRGB*)FastSPI_LED.getRGBData(); 

//   RTC.begin();
//   if (!RTC.isrunning()) {
// //    Serial.println("RTC is NOT running!"); /* TUESDAY Specifically upset with this. */
//     RTC.adjust(DateTime(__DATE__, __TIME__));
//   }

  
  setColors();

  

   patterns[62] = &flickerStrobeTwo;
  patterns[63] = &flickerStrobeFour;
  patterns[64] = &totesRandom;
  patterns[65] = &rainbowCycle;
  patterns[66] = &rainbow;
  //  // 67 = pause
  //  // 68 = off
  patterns[69] = &solidColor;
  patterns[70] = &gradient;
  patterns[71] = &pulseSine;
  patterns[72] = &pulseSaw;
  //  patterns[73] = &bounce;
  patterns[74] = &colorWipe;
  patterns[75] = &colorAlternator;
  patterns[76] = &stripe;
   patterns[77] = &colorChase;
   // patterns[78] = &colorWipeMeter;
  //  patterns[79] = &colorWipeMeterGradient;
  patterns[80] = &pulseOnce;


  pattern = &flickerStrobeFour;
  // pattern(-2, 0);

  // showAll();

  startedAt = 0;//RTC.now().unixtime();

}

void receiveEvent(int howMany) {
  //wait for 12 incoming bytes
  if (Wire.available() > 11) {

    incomingRate = Wire.read();
    patternByte = Wire.read();

    r1 = Wire.read();
    g1 = Wire.read();
    b1 = Wire.read();
    r2 = Wire.read();
    g2 = Wire.read();
    b2 = Wire.read();
    r3 = Wire.read();
    g3 = Wire.read();
    b3 = Wire.read();

    incomingBrightness = Wire.read()/127.0;

    setBrightnRate();
    setColors();


    if (patternByte == 1) {
      mapping = &forward;
    } 
    else if (patternByte == 2) {
      mapping = &backward;
    } 
    else if (patternByte == 3) {
      mapping = &peak;
    } 
    else if (patternByte == 4) {
      mapping = &valley;
    } 
    else if (patternByte == 5) {
      mapping = &dither;
    } 
    else if (patternByte == OFF_PATTERN) {
      hideAll();
      showAll();
      isOff = true;
    } 
    else if (patternByte != NULL_PATTERN && patterns[patternByte] != NULL) {
      isOff = false;
      pattern = patterns[patternByte];
      pattern(-2, 0); // On select initialization
    }


  }

}

void setColors() {

  color1.r = r1;
  color1.g = g1;
  color1.b = b1;
  
  color2.r = r2;
  color2.g = g2;
  color2.b = b2;
  
  color3.r = r3;
  color3.g = g3;
  color3.b = b3;

}

uint32_t Color(byte r, byte g, byte b) {
  return 0x808080 | ((uint32_t)g << 16) | ((uint32_t)r << 8) | (uint32_t)b;
}

void setBrightnRate() {

  rate = incomingRate;
  brightness = incomingBrightness;

}

void loop() {

  if (isOff){
    hideAll();
    showAll();
    return;
  }

  t = 0;//RTC.now().unixtime();// * 50 / (rate+1);
  m = millis();

  if (t != lastTime) {
    internalTimeSmoother = 0;
  }

  internalTimeSmoother += m - lastMillis;
  lastMillis = m;
  lastTime = t;

  frame = (t * 1000 + internalTimeSmoother) / rate;

  // if (currentTime >= loopTime + rate) { 

  Serial.println(frame);
  Serial.println(lastFrame);
  Serial.println("------");

  if (frame != lastFrame)
    pattern(-1, 0); // Per frame initialization

  lastFrame = frame;

  for (_i = 0; _i < NUM_PIXELS; _i++) {

    k = snake(frame, _i);
    j = mapping(frame, k);
    color = pattern(frame, j);

//    r = red(color), g = green(color), b = blue(color);

    if (brightness < 1) {
      color.r = lerp(0, gamma(r), brightness);
      color.g = lerp(0, gamma(g), brightness);
      color.b = lerp(0, gamma(b), brightness);
    }

    //      strip.setPixelColor(_i, r, g, b);
    leds[_i].r = color.r;
    leds[_i].g = color.g;
    leds[_i].b = color.b;


    //      if (i == 0) {
    //        Serial.println("color ");
    //        Serial.println(r);
    //        Serial.println(g);
    //        Serial.println(b);
    //        Serial.println("frame ");
    //        Serial.println(frame);
    //        
    //        Serial.println("===================== ");
    //      }

  }

  showAll();  

  if (frame >= MAX_FRAME) { 
    frame = 0;
  } 
  // Serial.println("frame is " + frame);
  // frame++;

  // loopTime = currentTime;  // Updates loopTime


  // }
  //advance = false;

  //  delay(100);

  //  hideAll();
  //  showAll();


  if (light)
    digitalWrite(13, HIGH);
  else
    digitalWrite(13, LOW);

  light = !light;

}


// void loop(){
// }

/* Helper functions */

//Input a rateue 0 to 384 to get a color rateue.
//The colours are a transition r - g -b - back to r

void hideAll() {
  memset(leds, 0, NUM_PIXELS * 3);
  //  for (int i = 0; i < strip.numPixels(); i++) {
  //    strip.setPixelColor(i, 0); 
  //  }
}

void showAll(){
  FastSPI_LED.show();
  //  strip.show(1);    
}














