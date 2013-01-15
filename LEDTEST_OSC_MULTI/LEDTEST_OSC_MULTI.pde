import processing.serial.*; 
Serial port; 
import oscP5.*;
import netP5.*;

boolean DEBUG = false;

int redVal1 = 0, greenVal1 = 0, blueVal1 = 0, redVal2 = 0, greenVal2 = 0, blueVal2 = 0;

OscP5 oscP5;
NetAddress myRemoteLocation;
NetAddress myCompLocation;

String iPadIP = "192.168.1.14";
int iPadPort = 12005;
String compIP = "192.168.1.10";
int compPort = 12001;

public int headbandSelect=1;

String[] myPatterns = new String[81];

import controlP5.*;
ControlP5 controlP5;

Radio r2;

Headband[] headbands = new Headband[3];

void setup() {
  size(400, 650);
  background(0);
  println(Serial.list());
  
  port = new Serial(this, Serial.list()[4], 9600); 
  controlP5 = new ControlP5(this);

  /* start oscP5, listening for incoming messages at port 12005 */
  oscP5 = new OscP5(this, compPort);

  /* myRemoteLocation is a NetAddress. a NetAddress takes 2 parameters,
   * an ip address and a port number. myRemoteLocation is used as parameter in
   * oscP5.send() when sending osc packets to another computer, device, 
   * application. usage see below. for testing purposes the listening port
   * and the port of the remote location address are the same, hence you will
   * send messages back to this sketch.
   */
  myRemoteLocation = new NetAddress(iPadIP, iPadPort);
  myCompLocation = new NetAddress(compIP, compPort);

  r2 = controlP5.addRadio("whichLights", 5, 270);
  r2.add("vertical1", 0);
  r2.add("vertical2", 1);
  r2.add("vertical3", 2);
  r2.add("vertical4", 3);
  r2.add("vertical5", 4);
  r2.add("vertical6", 5);
  r2.add("horizontal1", 6);
  r2.add("horizontal2", 7);
  r2.add("horizontal3", 8);
  r2.add("horizontal4", 9);
  r2.add("horizontal5", 10);
  r2.add("horizontal6", 11);
  r2.add("box1", 12);
  r2.add("box2", 13);
  r2.add("box3", 14);
  r2.add("dmx", 15);

  Radio r = controlP5.addRadio("radio", 270, 5);
  r.deactivateAll(); // use deactiveAll to not make the first radio button active.
  r.add("pulseOnce", 80);
  r.add("colorWipeMeterGradient", 79);
  r.add("colorWipeMeter", 78);
  r.add("colorChase", 77);
  r.add("stripe", 76);
  r.add("colorAlternator   & DMX", 75);
  r.add("colorWipe", 74);
  r.add("bounce", 73);
  r.add("pulseSaw", 72);
  r.add("pulseSine         & DMX", 71);
  r.add("gradient", 70);
  r.add("solidColor        & DMX", 69);
  r.add("** OFF", 68);
  r.add("rainbow          & DMX", 66);
  r.add("rainbowCycle     & DMX", 65);
  r.add("totesRandom", 64);
  r.add("flickerStrobeFour", 63);
  r.add("flickerStrobeTwo & DMX", 62);
  r.add("** FORWARD", 1);
  r.add("** BACKWARD", 2);
  r.add("** PEAK", 3);
  r.add("** VALLEY", 4);
  r.add("** DITHER", 5);
//  r.add("** SNAKE64", 6);

  r.add("*** SYNC CLOCKS", 7);

  controlP5.addSlider("speed", 0, 127, 5, 200, 220, 20);
  controlP5.addSlider("dimVal", 0, 127, 5, 225, 220, 20);

  //initialize all array values so no errors
  for (int i = 0; i < myPatterns.length; i++) {
    myPatterns[i] = " ";
  }
  //Assign pattern array OSC locations
  myPatterns[80] = "/1/multitoggle1/1/1";
  myPatterns[79] = "/1/multitoggle1/2/1";
  myPatterns[78] = "/1/multitoggle1/1/2";
  myPatterns[77] = "/1/multitoggle1/2/2";
  myPatterns[76] = "/1/multitoggle1/1/3";
  myPatterns[75] = "/1/multitoggle1/2/3";
  myPatterns[74] = "/1/multitoggle1/1/4";
  myPatterns[73] = "/1/multitoggle1/2/4";
  myPatterns[72] = "/1/multitoggle1/1/5";
  myPatterns[71] = "/1/multitoggle1/2/5";
  myPatterns[70] = "/1/multitoggle1/1/6";
  myPatterns[69] = "/1/multitoggle1/2/6";
  myPatterns[68] = "/1/multitoggle1/2/9";
  myPatterns[66] = "/1/multitoggle1/1/7";
  myPatterns[65] = "/1/multitoggle1/2/7";
  myPatterns[64] = "/1/multitoggle1/1/8";
  myPatterns[63] = "/1/multitoggle1/2/8";
  myPatterns[62] = "/1/multitoggle1/1/9";
  myPatterns[1] = "/1/multitoggle2/1/1";
  myPatterns[2] = "/1/multitoggle2/2/1";
  myPatterns[3] = "/1/multitoggle2/1/2";
  myPatterns[4] = "/1/multitoggle2/2/2";
  myPatterns[5] = "/1/multitoggle2/1/3";


  headbands[0] = new Headband(0x5678);
  headbands[2] = new Headband(0x6789);
  headbands[1] = new Headband(0xFFFF);

  headbands[headbandSelect].pattern = 65;
  headbands[headbandSelect].rate = 20;
  headbands[headbandSelect].send();
}


int lastSend = -1;
final int INTERVAL = 1;
final int TIMING_ADDR = 100;
void draw() {
  
  int now = millis()/1000;
  
  if (now - lastSend >= INTERVAL) {
    
//    println("time " + now);
    
    port.write(TIMING_ADDR);
    port.write(now + ",");
    for (int i = 0; i < MESSAGE_SIZE - 2; i++) {
      port.write(0);
    }

    lastSend = now;
    
  }
  println(now);
}

void mouseMoved() {
  headbands[headbandSelect].rate = int(map(mouseX, 0, width, 0, 255));
//  headbands[headbandSelect].send();
}

void keyPressed() {
  headbands[headbandSelect].send();
}

void radio(int theId) {

  if (theId < 0) return;
  headbands[headbandSelect].pattern = (byte)theId;
  headbands[headbandSelect].send();
  
}  

void whichLights(int theId) {
  WHICH_LIGHTS = theId;
}  


/* incoming osc message are forwarded to the oscEvent method. */
void oscEvent(OscMessage theOscMessage) {

  //  String message = theOscMessage.addrPattern();
  //  
  //  if(message.indexOf("z") == message.length() - 1){
  //   return; 
  //  }

  /* print the address pattern and the typetag of the received OscMessage */

  if (DEBUG) {
    println("### received an osc message.");
    println("### addrpattern\t"+theOscMessage.addrPattern());
    println(theOscMessage.get(0).floatValue());
  }


  if (theOscMessage.checkAddrPattern("/1/masterFader")) {
    headbands[headbandSelect].adjustBrightness((int)theOscMessage.get(0).floatValue());
    headbands[headbandSelect].send();
    return;
  }
  if (theOscMessage.checkAddrPattern("/1/rateFader")) {
    headbands[headbandSelect].adjustRate((int)theOscMessage.get(0).floatValue());
    headbands[headbandSelect].send();
    return;
  }
  if (theOscMessage.checkAddrPattern("/1/fader1")) {
    headbandSelect = (int)theOscMessage.get(0).floatValue();
    return;
  }

  //IF COLOR FADER:
  if (theOscMessage.checkAddrPattern("/1/colorFader1") || 
    theOscMessage.checkAddrPattern("/1/colorFader2") || 
    theOscMessage.checkAddrPattern("/1/colorFader3") || 
    theOscMessage.checkAddrPattern("/1/colorFader4") || 
    theOscMessage.checkAddrPattern("/1/colorFader5") || 
    theOscMessage.checkAddrPattern("/1/colorFader6")) {

    if (theOscMessage.checkAddrPattern("/1/colorFader1")) {
      redVal1 = (int) theOscMessage.get(0).floatValue();
    }
    if (theOscMessage.checkAddrPattern("/1/colorFader2")) {
      greenVal1 = (int) theOscMessage.get(0).floatValue();
    }
    if (theOscMessage.checkAddrPattern("/1/colorFader3")) {
      blueVal1 = (int)theOscMessage.get(0).floatValue();
    }
    if (theOscMessage.checkAddrPattern("/1/colorFader4")) {
      redVal2 = (int) theOscMessage.get(0).floatValue();
    }
    if (theOscMessage.checkAddrPattern("/1/colorFader5")) {
      greenVal2 = (int) theOscMessage.get(0).floatValue();
    }
    if (theOscMessage.checkAddrPattern("/1/colorFader6")) {
      blueVal2 = (int) theOscMessage.get(0).floatValue();
    }

    headbands[headbandSelect].setColor(0, redVal1, greenVal1, blueVal1);
    headbands[headbandSelect].setColor(1, redVal2, greenVal2, blueVal2); 
    headbands[headbandSelect].send();
    return;
  }

  for (int i = 1; i < 6; i++) {
    if (theOscMessage.checkAddrPattern(myPatterns[i]) 
      && theOscMessage.get(0).floatValue()==1.0) {
      headbands[headbandSelect].pattern = i;
      headbands[headbandSelect].send();
      return;
    }
  }

  if (theOscMessage.checkAddrPattern("/1/push1") 
    && theOscMessage.get(0).floatValue()==1.0) {
    swapColors();
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/push2") 
    && theOscMessage.get(0).floatValue()==1.0) {
    headbands[headbandSelect].pattern = 80;
    headbands[headbandSelect].send();
    return;
  }

  for (int i = 62; i < 81; i++) {
    if (theOscMessage.checkAddrPattern(myPatterns[i]) 
      && theOscMessage.get(0).floatValue()==1.0) {
      if (i == 68) {
        OSCmaster(0);
        headbands[headbandSelect].send();
        return;
      }
      headbands[headbandSelect].pattern = i;
      headbands[headbandSelect].send();
      return;
    }
  }



  /////////PRESETS/////////

  if (theOscMessage.checkAddrPattern("/1/push3") 
    && theOscMessage.get(0).floatValue()==1.0) {
    //PULSE BLUE TO RED
    OSCrate(50);
    OSCmaster(127);//FULL BRIGHTNESS
    OSCcolors(0, 0, 255, 255, 0, 0);
    //make sure to set mapping too
    OSCpattern(1);
    OSCpattern(80);
    return;
  }
  if (theOscMessage.checkAddrPattern("/1/push4") 
    && theOscMessage.get(0).floatValue()==1.0) {
    //PULSE GREEN TO WHITE
    OSCrate(50);
    OSCmaster(127);//FULL BRIGHTNESS
    OSCcolors(0, 255, 0, 255, 255, 255);
    //make sure to set mapping too
    OSCpattern(1);
    OSCpattern(80);
    return;
  }

  //    if (theOscMessage.checkAddrPattern("/1/presets/1/1") 
  //&& theOscMessage.get(0).floatValue()==1.0) {
  ////Pulse blue to red
  //  OSCrate(50);
  //  OSCmaster(127);//FULL BRIGHTNESS
  //  OSCcolors(0,0,255,255,0,0);
  //   //make sure to set mapping too
  //   OSCpattern(1);
  //   OSCpattern(80);
  //   return;
  //}
  if (theOscMessage.checkAddrPattern("/1/presets/1/2") 
    && theOscMessage.get(0).floatValue()==1.0) {
    //RAINBOW FAST
    OSCrate(0);
    OSCmaster(127);//FULL BRIGHTNESS
    //make sure to set mapping too
    OSCpattern(1);
    OSCpattern(65);
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/1/3") 
    && theOscMessage.get(0).floatValue()==1.0) {
    //RAINBOW SLOW DIM
    OSCrate(127);
    OSCmaster(15);//FULL BRIGHTNESS
    //make sure to set mapping too
    OSCpattern(1);
    OSCpattern(65);
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/1/4") 
    && theOscMessage.get(0).floatValue()==1.0) {
    //RAINBOW PEAK
    OSCrate(0);
    OSCmaster(127);//FULL BRIGHTNESS
    //make sure to set mapping too
    OSCpattern(3);
    OSCpattern(65);
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/1/5") 
    && theOscMessage.get(0).floatValue()==1.0) {
    //RAINBOW DITHER
    OSCrate(0);
    OSCmaster(127);//FULL BRIGHTNESS
    //make sure to set mapping too
    OSCpattern(5);
    OSCpattern(65);
    return;
  }

  //if (theOscMessage.checkAddrPattern("/1/presets/2/1") 
  //&& theOscMessage.get(0).floatValue()==1.0) {
  //
  //}

  if (theOscMessage.checkAddrPattern("/1/presets/2/2") 
    && theOscMessage.get(0).floatValue()==1.0) {
    //EASTER PASTELS
    OSCrate(0);
    OSCmaster(127);//FULL BRIGHTNESS
    //make sure to set mapping too
    OSCpattern(1);
    OSCpattern(64);
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/2/3") 
    && theOscMessage.get(0).floatValue()==1.0) {
    //SINE FADE BLUE TO PINK DIM
    OSCrate(100);
    OSCmaster(13);//DIM
    OSCcolors(255, 0, 70, 0, 0, 255);
    //make sure to set mapping too
    OSCpattern(1);
    OSCpattern(71);
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/2/4") 
    && theOscMessage.get(0).floatValue()==1.0) {
    //FLICKER FOUR ALL WHITE FAST 
    OSCrate(0);
    OSCmaster(127);//FULL BRIGHTNESS
    OSCcolors(255, 255, 255, 255, 255, 255);
    //make sure to set mapping too
    OSCpattern(1);
    OSCpattern(63);
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/2/5") 
    && theOscMessage.get(0).floatValue()==1.0) {
    //RAINBOW CYCLE
    OSCrate(0);
    OSCmaster(127);//FULL BRIGHTNESS
    //make sure to set mapping too
    OSCpattern(1);
    OSCpattern(66);
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/3/1") 
    && theOscMessage.get(0).floatValue()==1.0) {
    //BLUE PINK RAIN
    OSCrate(0);
    OSCmaster(127);//FULL BRIGHTNESS
    OSCcolors(0, 0, 255, 255, 0, 0);
    //make sure to set mapping too
    OSCpattern(1);
    OSCpattern(70);
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/3/2") 
    && theOscMessage.get(0).floatValue()==1.0) {
    //YELLOW GREEN RAIN
    OSCrate(0);
    OSCmaster(127);//FULL BRIGHTNESS
    OSCcolors(255, 255, 0, 0, 255, 0);
    //make sure to set mapping too
    OSCpattern(1);
    OSCpattern(70);
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/3/3") 
    && theOscMessage.get(0).floatValue()==1.0) {
    //BLUE TURQUOISE STRIPES UP
    OSCrate(0);
    OSCmaster(127);//FULL BRIGHTNESS
    OSCcolors(0, 0, 255, 0, 255, 118);
    //make sure to set mapping too
    OSCpattern(2);
    OSCpattern(76);
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/3/4") 
    && theOscMessage.get(0).floatValue()==1.0) {
    //RED GAP CHASE
    OSCrate(0);
    OSCmaster(127);//FULL BRIGHTNESS
    OSCcolors(0, 0, 0, 255, 0, 0);
    //make sure to set mapping too
    OSCpattern(1);
    OSCpattern(77);
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/3/5") 
    && theOscMessage.get(0).floatValue()==1.0) {
    //RED/GREEN STROBE
    OSCrate(0);
    OSCmaster(127);//FULL BRIGHTNESS
    OSCcolors(255, 0, 0, 0, 255, 0);
    //make sure to set mapping too
    OSCpattern(1);
    OSCpattern(75);
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/4/1") 
    && theOscMessage.get(0).floatValue()==1.0) {
    //VU METER turquoise/orange
    OSCrate(15);
    OSCmaster(127);//FULL BRIGHTNESS
    OSCcolors(0, 255, 75, 255, 75, 0);
    //make sure to set mapping too
    OSCpattern(1);
    OSCpattern(78);
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/4/2") 
    && theOscMessage.get(0).floatValue()==1.0) {
    //WHITE RAIN
    OSCrate(0);
    OSCmaster(127);//FULL BRIGHTNESS
    OSCcolors(255, 255, 255, 0, 0, 0);
    //make sure to set mapping too
    OSCpattern(1);
    OSCpattern(70);
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/4/3") 
    && theOscMessage.get(0).floatValue()==1.0) {
    //PURPLE BLUE STRIPES
    OSCrate(0);
    OSCmaster(127);//FULL BRIGHTNESS
    OSCcolors(255, 0, 255, 0, 255, 255);
    //make sure to set mapping too
    OSCpattern(3);
    OSCpattern(76);
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/4/4") 
    && theOscMessage.get(0).floatValue()==1.0) {
    //BOUNCE
    OSCrate(30);
    OSCmaster(127);//FULL BRIGHTNESS
    OSCcolors(211, 0, 255, 0, 255, 0);
    //make sure to set mapping too
    OSCpattern(1);
    OSCpattern(73);
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/4/5") 
    && theOscMessage.get(0).floatValue()==1.0) {
    //TEETH PEAK METER
    OSCrate(50);
    OSCmaster(127);//FULL BRIGHTNESS
    OSCcolors(255, 255, 255, 0, 0, 0);
    //make sure to set mapping too
    OSCpattern(3);
    OSCpattern(78);
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/5/1") 
    && theOscMessage.get(0).floatValue()==1.0) {
    //CHASE INVERTED
    OSCrate(9);
    OSCmaster(127);//FULL BRIGHTNESS
    OSCcolors(255, 0, 0, 0, 0, 20);
    //make sure to set mapping too
    OSCpattern(3);
    OSCpattern(77);
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/5/2") 
    && theOscMessage.get(0).floatValue()==1.0) {
    //XMAS COLORS
    OSCmaster(127);//FULL BRIGHTNESS
    OSCcolors(255, 0, 0, 0, 255, 0);
    headbands[headbandSelect].send();
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/5/3") 
    && theOscMessage.get(0).floatValue()==1.0) {
    //SINE FADE GREEN TO PURPLE
    OSCrate(0);
    OSCmaster(127);//FULL BRIGHTNESS
    OSCcolors(0, 255, 0, 211, 0, 255);
    //make sure to set mapping too
    OSCpattern(1);
    OSCpattern(71);
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/5/4") 
    && theOscMessage.get(0).floatValue()==1.0) {
    //SINE FADE RED TO BLUE
    OSCrate(0);
    OSCmaster(127);//FULL BRIGHTNESS
    OSCcolors(255, 0, 0, 0, 0, 255);
    //make sure to set mapping too
    OSCpattern(1);
    OSCpattern(71);
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/5/5") 
    && theOscMessage.get(0).floatValue()==1.0) {
    //STROBE WHITE
    OSCrate(0);
    OSCmaster(127);//FULL BRIGHTNESS
    OSCcolors(255, 255, 255, 0, 0, 0);
    //make sure to set mapping too
    OSCpattern(1);
    OSCpattern(75);
    return;
  }
}


//OSC helper functions
void OSCpattern(int patternNum) {
  OscMessage patternMessage = new OscMessage(myPatterns[patternNum]);
  patternMessage.add(1.0); /* toggle on */
  oscP5.send(patternMessage, myRemoteLocation);
  oscP5.send(patternMessage, myCompLocation);
}

void OSCrate(int rateValue) {
  OscMessage rateMessage = new OscMessage("/1/rateFader");
  rateMessage.add(rateValue); /* add an int to the osc message */
  oscP5.send(rateMessage, myRemoteLocation);
  headbands[headbandSelect].rate= rateValue;
}

void OSCmaster(int masterValue) {
  OscMessage masterMessage = new OscMessage("/1/masterFader");
  masterMessage.add(masterValue); /* add an int to the osc message */
  oscP5.send(masterMessage, myRemoteLocation);
  headbands[headbandSelect].dim = masterValue;
}

void OSCcolors(int r1, int g1, int b1, int r2, int g2, int b2) {
  OscMessage redMessage = new OscMessage("/1/colorFader1");
  redMessage.add(map(r1, 0, 127, 0, 255)); /* add an int to the osc message */
  oscP5.send(redMessage, myRemoteLocation);
  OscMessage greenMessage = new OscMessage("/1/colorFader2");
  greenMessage.add(map(g1, 0, 127, 0, 255)); /* add an int to the osc message */
  oscP5.send(greenMessage, myRemoteLocation);
  OscMessage blueMessage = new OscMessage("/1/colorFader3");
  blueMessage.add(map(b1, 0, 127, 0, 255)); /* add an int to the osc message */
  oscP5.send(blueMessage, myRemoteLocation);

  OscMessage red2Message = new OscMessage("/1/colorFader4");
  red2Message.add(map(r2, 0, 127, 0, 255)); /* add an int to the osc message */
  oscP5.send(red2Message, myRemoteLocation);
  OscMessage green2Message = new OscMessage("/1/colorFader5");
  green2Message.add(map(g2, 0, 127, 0, 255)); /* add an int to the osc message */
  oscP5.send(green2Message, myRemoteLocation);
  OscMessage blue2Message = new OscMessage("/1/colorFader6");
  blue2Message.add(map(b2, 0, 127, 0, 255)); /* add an int to the osc message */
  oscP5.send(blue2Message, myRemoteLocation);

  redVal1 = r1;
  greenVal1 = g1;
  blueVal1 = b1;
  redVal2 = r2;
  greenVal2 = g2;
  blueVal2 = b2;

  headbands[headbandSelect].setColor(0, r1, g1, b1);
  headbands[headbandSelect].setColor(1, r2, g2, b2);
}

void swapColors() {
  OSCcolors(redVal2, greenVal2, blueVal2, redVal1, greenVal1, blueVal1); 
  headbands[headbandSelect].send();
}

