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

  public LightGroupSettings(String serialized) {
    String[] data = split(serialized, ",");
    this.pattern = int(data[0]);
    this.mapping = int(data[1]);
    this.rate = int(data[2]);
    this.brightness = int(data[3]);
    this.color1 = int(data[4]);
    this.color2 = int(data[5]);
  }

  public String toString() {

    return "Pattern: " + patterns[pattern] + "\n" + 
           "Mapping: " + mappings[mapping] + "\n" + 
           "Rate: " + rate + "\n" + 
           "Brightness: " + brightness + "\n" + 
           "Color1: " + color1 + "\n" + 
           "Color2: " + color2;

  }

  public String serialize() {
    return pattern + "," + mapping + "," + rate + "," + brightness + "," + color1 + "," + color2;
  }


}