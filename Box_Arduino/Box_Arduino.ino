// make an empty tab called ignore.h and this will run.
#include "ignore.h" 
#include "LPD8806.h"

#include "RTClib.h"
#include <SPI.h>
#include <Wire.h>

// this doesn't work as define?
unsigned int NUM_ROWS = 64; 


// variables i took out of the loop.
// _i is because it was yelling at me otherwise?
unsigned long t = 0;
unsigned long m = 0;


int _i = 0, j = 0, k = 0;
uint8_t r = 0, g = 0, b = 0;
uint32_t color = 0;

#define NUM_PIXELS 526                   

#define wireAddress 1 
boolean light = false;

// unsigned long currentTime = 0;
// unsigned long loopTime = 0;

//#ifndef _SERIAL
//#define _SERIAL Uart
//HardwareSerial Uart = HardwareSerial();
//#endif

// Access to the pixel strip

#define NUM_POLES 6

LPD8806 strip = LPD8806(NUM_PIXELS);

RTC_DS1307 RTC;

#define NULL_PATTERN 0
#define OFF_PATTERN 68
#define PAUSE_PATTERN 67
#define ADV_PATTERN 101
#define RESET_FRAME 100

#define MAX_FRAME 100000

unsigned int incomingBrightness=0;
unsigned int incomingRate=0;
unsigned int rate = 0;
unsigned int patternByte = NULL_PATTERN;

// unix timestamp that the sketch starts at
unsigned long startedAt = 0;
unsigned long lastTime = -1;

unsigned long lastMillis = 0;
unsigned long internalTimeSmoother = 0;

float brightness = 1.0;
int r1 = 127, g1 = 127, b1 = 127, r2 = 0, g2 = 0, b2 = 0, r3 = 0, g3 = 0, b3 = 0;

float params[30];
uint32_t color1, color2, color3;

boolean isOff = false;
boolean advance = false;

long frame = 0;

typedef uint32_t (*Pattern)(long, int);
Pattern patterns[128];
Pattern pattern;

typedef int (*Mapping)(long, int);
Mapping mapping = &forward;

//typedef struct {
//  int rate;
//  int brightness;
//  int red1;
//  int green1;
//  int blue1;
//  int red2;
//  int green2;
//  int blue2;
//  int pattern;
//} 
//lightPoles;
//
//lightPoles pole[NUM_POLES];

void setup() {  

  pinMode(13, OUTPUT); 

  Wire.begin(wireAddress);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600); 

  RTC.begin();
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }

  strip.begin();

  hideAll();
  showAll();

  patterns[62] = &flickerStrobeTwo;
  patterns[63] = &flickerStrobeFour;
  patterns[64] = &totesRandom;
  patterns[65] = &rainbowCycle;
  patterns[66] = &rainbow;
  // 67 = pause
  // 68 = off
  patterns[69] = &solidColor;
  patterns[70] = &gradient;
  patterns[71] = &pulseSine;
  patterns[72] = &pulseSaw;
  patterns[73] = &bounce;
  patterns[74] = &colorWipe;
  patterns[75] = &colorAlternator;
  patterns[76] = &stripe;
  patterns[77] = &colorChase;
  patterns[78] = &colorWipeMeter;
  patterns[79] = &colorWipeMeterGradient;
  patterns[80] = &pulseOnce;

  startedAt = RTC.now().unixtime();

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

  color1 = strip.Color(r1, g1, b1);
  color2 = strip.Color(r2, g2, b2);
  color3 = strip.Color(r3, g3, b3);

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

  t = RTC.now().unixtime();// * 50 / (rate+1);
  m = millis();

  if (t != lastTime) {
    internalTimeSmoother = 0;
  }

  internalTimeSmoother += m - lastMillis;
  lastMillis = m;
  lastTime = t;

  frame = (t * 1000 + internalTimeSmoother) / rate;

  // if (currentTime >= loopTime + rate) { 

  pattern(-1, 0); // Per frame initialization

  for (_i = 0; _i < NUM_PIXELS; _i++) {

    k = snake(frame, _i);
    j = mapping(frame, k);
    color = pattern(frame, j);


    if (brightness < 1) {
      r = red(color), g = green(color), b = blue(color);
      r = lerp(0, gamma(r), brightness);
      g = lerp(0, gamma(g), brightness);
      b = lerp(0, gamma(b), brightness);
      strip.setPixelColor(_i, r, g, b);

    } 
    else { 
      strip.setPixelColor(_i, color);
    }



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
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 0); 
  }
}

void showAll(){
  strip.show(1);    
}













