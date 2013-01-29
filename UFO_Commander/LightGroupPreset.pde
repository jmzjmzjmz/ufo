class LightGroupSettings { 

  public final color color1, color2;
  public final int rate;
  public final int pattern;
  public final int mapping;
  public final int brightness;

  public LightGroupSettings(int pattern, int mapping, int rate, int brightness, color color1, color color2) {
    this.pattern = pattern;
    this.mapping = mapping;
    this.rate = rate;
    this.brightness = brightness;
    this.color1 = color1;
    this.color2 = color2;
  }

  public String toString() {

    return "Pattern: " + patterns[pattern] + "\n" + 
           "Mapping: " + mappings[mapping] + "\n" + 
           "Rate: " + rate + "\n" + 
           "Brightness: " + brightness + "\n" + 
           "Color1: " + color1 + "\n" + 
           "Color2: " + color2;

  }

  // public int[] serialize() {

  //   int[] data = new int[2];

  // }

}