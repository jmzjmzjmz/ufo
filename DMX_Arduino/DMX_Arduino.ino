#include <DmxSimple.h>

unsigned int NUM_DMX = 2;
unsigned int rate = 9;

unsigned long frame = 0;

struct Color { 
  int r;
  int g;
  int b;
  int a;
  int w;
};

typedef struct Color (*Pattern)(long, int);
Pattern patterns[6];
Pattern pattern;

struct Color color1 = (Color){ 255, 0, 0, 0, 0 };
struct Color color2 = (Color){ 0, 0, 255, 0, 0 };


void setup() {
  DmxSimple.usePin(3);
  DmxSimple.maxChannel(4);  
  sanityCheck();
  pattern = &rainbowCycle;
}

void loop() {
  
  frame++;

  for (int i = 0; i < NUM_DMX; i++) {

    int f = frame / 10; // in lieu of rate

    struct Color c = pattern(f, i);

    setDMXColor(i, c.r, c.g, c.b, c.a, c.w);
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

struct Color totesRandom(long f, int dmxIndex) {
  return (Color){ random()*255, random()*255, random()*255, random()*255, random()*255 };
}

struct Color rainbow(long f, int dmxIndex) {
  return wheel(f);
}

struct Color rainbowCycle(long f, int dmxIndex) {
  return wheel(f + dmxIndex * 20);
}

struct Color crossfade(long f, int dmxIndex) {
  float r = 2500;
  return lerpColor(color1, color2, (sin(f/r)+1)/2);
}


// Utilities
struct Color wheel(int WheelPos) {
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

  int d = 100;

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