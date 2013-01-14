#include <DmxSimple.h>
#include <Wire.h>

#define OFF_PATTERN 68
#define myADDRESS 2

unsigned int NUM_DMX = 4;
unsigned int rate = 127;

unsigned long frame = 0;
unsigned long lastFrame = 0;
unsigned long lastTime = 0;

unsigned long lastMillis = 0;
unsigned long internalTimeSmoother = 0;

struct Color { 
  int r;
  int g;
  int b;
};

typedef struct Color (*Pattern)(long, int);
Pattern patterns[128];
Pattern pattern;

struct Color color1 = (Color){ 0, 0, 255 };
struct Color color2 = (Color){ 255, 0, 255 };

struct Color BLACK = (Color){ 0, 0, 0 };

float params[20];

bool isOff = false;

void setup() {

  Serial1.begin(9600); 

  DmxSimple.usePin(3);
  DmxSimple.maxChannel(4);  
     

  patterns[62] = &flickerStrobe;
  patterns[65] = &rainbowCycle;
  patterns[66] = &rainbow;
  patterns[69] = &solidColor;
  patterns[71] = &crossfade;
  patterns[75] = &colorAlternator;
  patterns[80] = &pulseOnce;

  pattern = &flickerStrobe;

  sanityCheck();

}

void loop() {
  
read();
  if (isOff)
    return;


  unsigned long t = 0;//RTC.now().unixtime();// * 50 / (rate+1);
  unsigned long m = millis();

  if (t != lastTime) {
    internalTimeSmoother = 0;
  }

  internalTimeSmoother += m - lastMillis;
  lastMillis = m;
  lastTime = t;

  frame = millis() / rate;

  if (frame != lastFrame)
    pattern(-1, 0); // Per frame initialization

  lastFrame = frame;

  for (int i = 0; i < NUM_DMX; i++) {

    struct Color c = pattern(frame, i);

    setDMXColor(i, c.r, c.g, c.b);

  }


}


void read() {

  // wait for 12 incoming bytes
  if (Serial1.available() > 12) {

    if (Serial1.read() != myADDRESS){
      
      Serial1.flush();
      return;

    }

    rate = Serial1.read();
    byte patternByte = Serial1.read();

    color1.r = Serial1.read();
    color1.g = Serial1.read();
    color1.b = Serial1.read();

    color2.r = Serial1.read();
    color2.g = Serial1.read();
    color2.b = Serial1.read();

    Serial1.read();
    Serial1.read();
    Serial1.read();
    
    Serial1.read();

    if (patternByte == OFF_PATTERN) {
      isOff = true;
      hideAll();
    } else if (patterns[patternByte] != NULL) {
      isOff = false;
      pattern = patterns[patternByte];
      pattern(-2, 0);
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
  return wheel(f);
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