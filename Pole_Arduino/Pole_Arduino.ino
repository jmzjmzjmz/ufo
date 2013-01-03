// make an empty tab called ignore.h and this will run.
#include "ignore.h" 
#include "LPD8806.h"

#include "RTClib.h"
#include <SPI.h>
#include <Wire.h>

#define wireAddress 1 


unsigned long currentTime = 0;
unsigned long loopTime = 0;

//#ifndef _SERIAL
//#define _SERIAL Uart
//HardwareSerial Uart = HardwareSerial();
//#endif

// Access to the pixel strip

#define NUM_POLES 6

LPD8806 strip = LPD8806(40);

RTC_DS1307 RTC;

#define NULL_PATTERN 0
#define OFF_PATTERN 68
#define PAUSE_PATTERN 67
#define ADV_PATTERN 101
#define RESET_FRAME 100

#define MAX_FRAME 100000

unsigned int incomingBrightness=0;
unsigned int incomingRate=0;
unsigned int rate = 9;
unsigned int patternByte = NULL_PATTERN;

float brightness = 1.0;
int r1 = 127, g1 = 127, b1 = 127, r2 = 0, g2 = 0, b2 = 0, r3 = 0, g3 = 0, b3 = 0;

float params[20];
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

  loopTime = millis();

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

}

void receiveEvent(int howMany) {
  //wait for 12 incoming bytes
  Serial.println("RECEIVED EVENT");
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

    //    Serial.print("rate: ");
    //    Serial.println(rate);
    //    Serial.print("pattern: ");
    //    Serial.println(patternByte);
    //    Serial.print("r1: ");
    //    Serial.println(r1);
    //    Serial.print("g1: ");
    //    Serial.println(g1);
    //    Serial.print("b1: ");
    //    Serial.println(b1);
    //    Serial.print("r2: ");
    //    Serial.println(r2);
    //    Serial.print("g2: ");
    //    Serial.println(g2);
    //    Serial.print("b2: ");
    //    Serial.println(b2);
    //    Serial.print("r3: ");
    //    Serial.println(r3);
    //    Serial.print("g3: ");
    //    Serial.println(g3);
    //    Serial.print("b3: ");
    //    Serial.println(b3);
    //    Serial.print("brightness: ");
    //    Serial.println(brightness);
    //    Serial.print("frame: ");
    //    Serial.println(frame);
    //    Serial.println("time: ");
    //    Serial.println(currentTime);
    //    Serial.println("isOff: ");
    //    Serial.println(isOff);
    //    Serial.println("=========================");

    if(patternByte != RESET_FRAME && patternByte != ADV_PATTERN){
      setBrightnRate();
      setColors();
    }


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
    else if (patternByte == RESET_FRAME) {
      Serial.println("RESET FRAME");
      frame = 0;
    } 
    else if (patternByte == ADV_PATTERN) {
      Serial.println("FRAME == " + frame);
      // _loop();
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

  if (isOff)
    return;

  frame = RTC.now().unixtime() / (rate+1);

  // if (currentTime >= loopTime + rate) { 

  pattern(-1, 0); // Per frame initialization

  for (int i = 0; i < strip.numPixels(); i++) {

    int j = mapping(frame, i);
    uint32_t color = pattern(frame, j);


    uint8_t r = red(color), g = green(color), b = blue(color);


    if (brightness < 1) {
      r = lerp(0, gamma(r), brightness);
      g = lerp(0, gamma(g), brightness);
      b = lerp(0, gamma(b), brightness);
    }

    strip.setPixelColor(i, r, g, b);


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

    // frame++;

    // loopTime = currentTime;  // Updates loopTime

  // }
//advance = false;

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












