#include <DmxSimple.h>
#include <Wire.h>

#define OFF_PATTERN 68
#define myADDRESS 18
#define mySETADDRESS 18
#define globalADDR 19
#define NULL_PATTERN 0


unsigned int NUM_DMX = 8;
unsigned int rate = 1;

unsigned long frame = 0;
unsigned long lastFrame = 0;

unsigned long lastTime;
unsigned long currentTime;

unsigned long internalTimeSmoother;

unsigned long currentMillis;
unsigned long lastMillis = -1;
unsigned long millisCounter = 0;

struct Color { 
  int r;
  int g;
  int b;
};

typedef struct Color (*Pattern)(long, int);
Pattern patterns[128];
Pattern pattern;

struct Color color1 = (Color){ 70, 70, 70 };
struct Color color2 = (Color){ 0, 0, 0 };

struct Color BLACK = (Color){ 0, 0, 0 };

float params[20];

float brightness = 1.0;
bool isOff = false;

String inputString = "";

boolean light = false;

void setup() {

  Serial1.begin(9600); 

  DmxSimple.usePin(3);
  DmxSimple.maxChannel(50);  
     
  inputString.reserve(200);

  patterns[62] = &flickerStrobe;
  patterns[65] = &rainbowCycle;
  patterns[66] = &rainbow;
  patterns[69] = &solidColor;
  patterns[71] = &crossfade;
  patterns[75] = &colorAlternator;
  patterns[80] = &pulseOnce;

  pattern = &pulseOnce;
  pattern(-2, 0);
  // sanityCheck();

}

void loop() {
  
  read();
  
  if (isOff)
    return;


  unsigned long currentMillis = millis();

  if (currentTime != lastTime) {
    internalTimeSmoother = 0;
  }

  internalTimeSmoother += currentMillis - lastMillis;

  lastMillis = currentMillis;
  lastTime = currentTime;

  // int t = (currentTime + timesCycled * 256);

  frame = (currentTime + internalTimeSmoother) / rate;

  // Serial.println(currentTime + internalTimeSmoother);

  if (frame != lastFrame)
    pattern(-1, 0); // Per frame initialization

  lastFrame = frame;

  for (int i = 0; i < NUM_DMX; i++) {

    struct Color c = pattern(frame, i);

    setDMXColor(i, c.r*brightness, c.g*brightness, c.b*brightness);

  }


  if (light)
    digitalWrite(13, HIGH);
  else
    digitalWrite(13, LOW);

  light = !light;

}

void read() {
  
  while (Serial1.available()) {

    char c = (char)Serial1.read();
    inputString += c;
    if (c == -128) {


      // for (int i = 0; i < inputString.length(); i++) {
      //   Serial.print(inputString.charAt(i), DEC);
      //   if (i == inputString.length()-1) {
      //     Serial.println(" | ");
      //   } else { 
      //     Serial.print(" | ");
      //   }
      // }
      
      if (inputString.charAt(0) == -127) {

        // Heartbeat.

        // Serial.println("HEARTBEAT");

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
        if (addr == myADDRESS || addr == mySETADDRESS || addr == globalADDR) {
          
          rate = (unsigned char)inputString.charAt(1) + 1;
          unsigned char patternByte = (unsigned char)inputString.charAt(2);

          color1.r = (unsigned char)inputString.charAt(3);
          color1.g = (unsigned char)inputString.charAt(4);
          color1.b = (unsigned char)inputString.charAt(5);
          color2.r = (unsigned char)inputString.charAt(6);
          color2.g = (unsigned char)inputString.charAt(7);
          color2.b = (unsigned char)inputString.charAt(8);
          // r3 = (unsigned char)inputString.charAt(9);
          // g3 = (unsigned char)inputString.charAt(10);
          // b3 = (unsigned char)inputString.charAt(11);

          brightness = ((unsigned char)inputString.charAt(12))/127.0;

          // setColors();
          
          if (patternByte == OFF_PATTERN) {
            hideAll();
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

void setDMXColor(int dmxIndex, int r, int g, int b) {

  int a = 0, w = 0;

  if (r == g && g == b) {
    w = r;
    r = 0;
    g = 0;
    b = 0;
  }

  DmxSimple.write(dmxIndex * 5 + 1, r);
  DmxSimple.write(dmxIndex * 5 + 2, g);
  DmxSimple.write(dmxIndex * 5 + 3, b);
  DmxSimple.write(dmxIndex * 5 + 4, a);
  DmxSimple.write(dmxIndex * 5 + 5, w);

}

struct Color solidColor(long f, int dmxIndex) {
  return color1;
}

struct Color colorAlternator(long f, int dmxIndex) {
  if (f % 2 == 0)
    return color1;
  return color2;
}

struct Color rainbow(long f, int dmxIndex) {
  return wheel(f % 384);
}

struct Color rainbowCycle(long f, int dmxIndex) {
  return wheel(f + dmxIndex * 40);
}

struct Color crossfade(long f, int dmxIndex) {
  float r = 25;
  return lerpColor(color1, color2, (sin(f/r)+1)/2);
}

struct Color flickerStrobe(long f, int dmxIndex) {
  
  if (f == -1) {
    params[0] = random(NUM_DMX);
    params[1] = random(NUM_DMX);
    return BLACK;
  }

  if (dmxIndex == (int)params[0]) {
    return color1;
  } else if (dmxIndex == (int)params[1]) {
    return color2;
  } else { 
    return BLACK;
  }
  
}

struct Color pulseOnce(long f, int pixelIndex) {

  if (f == -2) {
    params[0] = 1.0;
    return BLACK;
  }

  if (f == -1) {
    params[0] *= 0.9;
    return BLACK;
  }

  return lerpColor(color2, color1, params[0]);  

}

void hideAll() {
  for (int i = 0; i < NUM_DMX; i++) {
    setDMXColor(i, 0, 0, 0);
  }
}

// Utilities
struct Color wheel(uint16_t WheelPos) {
  WheelPos %= 384;
  int r, g, b;
  switch(WheelPos / 128) {
  case 0:
    r = 127 - WheelPos % 128;   //Red down
    g = WheelPos % 128;      // Green up
    b = 0;                  //blue off
    break; 
  case 1:
    g = 127 - WheelPos % 128;  //green down
    b = WheelPos % 128;      //blue up
    r = 0;                  //red off
    break; 
  case 2:
    b = 127 - WheelPos % 128;  //blue down 
    r = WheelPos % 128;      //red up
    g = 0;                  //green off
    break; 
  }
  return (Color){ r*2, g*2, b*2 };
}

struct Color lerpColor(struct Color a, struct Color b, float t) {
  int cr = lerp(a.r, b.r, t);
  int cg = lerp(a.g, b.g, t);
  int cb = lerp(a.b, b.b, t);
  return (Color){ cr, cg, cb };
}

int lerp(int a, int b, float t) {
  return a + (b - a) *t;
}

void sanityCheck() {

  int d = 10;

  for (int i = 0; i < NUM_DMX; i++) {
    setDMXColor(i, 255, 0, 0);
    delay(d);
    setDMXColor(i, 0, 255, 0);
    delay(d);
    setDMXColor(i, 0, 0, 255);
    delay(d);    
    setDMXColor(i, 0, 0, 0);
    delay(d);    
    setDMXColor(i, 0, 0, 0);
    delay(d);    
    setDMXColor(i, 0, 0, 0);
  }

}
