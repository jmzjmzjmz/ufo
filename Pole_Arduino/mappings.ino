int forward(long frame, int i) {
  return i;
}

int backward(long frame, int i) {
  return strip.numPixels() - 1 - i;
}

int peak(long frame, int i) {
  if (i < strip.numPixels() / 2) {
    return i;
  } else { 
    return strip.numPixels() - 1 - i;
  }
}

int valley(long frame, int i) {
  if (i < strip.numPixels() / 2) {
    return strip.numPixels() / 2 - 1 - i;
  } else { 
    return i;
  }
}

int dither(long frame, int i) {
  if (i % 2 == 0) {
    return strip.numPixels() - 1 - i;
  } else { 
    return i;
  }
}





int box(long frame, int i) {
  
  int len = 64;

  // Is this an odd column?
  if (i % (len*2) >= len) {

    // How many odd colums so far?
    int o = i / (len*2);

    // Fake index and real index add up to this number
    int s = (len*3-1) + o*(len*4);

    return s - i;

  }

  return i;

}