uint32_t solidColor(long f, int pixelIndex) {
  if (f < 0) return -1;
  return color1;
}

uint32_t colorWipe(long f, int pixelIndex) {
  if (f < 0) return -1;
  if (pixelIndex < f % strip.numPixels())
    return color1;
  return color2;
}

uint32_t rainbow(long f, int pixelIndex) {

  if (f < 0) return -1;
  return Wheel(f % 384);
}

// up/down variants to be replaced by mapping
uint32_t rainbowCycle(long f, int pixelIndex) {
  if (f < 0) return -1;
  int j = f % 384 * 5;			
  return Wheel((pixelIndex * 384 / strip.numPixels() + j));
}

uint32_t colorAlternator(long f, int pixelIndex) {
  if (f < 0) return -1;
  int s = 5;
  if (f % 2 == 0) 
    return color1;
  return color2;
}

uint32_t stripe(long f, int pixelIndex) {
  if (f < 0) return -1;
  int s = 4;
  if ((pixelIndex + f)/s % s < s/2)
    return color1;
  return color2;
}

uint32_t bounce(long f, int pixelIndex) {
  if (f < 0) return -1;
  int r = 5;
  float b = triangleWave(f, r) + (pixelIndex+f)/(r*2);
  b = b - floor(b);
  return lerpColor(color1, color2, b);
}
// this guy's not quite the same.
uint32_t colorChase(long f, int pixelIndex) {
  if (f < 0) return -1;
  int j = 9;
  if (pixelIndex >= f%strip.numPixels() && pixelIndex <= f%strip.numPixels()+j)
    return color1;
  return color2;
}

uint32_t totesRandom(long f, int pixelIndex) {
  if (f < 0) return -1;
  if ((pixelIndex + f/10) % 2 == 0) 
    return strip.Color(random(127), random(127), random(127));
  return strip.Color(0, 0, 0);
}

uint32_t gradient(long f, int pixelIndex) {
  if (f < 0) return -1;
  float r = 25;
  return lerpColor(color1, color2, ((pixelIndex + f) % (int)r) / r);
}

uint32_t pulseSine(long f, int pixelIndex) {
  if (f < 0) return -1;
  float r = 25;
  return lerpColor(color1, color2, (sin(f/r)+1)/2);
}

uint32_t pulseSaw(long f, int pixelIndex) {
  if (f < 0) return -1;
  float r = 25;
  return lerpColor(color1, color2, (f % (int)r / r));
}



uint32_t colorWipeMeter(long f, int pixelIndex) {
  
  if (f == -2) return -1;
    
  if (f == -1) {
    params[0] = random(0,strip.numPixels());
    return -1;
  }
  
  if (pixelIndex < params[0]) 
    return color1;
  return color2;
  
}

uint32_t colorWipeMeterGradient(long f, int pixelIndex) {
  
  if (f == -2) return -1;
  
  if (f == -1) {
    params[0] = random(strip.numPixels());
    return -1;
  }

  if (pixelIndex < params[0]) 
    return lerpColor(color1, color2, (sin(f/25.0)+1)/2);
  return color3;

}

uint32_t flickerStrobeTwo(long f, int pixelIndex) {
  
  if (f == -2) return -1;
  
  if (f == -1) {
    // Select two random pixels.
    params[0] = random(strip.numPixels());
    params[1] = random(strip.numPixels());
    return -1;
  }
  
  if (pixelIndex == (int)params[0]) {
    return color1;
  } else if (pixelIndex == (int)params[1]) {
    return color2;
  } else {
    return color3;
  }
  
}

uint32_t flickerStrobeFour(long f, int pixelIndex) {
  
  if (f == -2) return -1;
  
  if (f == -1) {
    // Select four random pixels.
    params[0] = random(strip.numPixels());
    params[1] = random(strip.numPixels());
    params[2] = random(strip.numPixels());
    params[3] = random(strip.numPixels());
    return -1;
  }
  
  if (pixelIndex == (int)params[0] || 
      pixelIndex == (int)params[1]) {
    return color1;
  } else if (pixelIndex == (int)params[2] || 
             pixelIndex == (int)params[3]) {
    return color2;
  } else {
    return color3;
  }
  
}

uint32_t pulseOnce(long f, int pixelIndex) {
  
  if (f == -2) {
    params[0] = 1.0;
    return -1;
  }
  
  if (f == -1) {
    params[0] *= 0.9;
    return -1;
  }
  
  return lerpColor(color2, color1, params[0]);
  
}
