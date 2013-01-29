import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import controlP5.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class UFO_Commander extends PApplet {



final int MESSAGE_SIZE = 14;
final int TIMING_ADDR = 129;
final int OFF_PATTERN = 68;
final int INITIAL_PATTERN = 80;
final int DELIMETER = 128;
final int INTERVAL = 1000;

int lastSend;

ArrayList lightGroups = new ArrayList();

LightGroup groupVerticalPoles;
LightGroup groupHorizontalPoles;
LightGroup groupBoxes;
LightGroup groupDMX;
LightGroup groupAll;

final int LIGHT_GROUP_WIDTH = 270;
final int RADIO_SIZE = 20;
final int PADDING = 15;

ControlP5 controlP5;

String[] patterns = new String[127];
String[] mappings = new String[6];

ListBox presetList;

public void setup() {
  
  size(1440, 631);

  patterns[80] = "pulseOnce";
  patterns[79] = "colorWipeMeterGradient";
  patterns[78] = "colorWipeMeter";
  patterns[77] = "colorChase";
  patterns[76] = "stripe";
  patterns[75] = "colorAlternator";
  patterns[74] = "colorWipe";
  patterns[73] = "bounce";
  patterns[72] = "pulseSaw";
  patterns[71] = "pulseSine";
  patterns[70] = "gradient";
  patterns[69] = "solidColor";
  patterns[OFF_PATTERN] = "off";
  patterns[66] = "rainbow";
  patterns[65] = "rainbowCycle";
  patterns[64] = "totesRandom";
  patterns[63] = "flickerStrobeFour";
  patterns[62] = "flickerStrobeTwo";

  mappings[1] = "forward";
  mappings[2] = "backward";
  mappings[3] = "peak";
  mappings[4] = "valley";
  mappings[5] = "dither";
  
  controlP5 = new ControlP5(this);

  groupVerticalPoles = new LightGroup("Vertical Poles", 10);
  groupHorizontalPoles = new LightGroup("Horizontal Poles", 11);
  groupBoxes = new LightGroup("Boxes", 12);
  groupDMX = new LightGroup("DMX", 13);

//  presetList = controlP5.addListBox("presets")
//                        .setPosition(lightGroups.size() * LIGHT_GROUP_WIDTH, PADDING);

  println(controlP5.getAll());

}

public void draw() {
 

  background(0);

}


boolean sympathizeEvents = false;

public void controlEvent(ControlEvent theEvent) {

  expressSympathy(theEvent);
  if (checkLightControllers(theEvent)) return;


}

public void expressSympathy(ControlEvent theEvent) {

  if (keyPressed && !sympathizeEvents) {
    
    sympathizeEvents = true;

    String name = theEvent.name();
    name = name.substring(0, name.indexOf("-"));

    for (Object o : lightGroups) {
      LightGroup l = (LightGroup)o;
      Controller c = controlP5.getController(name + "-" + l.address);
      if (c != null) {
        c.setValue(theEvent.value());
      }
    }

    sympathizeEvents = false;

  }

}

/**
 * Returns true if the event was from one of the Light's controllers.
 */
public boolean checkLightControllers(ControlEvent theEvent) {

  for (Object o : lightGroups) {

    LightGroup l = (LightGroup)o;

    if (theEvent.isFrom(l.rateSlider)) {
      l.rate = (int)theEvent.value();
      return true;
    }

    if (theEvent.isFrom(l.brightnessSlider)) {
      l.brightness = (int)theEvent.value();
      return true;
    }

    if (theEvent.isFrom(l.patternList)) {
      l.pattern = (int)theEvent.value();
      return true;
    }

    if (theEvent.isFrom(l.mappingList)) {
      l.mapping = (int)theEvent.value();
      return true;
    }

    if (theEvent.isFrom(l.colorPicker1)) {
      l.color1 = (int)theEvent.value();
      return true;
    }

    if (theEvent.isFrom(l.colorPicker2)) {
      l.color2 = (int)theEvent.value();
      return true;
    }

  }

  return false;

}
class LightGroup {

  public final int address;

  // These are only public so p5 can manipulate them.
  // Don't change!
  public int pattern = OFF_PATTERN;
  public int mapping;
  public int rate;
  public int brightness;

  // Would have liked to make these arrays, but controlP5's reflection makes that harder.
  // Also addListener didn't work for color
  public int color1, color2;
  
  // Interface elements
  public final ColorPicker colorPicker1, colorPicker2;
  public final RadioButton patternList;
  public final RadioButton mappingList;
  public final Slider rateSlider;
  public final Slider brightnessSlider;

  // Just the order in which we created this group
  protected final int index;

  public LightGroup(String title, int address) {

    index = lightGroups.size();
    lightGroups.add(this);
    this.address = address;

    // Build interface

    int x = index * LIGHT_GROUP_WIDTH + PADDING;
    int y = PADDING;

    controlP5.addTextlabel("label"+address)
             .setText(title)
             .setPosition(x - 4, y);


    y += PADDING;

    
    colorPicker1 = controlP5.addColorPicker("picker1-" + address)
                            .setPosition(x, y);
    y += 70;

    colorPicker2 = controlP5.addColorPicker("picker2-" + address)
                            .setPosition(x, y);
    y += 70;

    rateSlider = controlP5.addSlider("rate-"+address)
             .setPosition(x, y)
             .setRange(0, 127)
             .setSize(220, 20)
             .setDecimalPrecision(0)
             .setLabel("rate")
             .setValue(0);

    y += PADDING * 2;

    brightnessSlider = controlP5.addSlider("brightness-"+address)
             .setPosition(x, y)
             .setRange(0, 127)
             .setSize(220, 20)
             .setDecimalPrecision(0)
             .setLabel("bright")
             .setValue(127);

    y += PADDING * 2;

    
    patternList = controlP5.addRadioButton("patterns-" + address)
                           .setPosition(x , y)
                           .setSize(RADIO_SIZE, RADIO_SIZE)
                           .setNoneSelectedAllowed(false);

    int j = 0;
    for (int i = 0; i < patterns.length; i++) {
      if (patterns[i] != null) {
        patternList.addItem(patterns[i] + address, i);
        patternList.getItem(j).setCaptionLabel(patterns[i]);
        if (i == INITIAL_PATTERN) {
          patternList.activate(patternList.getItems().size()-1);
        }
        // y += RADIO_SIZE;
        j++;
      }
    }

    // y += PADDING*2;

    mappingList = controlP5.addRadioButton("mappings-" + address)
                           .setPosition(x +150, y)
                           .setSize(RADIO_SIZE, RADIO_SIZE)
                           .setNoneSelectedAllowed(false);

    j = 0;
    for (int i = 0; i < mappings.length; i++) {
      if (mappings[i] != null) {
        mappingList.addItem(mappings[i] + address, i);
        mappingList.getItem(j).setCaptionLabel(mappings[i]);
        j++;
      }
    }

    mappingList.activate(0);


  }

  public LightGroupSettings getSettings() {

    return new LightGroupSettings(pattern, 
      mapping, rate, brightness, color1, color2);

  }

}
class LightGroupSettings { 

  public final int color1, color2;
  public final int rate;
  public final int pattern;
  public final int mapping;
  public final int brightness;

  public LightGroupSettings(int pattern, int mapping, int rate, int brightness, int color1, int color2) {
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

}
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "UFO_Commander" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
