struct CRGB solidColor(long f, int pixelIndex) {
 if (f < 0) return NULL_COLOR;
 return color1;
}

struct CRGB colorWipe(long f, int pixelIndex) {
  if (f < 0) return NULL_COLOR;
  if (pixelIndex % NUM_ROWS < f % NUM_ROWS)
    return color1;
  return color2;
}

struct CRGB rainbow(long f, int pixelIndex) {
 if (f < 0) return NULL_COLOR;
 return Wheel(f % 384);
}

//// up/down variants to be replaced by mapping
struct CRGB rainbowCycle(long f, int pixelIndex) {
  if (f < 0) return NULL_COLOR;
  return Wheel((f + (pixelIndex % NUM_ROWS)*5) % 384);
}

struct CRGB colorAlternator(long f, int pixelIndex) {
  if (f < 0) return NULL_COLOR;
  if (f % 2 == 0) 
    return color1;
  return color2;
}

struct CRGB stripe(long f, int pixelIndex) {
  if (f < 0) return NULL_COLOR;
  int s = 4;
  if ((pixelIndex + f)/s % s < s/2)
    return color1;
  return color2;
}

struct CRGB bounce(long f, int pixelIndex) {
 if (f < 0) return NULL_COLOR;
 int r = 5;
 float b = triangleWave(f, r) + (pixelIndex+f)/(r*2);
 b = b - floor(b);
 return lerpColor(color1, color2, b);
}

// this guy's not quite the same.
struct CRGB colorChase(long f, int pixelIndex) {
 if (f < 0) return NULL_COLOR;
 int j = 9;
 if (pixelIndex%NUM_ROWS >= f%NUM_ROWS && pixelIndex%NUM_ROWS <= f%NUM_ROWS+j)
   return color1;
 return color2;
}

struct CRGB totesRandom(long f, int pixelIndex) {
 if (f < 0) return NULL_COLOR;
 if ((pixelIndex + f/10) % 2 == 0) 
   return (CRGB){(unsigned char)random(127), (unsigned char)random(127), (unsigned char)random(127)};
 return (CRGB){0, 0, 0};
}

struct CRGB gradient(long f, int pixelIndex) {
 if (f < 0) return NULL_COLOR;
 float r = NUM_ROWS;
 return lerpColor(color1, color2, ((pixelIndex + f) % (int)r) / r);
}

struct CRGB pulseSine(long f, int pixelIndex) {
  if (f == -2) return NULL_COLOR;
  if (f == -1) {
    params[0] = (sin(frame/25.0)+1)/2;
    return NULL_COLOR;
  }
  return lerpColor(color1, color2, params[0]);
}

struct CRGB pulseSaw(long f, int pixelIndex) {
 if (f < 0) return NULL_COLOR;
 float r = 25;
 return lerpColor(color1, color2, (f % (int)r / r));
}

struct CRGB colorWipeMeter(long f, int pixelIndex) {
 
 if (f == -2) return NULL_COLOR;
   
 if (f == -1) {
   params[0] = random(0,NUM_ROWS);
   return NULL_COLOR;
 }
 
 if (pixelIndex % NUM_ROWS < params[0]) 
   return color1;
 return color2;
 
}

struct CRGB colorWipeMeterGradient(long f, int pixelIndex) {
 
 if (f == -2) return NULL_COLOR;
 
 if (f == -1) {
   params[0] = random(NUM_ROWS);
   return NULL_COLOR;
 }

 if (pixelIndex % NUM_ROWS < params[0]) 
   return lerpColor(color1, color2, (sin(f/25.0)+1)/2);
 return color3;

}

struct CRGB flickerStrobeTwo(long f, int pixelIndex) {
 
 if (f == -2) return NULL_COLOR;
 
 if (f == -1) {
   // Select two random pixels.
   params[0] = random(NUM_PIXELS);
   params[1] = random(NUM_PIXELS);
   return NULL_COLOR;
 }
 
 if (pixelIndex == (int)params[0]) {
   return color1;
 } else if (pixelIndex == (int)params[1]) {
   return color2;
 } else {
   return color3;
 }
 
}

struct CRGB flickerStrobeFour(long f, int pixelIndex) {
 
 if (f == -2) return NULL_COLOR;
 
 if (f == -1) {
   // Select four random pixels.
   params[0] = random(NUM_PIXELS);
   params[1] = random(NUM_PIXELS);
   params[2] = random(NUM_PIXELS);
   params[3] = random(NUM_PIXELS);
   params[4] = random(NUM_PIXELS);
   params[5] = random(NUM_PIXELS);
   params[6] = random(NUM_PIXELS);
   params[7] = random(NUM_PIXELS);
   params[8] = random(NUM_PIXELS);
   params[9] = random(NUM_PIXELS);
   params[10] = random(NUM_PIXELS);
   params[11] = random(NUM_PIXELS);
   params[12] = random(NUM_PIXELS);
   params[13] = random(NUM_PIXELS);
   params[14] = random(NUM_PIXELS);
   params[15] = random(NUM_PIXELS);
   return NULL_COLOR;
 }
 
 if (pixelIndex == (int)params[0] || 
     pixelIndex == (int)params[1] || 
     pixelIndex == (int)params[2] || 
     pixelIndex == (int)params[3] || 
     pixelIndex == (int)params[4] || 
     pixelIndex == (int)params[5] || 
     pixelIndex == (int)params[6] || 
     pixelIndex == (int)params[7]) {
   return color1;
 } else if (pixelIndex == (int)params[8] || 
     pixelIndex == (int)params[9] || 
     pixelIndex == (int)params[10] || 
     pixelIndex == (int)params[11] || 
     pixelIndex == (int)params[12] || 
     pixelIndex == (int)params[13] || 
     pixelIndex == (int)params[14] || 
     pixelIndex == (int)params[15]) {
   return color2;
 } else {
   return color3;
 }
 
}

struct CRGB pulseOnce(long f, int pixelIndex) {
 
 if (f == -2) {
   params[0] = 1.0;
   return NULL_COLOR;
 }
 
 if (f == -1) {
   params[0] *= 0.9;
   return NULL_COLOR;
 }
 
 return lerpColor(color2, color1, params[0]);
 
}
