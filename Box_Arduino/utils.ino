uint8_t green(uint32_t c) {
  return (c >> 16) - 128;
}
uint8_t red(uint32_t c) {
  return (c >> 8) - 128;
}
uint8_t blue(uint32_t c) {
  return (c) - 128;
}
byte lerp(byte a, byte b, float t) {
  return a + (b - a)*t; 
}

float triangleWave(int frame, float rate) {
  if ((int)(frame / rate) % 2 == 0) {
    return (frame % (int)rate / rate);
  } 
  else { 
    return 1 - (frame % (int)rate / rate);
  }
}


uint32_t lerpColor(uint32_t a, uint32_t b, float t) {
  return Color(lerp(red(a), red(b), t), 
  lerp(green(a), green(b), t), 
  lerp(blue(a), blue(b), t));
}

uint32_t Wheel(uint16_t WheelPos) {

  WheelPos %= 384;
  byte r, g, b;
  switch(WheelPos / 128) {
  case 0:
    r = 127 - WheelPos % 128;		//Red down
    g = WheelPos % 128;			 // Green up
    b = 0;									//blue off
    break; 
  case 1:
    g = 127 - WheelPos % 128;	 //green down
    b = WheelPos % 128;			 //blue up
    r = 0;									//red off
    break; 
  case 2:
    b = 127 - WheelPos % 128;	 //blue down 
    r = WheelPos % 128;			 //red up
    g = 0;									//green off
    break; 
  }
  return(Color(r,g,b));
}

PROGMEM prog_uchar gammaTable[]  = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,
    4,  4,  4,  4,  5,  5,  5,  5,  5,  6,  6,  6,  6,  6,  7,  7,
    7,  7,  7,  8,  8,  8,  8,  9,  9,  9,  9, 10, 10, 10, 10, 11,
   11, 11, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 15, 15, 16, 16,
   16, 17, 17, 17, 18, 18, 18, 19, 19, 20, 20, 21, 21, 21, 22, 22,
   23, 23, 24, 24, 24, 25, 25, 26, 26, 27, 27, 28, 28, 29, 29, 30,
   30, 31, 32, 32, 33, 33, 34, 34, 35, 35, 36, 37, 37, 38, 38, 39,
   40, 40, 41, 41, 42, 43, 43, 44, 45, 45, 46, 47, 47, 48, 49, 50,
   50, 51, 52, 52, 53, 54, 55, 55, 56, 57, 58, 58, 59, 60, 61, 62,
   62, 63, 64, 65, 66, 67, 67, 68, 69, 70, 71, 72, 73, 74, 74, 75,
   76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91,
   92, 93, 94, 95, 96, 97, 98, 99,100,101,102,104,105,106,107,108,
  109,110,111,113,114,115,116,117,118,120,121,122,123,125,126,127
};

// This function (which actually gets 'inlined' anywhere it's called)
// exists so that gammaTable can reside out of the way down here in the
// utility code...didn't want that huge table distracting or intimidating
// folks before even getting into the real substance of the program, and
// the compiler permits forward references to functions but not data.
uint8_t gamma(uint8_t x) {
  return pgm_read_byte(&gammaTable[x*2]);
}


