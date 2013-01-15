// make an empty tab called ignore.h and this will run.
#include "ignore.h" 
#include "LPD8806.h"

// #include "RTClib.h"
#include <SPI.h>
#include <Wire.h>

// #define wireAddress 1 
boolean light = false;

// unsigned long currentTime = 0;
// unsigned long loopTime = 0;

//#ifndef _SERIAL
//#define _SERIAL Uart
//HardwareSerial1 Uart = HardwareSerial1();
//#endif

// Access to the pixel strip

#define NUM_POLES 6

#define myADDRESS 2

LPD8806 strip = LPD8806(40);

// RTC_DS1307 RTC;

#define NULL_PATTERN 0
#define OFF_PATTERN 68
#define PAUSE_PATTERN 67
#define ADV_PATTERN 101
#define RESET_FRAME 100

#define TIMING_ADDR 100

#define MAX_FRAME 100000

unsigned int incomingBrightness=0;
unsigned int incomingRate=0;
unsigned int rate = 2;
unsigned int patternByte = NULL_PATTERN;

// unix timestamp that the sketch starts at
unsigned long startedAt = 0;
unsigned long lastTime = -1;

float brightness = 1.0;
int r1 = 127, g1 = 127, b1 = 127, r2 = 0, g2 = 0, b2 = 0, r3 = 0, g3 = 0, b3 = 0;

float params[20];
uint32_t color1, color2, color3;

boolean isOff = false;
boolean advance = false;

long frame = 0;
long lastFrame = -1;

typedef uint32_t (*Pattern)(long, int);
Pattern patterns[128];
Pattern pattern;

typedef int (*Mapping)(long, int);
Mapping mapping = &forward;

int currentTime;
unsigned long lastMillis;
unsigned long internalTimeSmoother;

void setup() {  
  
  pinMode(13, OUTPUT); 

  Serial1.begin(9600); 
  Serial.begin(9600);
  
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

  pattern = &rainbow;

  startedAt = 0;//RTC.now().unixtime();

}

void read() {

  // return;

  //wait for 12 incoming bytes
  if (Serial1.available() > 12) {

    unsigned char address = Serial1.read();
//Serial.println(address);

    if (address == TIMING_ADDR) {
      currentTime = Serial1.parseInt();
      Serial.println(currentTime);
      Serial1.flush();
      return;
    }

    if (address != myADDRESS){
      Serial1.flush();
      return;
    }



    incomingRate = Serial1.read();
    patternByte = Serial1.read();

    // Serial.println(patternByte);

    r1 = Serial1.read();
    g1 = Serial1.read();
    b1 = Serial1.read();
    r2 = Serial1.read();
    g2 = Serial1.read();
    b2 = Serial1.read();
    r3 = Serial1.read();
    g3 = Serial1.read();
    b3 = Serial1.read();

    incomingBrightness = Serial1.read()/127.0;

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
      // now = 0;
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

  read();

  if (isOff){
    hideAll();
    showAll();
    return;
  }
 
  unsigned long currentMillis = millis();

  if (currentTime != lastTime) {
    internalTimeSmoother = 0;
  }

  internalTimeSmoother += currentMillis - lastMillis;

  lastMillis = currentMillis;
  lastTime = currentTime;

  // int t = (currentTime + timesCycled * 256);

  frame = (currentTime * 1000 + internalTimeSmoother) / rate;


  // if (currentTime >= loopTime + rate) { 

  if (frame != lastFrame)
    pattern(-1, 0); // Per frame initialization

  lastFrame = frame;

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
    //        Serial1.println("color ");
    //        Serial1.println(r);
    //        Serial1.println(g);
    //        Serial1.println(b);
    //        Serial1.println("frame ");
    //        Serial1.println(frame);
    //        
    //        Serial1.println("===================== ");
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
  strip.show();    
}


