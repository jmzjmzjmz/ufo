import controlP5.*;

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

void setup() {
  
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

void draw() {
 
  background(0);

}


boolean sympathizeEvents = false;

void controlEvent(ControlEvent theEvent) {

  expressSympathy(theEvent);
  if (checkLightControllers(theEvent)) return;

  // todo preset box

}

void expressSympathy(ControlEvent theEvent) {

  // todo controllerGroups.

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
boolean checkLightControllers(ControlEvent theEvent) {

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
      l.color1 = (color)theEvent.value();
      return true;
    }

    if (theEvent.isFrom(l.colorPicker2)) {
      l.color2 = (color)theEvent.value();
      return true;
    }

  }

  return false;

}