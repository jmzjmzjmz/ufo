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
#define myADDRESS 17
#define mySETADDRESS 14
#define globalADDR 19

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
#define TIMING_ADDR 100
#define PAUSE_PATTERN 67
#define ADV_PATTERN 101
// #define RESET_FRAME 100

#define MAX_FRAME 100000

unsigned int incomingBrightness = 0;
unsigned int incomingRate = 0;
unsigned int rate = 2;
unsigned int patternByte = NULL_PATTERN;

unsigned long lastTime = -1;


float brightness = 1.0;
int r1 = 25,  g1 = 25,  b1 = 25, 
    r2 = 0,   g2 = 0,   b2 = 0, 
    r3 = 0,   g3 = 0,   b3 = 0;

float params[20];
struct CRGB color1, color2, color3;

boolean isOff = false;
boolean doSnake = true;

long frame = 0;
long lastFrame = -1;

typedef CRGB (*Pattern)(long, int);
Pattern patterns[128];
Pattern pattern;

typedef int (*Mapping)(long, int);
Mapping mapping = &forward;


unsigned long currentTime;
unsigned long lastMillis;
unsigned long internalTimeSmoother;

String inputString = "";
boolean stringComplete = false;


void setup() {  

  //  pinMode(13, OUTPUT); 

  // Wire.begin(0);
  // Wire.onReceive(receiveEvent); /* TUESDAY Specifically upset with this. */

  Serial.begin(9600);
  Serial1.begin(9600); 

  FastSPI_LED.setLeds(NUM_PIXELS);
  FastSPI_LED.setChipset(CFastSPI_LED::SPI_LPD8806);
  FastSPI_LED.setPin(PIN);
  FastSPI_LED.init();
  FastSPI_LED.start();
  leds = (struct CRGB*)FastSPI_LED.getRGBData(); 

  // RTC.begin();
//   if (!RTC.isrunning()) {
// //    Serial1.println("RTC is NOT running!"); /* TUESDAY Specifically upset with this. */
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
  patterns[73] = &bounce;
  patterns[74] = &colorWipe;
  patterns[75] = &colorAlternator;
  patterns[76] = &stripe;
  patterns[77] = &colorChase;
  patterns[78] = &colorWipeMeter;
  patterns[79] = &colorWipeMeterGradient;
  patterns[80] = &pulseOnce;


  pattern = &pulseOnce;
  pattern(-2, 0);

  inputString.reserve(200);


}

void read() {



  while (Serial1.available()) {

    char c = (char)Serial1.read();
    inputString += c;
    
    if (c == 128) {

      for (int i = 0; i < inputString.length(); i++) {
        Serial.print(inputString.charAt(i), DEC);
        if (i == inputString.length()-1) {
          Serial.println(" | ");
        } else { 
          Serial.print(" | ");
        }
      }
    
      if (inputString.startsWith('d')) {

        // Big fat hack to turn a String into an int.
        String sub = inputString.substring(1, inputString.length()-1);
        char c[sub.length()];
        for (int i = 0; i < sub.length(); i++) {
          c[i] = sub.charAt(i);
        }
        currentTime = atol(c);

        // Serial.print("Current time: ");
        // Serial.println(currentTime);


      } else { 

        unsigned char addr = (unsigned char)inputString.charAt(0);
        // Serial.println("PATTERN");
        // Serial.println(addr);

        // Pattern.
        

        if (addr == myADDRESS || addr == mySETADDRESS || addr == globalADDR)  {
          
          rate = (unsigned char)inputString.charAt(1);
          patternByte = (unsigned char)inputString.charAt(2);

          r1 = (unsigned char)inputString.charAt(3);
          g1 = (unsigned char)inputString.charAt(4);
          b1 = (unsigned char)inputString.charAt(5);
          r2 = (unsigned char)inputString.charAt(6);
          g2 = (unsigned char)inputString.charAt(7);
          b2 = (unsigned char)inputString.charAt(8);
          r3 = (unsigned char)inputString.charAt(9);
          g3 = (unsigned char)inputString.charAt(10);
          b3 = (unsigned char)inputString.charAt(11);

          brightness = ((unsigned char)inputString.charAt(12))/127.0;

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

      inputString = "";

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

  frame = (currentTime + internalTimeSmoother) / rate;

  

  if (frame != lastFrame)
    pattern(-1, 0); // Per frame initialization

  lastFrame = frame;

  for (_i = 0; _i < NUM_PIXELS; _i++) {

    if (doSnake) k = snake(frame, _i);
    else k = _i;

    j = mapping(frame, k);
    color = pattern(frame, j);

//    r = red(color), g = green(color), b = blue(color);

    if (brightness < 1) {
      color.r = lerp(0, color.r, brightness);
      color.g = lerp(0, color.g, brightness);
      color.b = lerp(0, color.b, brightness);
    }

    //      strip.setPixelColor(_i, r, g, b);
    leds[_i].r = color.r;
    leds[_i].g = color.g;
    leds[_i].b = color.b;


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
  // Serial1.println("frame is " + frame);
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

  // delay(500);

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

void showAll() {
  FastSPI_LED.show();
}
