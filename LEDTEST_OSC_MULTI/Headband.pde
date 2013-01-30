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



