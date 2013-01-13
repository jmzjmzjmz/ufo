#include <DmxSimple.h>
#include <Wire.h>

#define OFF_PATTERN 68
#define myADDRESS 1

unsigned int NUM_DMX = 4;
unsigned int rate = 127;

unsigned long frame = 0;

unsigned long currentTime = 0;
unsigned long loopTime = 0;


struct Color { 
  int r;
  int g;
  int b;
  int a;
  int w;
};

typedef struct Color (*Pattern)(long, int);
Pattern patterns[128];
Pattern pattern;

struct Color color1 = (Color){ 0, 0, 255, 0, 0 };
struct Color color2 = (Color){ 255, 0, 255, 0, 0 };

struct Color BLACK = (Color){ 0, 0, 0, 0, 0 };

float params[20];

bool isOff = false;

void setup() {

  Serial.begin(9600); 

  DmxSimple.usePin(3);
  DmxSimple.maxChannel(4);  
     // sanityCheck();


  patterns[62] = &flickerStrobe;
  patterns[65] = &rainbowCycle;
  patterns[66] = &rainbow;
  patterns[69] = &solidColor;
  patterns[71] = &crossfade;
  patterns[75] = &colorAlternator;

  pattern = &flickerStrobe;

}

void loop() {
  

  if (isOff)
    return;

  currentTime = millis();



  if (currentTime >= loopTime + rate) { 


    frame++;

    pattern(-1, 0); // per frame init

    for (int i = 0; i < NUM_DMX; i++) {

      struct Color c = pattern(frame, i);

      setDMXColor(i, c.r, c.g, c.b, c.a, c.w);

    }

    loopTime = currentTime; 

  }

}


void serialEvent() {

  // wait for 12 incoming bytes
  if (Serial.available() > 12) {

 if(Serial.read() != myADDRESS){
      Serial.flush();
      return;
    }
    rate = Serial.read();
    byte patternByte = Serial.read();

    color1.r = Serial.read();
    color1.g = Serial.read();
    color1.b = Serial.read();

    color2.r = Serial.read();
    color2.g = Serial.read();
    color2.b = Serial.read();

    Serial.read();
    Serial.read();
    Serial.read();
    
    Serial.read();

    if (patternByte == OFF_PATTERN) {
      isOff = true;
    } else if (patterns[patternByte] != NULL) {
      isOff = false;
      pattern = patterns[patternByte];
    }



  }

}

void setDMXColor(int dmxIndex, int r, int g, int b, int a, int w) {
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
  float r = 100;
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

void hideAll() {
  for (int i = 0; i < NUM_DMX; i++) {
    setDMXColor(i, 0, 0, 0, 0, 0);
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
  return (Color){ r*2, g*2, b*2, 0, 0 };
}

struct Color lerpColor(struct Color a, struct Color b, float t) {
  int cr = lerp(a.r, b.r, t);
  int cg = lerp(a.g, b.g, t);
  int cb = lerp(a.b, b.b, t);
  int ca = lerp(a.a, b.a, t);
  int cw = lerp(a.w, b.w, t);
  return (Color){ cr, cg, cb, ca, cw };
}

int lerp(int a, int b, float t) {
  return a + (b - a) *t;
}

void sanityCheck() {
  return;
  int d = 10;

  for (int i = 0; i < NUM_DMX; i++) {
    setDMXColor(i, 255, 0, 0, 0, 0);
    delay(d);
    setDMXColor(i, 0, 255, 0, 0, 0);
    delay(d);
    setDMXColor(i, 0, 0, 255, 0, 0);
    delay(d);    
    setDMXColor(i, 0, 0, 0, 255, 0);
    delay(d);    
    setDMXColor(i, 0, 0, 0, 0, 255);
    delay(d);    
    setDMXColor(i, 0, 0, 0, 0, 0);
  }

}