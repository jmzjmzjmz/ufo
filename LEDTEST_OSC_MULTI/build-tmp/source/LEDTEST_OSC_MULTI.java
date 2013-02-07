import processing.core.*; 
import processing.data.*; 
import processing.opengl.*; 

import processing.serial.*; 
import java.util.LinkedList; 
import oscP5.*; 
import netP5.*; 
import controlP5.*; 

import controlP5.*; 
import themidibus.*; 
import promidi.*; 

import java.applet.*; 
import java.awt.Dimension; 
import java.awt.Frame; 
import java.awt.event.MouseEvent; 
import java.awt.event.KeyEvent; 
import java.awt.event.FocusEvent; 
import java.awt.Image; 
import java.io.*; 
import java.net.*; 
import java.text.*; 
import java.util.*; 
import java.util.zip.*; 
import java.util.regex.*; 

public class LEDTEST_OSC_MULTI extends PApplet {

 




Serial port; 

boolean DEBUG = true;

int redVal1 = 0, greenVal1 = 0, blueVal1 = 0, redVal2 = 0, greenVal2 = 0, blueVal2 = 0;

final int SIX_FOOT_SET_ADDR = 0;
final int EIGHT_FOOT_SET_ADDR = 7;
final int BOX_SET_ADDR = 14;
final int DMX_ADDR = 18;
final int GLOBAL_ADDR = 19;

OscP5 oscP5;
NetAddress myRemoteLocation;
NetAddress myCompLocation;

String iPadIP = "192.168.1.7";
int iPadPort = 12005;
int compPort = 12001;

public int headbandSelect=1;

String[] myPatterns = new String[81];


ControlP5 controlP5;

Headband[] headbands = new Headband[3];
public void setup() {
  size(400, 400);
  background(0);
  println(Serial.list());

  port = new Serial(this, 
    // "/dev/tty.usbserial-A501E3DJ", 
    Serial.list()[0],
  9600); 

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
  myCompLocation = new NetAddress(oscP5.ip(), compPort);

  Radio r = controlP5.addRadio("radio", 270, 5);
  r.deactivateAll(); // use deactiveAll to not make the first radio button active.
  r.add("pulseOnce", 80);
  r.add("colorWipeMeterGradient", 79);
  r.add("colorWipeMeter", 78);
  r.add("colorChase", 77);
  r.add("stripe", 76);
  r.add("colorAlternator", 75);
  r.add("colorWipe", 74);
  r.add("bounce", 73);
  r.add("pulseSaw", 72);
  r.add("pulseSine", 71);
  r.add("gradient", 70);
  r.add("solidColor", 69);
  r.add("** OFF", 68);
  r.add("rainbow", 66);
  r.add("rainbowCycle", 65);
  r.add("totesRandom", 64);
  r.add("flickerStrobeFour", 63);
  r.add("flickerStrobeTwo", 62);
  r.add("** FORWARD", 1);
  r.add("** BACKWARD", 2);
  r.add("** PEAK", 3);
  r.add("** VALLEY", 4);
  r.add("** DITHER", 5);


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

  headbands[headbandSelect].pattern = 80;
  headbands[headbandSelect].rate = 9;
  headbands[headbandSelect].send();
}
int lastSend = -1;
final int INTERVAL = 100;
final int TIMING_ADDR = 129;

public void draw() {

  while (toSend.peek () != null) {
    port.write(((Integer)toSend.poll()).intValue());
  }

  int now = millis();
  if (now - lastSend >= INTERVAL) {

    port.write(TIMING_ADDR);
    //    port.write(now);

    port.write(new Integer(now).toString());

    port.write(128);

    lastSend = now;

    //    println(now);
  }
}


public void keyPressed() {
  headbands[headbandSelect].rate = (int)map(mouseX, 0, width, 2, 255);
  headbands[headbandSelect].send();
}

public void radio(int theId) {
  if (theId < 0) return;
  headbands[headbandSelect].pattern = (byte)theId;
  headbands[headbandSelect].send();
}  

/* incoming osc message are forwarded to the oscEvent method. */
public void oscEvent(OscMessage theOscMessage) {

  String message = theOscMessage.addrPattern();
 
  //  
  //  if(message.indexOf("z") == message.length() - 1){
  //   return; 
  //  }

  /* print the address pattern and the typetag of the received OscMessage */

  if (DEBUG) {
    println(message);
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
      && theOscMessage.get(0).floatValue()==1.0f) {
      headbands[headbandSelect].pattern = i;
      headbands[headbandSelect].send();
      return;
    }
  }

  if (theOscMessage.checkAddrPattern("/1/push1")  
    && theOscMessage.get(0).floatValue()==1.0f) {
    swapColors();
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/push2") 
    && theOscMessage.get(0).floatValue()==1.0f) {
    headbands[headbandSelect].pattern = 80;
    headbands[headbandSelect].send();
    return;
  }

  for (int i = 62; i < 81; i++) {
    if (theOscMessage.checkAddrPattern(myPatterns[i]) 
      && theOscMessage.get(0).floatValue()==1.0f) {
      headbands[headbandSelect].pattern = i;
      headbands[headbandSelect].send();
      return;
    }
  }



  /////////PRESETS/////////

  if (theOscMessage.checkAddrPattern("/1/push3") 
    && theOscMessage.get(0).floatValue()==1.0f) {
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
    && theOscMessage.get(0).floatValue()==1.0f) {
    //PULSE GREEN TO WHITE
    OSCrate(10);
    OSCmaster(127);//FULL BRIGHTNESS
    OSCcolors(0, 255, 255, 0, 0, 0);
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
    && theOscMessage.get(0).floatValue()==1.0f) {
    //RAINBOW FAST
    OSCrate(17);
    OSCmaster(127);//FULL BRIGHTNESS
    //make sure to set mapping too
    OSCpattern(1);
    OSCpattern(65);
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/1/3") 
    && theOscMessage.get(0).floatValue()==1.0f) {
    //RAINBOW SLOW DIM
    OSCrate(127);
    OSCmaster(15);//FULL BRIGHTNESS
    //make sure to set mapping too
    OSCpattern(1);
    OSCpattern(65);
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/1/4") 
    && theOscMessage.get(0).floatValue()==1.0f) {
    //RAINBOW PEAK
    OSCrate(17);
    OSCmaster(127);//FULL BRIGHTNESS
    //make sure to set mapping too
    OSCpattern(3);
    OSCpattern(65);
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/1/5") 
    && theOscMessage.get(0).floatValue()==1.0f) {
    //RAINBOW DITHER
    OSCrate(17);
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
    && theOscMessage.get(0).floatValue()==1.0f) {
    //EASTER PASTELS
    OSCrate(17);
    OSCmaster(127);//FULL BRIGHTNESS
    //make sure to set mapping too
    OSCpattern(1);
    OSCpattern(64);
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/2/3") 
    && theOscMessage.get(0).floatValue()==1.0f) {
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
    && theOscMessage.get(0).floatValue()==1.0f) {
    //FLICKER FOUR ALL WHITE FAST 
    OSCrate(17);
    OSCmaster(127);//FULL BRIGHTNESS
    OSCcolors(255, 255, 255, 255, 255, 255);
    //make sure to set mapping too
    OSCpattern(1);
    OSCpattern(63);
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/2/5") 
    && theOscMessage.get(0).floatValue()==1.0f) {
    //RAINBOW CYCLE
    OSCrate(17);
    OSCmaster(127);//FULL BRIGHTNESS
    //make sure to set mapping too
    OSCpattern(1);
    OSCpattern(66);
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/3/1") 
    && theOscMessage.get(0).floatValue()==1.0f) {
    //BLUE PINK RAIN
    OSCrate(17);
    OSCmaster(127);//FULL BRIGHTNESS
    OSCcolors(0, 0, 255, 255, 0, 0);
    //make sure to set mapping too
    OSCpattern(1);
    OSCpattern(70);
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/3/2") 
    && theOscMessage.get(0).floatValue()==1.0f) {
    //YELLOW GREEN RAIN
    OSCrate(17);
    OSCmaster(127);//FULL BRIGHTNESS
    OSCcolors(255, 255, 0, 0, 255, 0);
    //make sure to set mapping too
    OSCpattern(1);
    OSCpattern(70);
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/3/3") 
    && theOscMessage.get(0).floatValue()==1.0f) {
    //BLUE TURQUOISE STRIPES UP
    OSCrate(17);
    OSCmaster(127);//FULL BRIGHTNESS
    OSCcolors(0, 0, 255, 0, 255, 118);
    //make sure to set mapping too
    OSCpattern(2);
    OSCpattern(76);
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/3/4") 
    && theOscMessage.get(0).floatValue()==1.0f) {
    //RED GAP CHASE
    OSCrate(17);
    OSCmaster(127);//FULL BRIGHTNESS
    OSCcolors(0, 0, 0, 255, 0, 0);
    //make sure to set mapping too
    OSCpattern(1);
    OSCpattern(77);
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/3/5") 
    && theOscMessage.get(0).floatValue()==1.0f) {
    //RED/GREEN STROBE
    OSCrate(127/2);
    OSCmaster(127);//FULL BRIGHTNESS
    OSCcolors(255, 0, 0, 0, 255, 0);
    //make sure to set mapping too
    OSCpattern(1);
    OSCpattern(75);
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/4/1") 
    && theOscMessage.get(0).floatValue()==1.0f) {
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
    && theOscMessage.get(0).floatValue()==1.0f) {
    //WHITE RAIN
    OSCrate(17);
    OSCmaster(127);//FULL BRIGHTNESS
    OSCcolors(255, 255, 255, 0, 0, 0);
    //make sure to set mapping too
    OSCpattern(1);
    OSCpattern(70);
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/4/3") 
    && theOscMessage.get(0).floatValue()==1.0f) {
    //PURPLE BLUE STRIPES
    OSCrate(17);
    OSCmaster(127);//FULL BRIGHTNESS
    OSCcolors(255, 0, 255, 0, 255, 255);
    //make sure to set mapping too
    OSCpattern(3);
    OSCpattern(76);
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/4/4") 
    && theOscMessage.get(0).floatValue()==1.0f) {
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
    && theOscMessage.get(0).floatValue()==1.0f) {
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
    && theOscMessage.get(0).floatValue()==1.0f) {
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
    && theOscMessage.get(0).floatValue()==1.0f) {
    //XMAS COLORS
    OSCmaster(127);//FULL BRIGHTNESS
    OSCcolors(255, 0, 0, 0, 0, 10);
    headbands[headbandSelect].send();
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/5/3") 
    && theOscMessage.get(0).floatValue()==1.0f) {
    //SINE FADE GREEN TO PURPLE
    OSCrate(17);
    OSCmaster(127);//FULL BRIGHTNESS
    OSCcolors(0, 255, 0, 211, 0, 255);
    //make sure to set mapping too
    OSCpattern(1);
    OSCpattern(71);
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/5/4") 
    && theOscMessage.get(0).floatValue()==1.0f) {
    //SINE FADE RED TO BLUE
    OSCrate(17);
    OSCmaster(127);//FULL BRIGHTNESS
    OSCcolors(255, 0, 0, 0, 0, 255);
    //make sure to set mapping too
    OSCpattern(1);
    OSCpattern(71);
    return;
  }

  if (theOscMessage.checkAddrPattern("/1/presets/5/5") 
    && theOscMessage.get(0).floatValue()==1.0f) {
    //STROBE WHITE
    OSCrate(17);
    OSCmaster(127);//FULL BRIGHTNESS
    OSCcolors(255, 255, 255, 0, 0, 0);
    //make sure to set mapping too
    OSCpattern(1);
    OSCpattern(75);
    return;
  }
  
  
  if (theOscMessage.checkAddrPattern("/1/addr/1/1")) {
    WHICH = GLOBAL_ADDR;
    clearPattern();
    return;
  }
  
  if (theOscMessage.checkAddrPattern("/1/addr/2/1")) {
    WHICH = EIGHT_FOOT_SET_ADDR;
    clearPattern();
    return;
  }
  
  if (theOscMessage.checkAddrPattern("/1/addr/3/1")) {
    WHICH = SIX_FOOT_SET_ADDR;
    clearPattern();
    return;
  }
  
  if (theOscMessage.checkAddrPattern("/1/addr/4/1")) {
    WHICH = BOX_SET_ADDR;
    clearPattern();
    return;
  }
  
  if (theOscMessage.checkAddrPattern("/1/addr/5/1")) {
    WHICH = DMX_ADDR;
    clearPattern();
    return;
  }
  
  
}

public void clearPattern() { 
  for (int i = 1; i <= 9; i++) {
    for (int j = 1; j <= 2; j++) {  
      String addr = "/1/multitoggle1/"+j+"/"+i;
      OscMessage clearPattern = new OscMessage(addr);
      clearPattern.add(0.0f); /* add an int to the osc message */
      oscP5.send(clearPattern, myRemoteLocation);
    }
  }
  }

//OSC helper functions
public void OSCpattern(int patternNum) {
  OscMessage patternMessage = new OscMessage(myPatterns[patternNum]);
  patternMessage.add(1.0f); /* toggle on */
  oscP5.send(patternMessage, myRemoteLocation);
  oscP5.send(patternMessage, myCompLocation);
}

public void OSCrate(int rateValue) {
  OscMessage rateMessage = new OscMessage("/1/rateFader");
  rateMessage.add(rateValue); /* add an int to the osc message */
  oscP5.send(rateMessage, myRemoteLocation);
  headbands[headbandSelect].rate= rateValue;
}

public void OSCmaster(int masterValue) {
  OscMessage masterMessage = new OscMessage("/1/masterFader");
  masterMessage.add(masterValue); /* add an int to the osc message */
  oscP5.send(masterMessage, myRemoteLocation);
  headbands[headbandSelect].dim = masterValue;
}

public void OSCcolors(int r1, int g1, int b1, int r2, int g2, int b2) {
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

public void swapColors() {
  OSCcolors(redVal2, greenVal2, blueVal2, redVal1, greenVal1, blueVal1); 
  headbands[headbandSelect].send();
}
final int MESSAGE_SIZE = 14;

int WHICH = 19;

LinkedList toSend = new LinkedList();

class Headband {

  private int serialData[] = new int[MESSAGE_SIZE];

  public ColorPicker[] colors = new ColorPicker[3];
  public int pattern = 0;
  public int dim = 127;
  public int rate = 1;

  public int address;

  public Headband(int address) {

    colors[0] = controlP5.addColorPicker(address+"picker1", 5, 5, 255, 20);
    colors[0].setColorValue(color(255));

    colors[1] = controlP5.addColorPicker(address+"picker2", 5, 70, 255, 20);
    colors[1].setColorValue(color(0));

    colors[2] = controlP5.addColorPicker(address+"picker3", 5, 135, 255, 20);
    colors[2].setColorValue(color(0));


    this.address = address;
  }


  //helper function for OSC
  public void setColor(int cindex, int r, int g, int b) {

    pattern = 0;
    ColorPicker cp = colors[cindex];
    colorMode(RGB, 255, 255, 255);
    cp.setColorValue(color(r, g, b));
  }


  //send null pattern byte so only rate is adjusted
  public void adjustRate(int rate) {
    pattern = 0;
    this.rate = rate;
  }

  //send null pattern byte so only dim value is affected
  public void adjustBrightness(int dim) {
    pattern = 0;
    this.dim = dim;
  }

  public void send() {
    serialData[0] = WHICH;
    serialData[1] = (byte)rate;
    serialData[2] = (byte)pattern;
    float a = alpha(colors[0].getColorValue())/255;
    serialData[3] = (byte)(red(colors[0].getColorValue())/2*a);
    serialData[4] = (byte)(green(colors[0].getColorValue())/2*a);
    serialData[5] = (byte) (blue(colors[0].getColorValue())/2*a);
    a = alpha(colors[1].getColorValue())/255;
    serialData[6] = (byte)(red(colors[1].getColorValue())/2*a);
    serialData[7] = (byte)(green(colors[1].getColorValue())/2*a);
    serialData[8] = (byte) (blue(colors[1].getColorValue())/2*a);
    a = alpha(colors[2].getColorValue())/255;
    serialData[9] = (byte)(red(colors[2].getColorValue())/2*a);
    serialData[10] = (byte)(green(colors[2].getColorValue())/2*a);
    serialData[11] = (byte) (blue(colors[2].getColorValue())/2*a);
    serialData[12] = (byte)dim;
    serialData[13] = 128;

  //  println(serialData);

    for (int i = 0; i<serialData.length;i++) {
     toSend.offer(serialData[i]); 
//      port.write(serialData[i]);
    }

    
    
  }
  
}



  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "LEDTEST_OSC_MULTI" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
