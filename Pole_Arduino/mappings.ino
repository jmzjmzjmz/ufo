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

