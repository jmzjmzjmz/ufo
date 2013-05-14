class LemurReader implements OscReader {


  void read(OscMessage theOscMessage) {

    if (theOscMessage.addrPattern().equals("/Presets/x")) {
      
      for (int i = 0; i < theOscMessage.arguments().length; i++) {
        if (theOscMessage.arguments()[i].equals(1.0)) {
    
          applyPreset(i);
          sendFeedback();

        }
      }

    } else if (theOscMessage.addrPattern().equals("/Patterns/x")) {
      
      for (int i = 0; i < theOscMessage.arguments().length; i++) {
        if (theOscMessage.arguments()[i].equals(1.0)) {
        
          for (int j = 0; j < activeAddr.length; j++) {
            if (activeAddr[j].equals(1.0)) {
              
              LightGroup l = (LightGroup)lightGroups.get(j);
              l.setPattern(patternsToIndeces[i]);
              l.sendMessage();

            }
          }

        }
      }

    } else if (theOscMessage.addrPattern().equals("/Mappings/x")) {
      
      for (int i = 0; i < theOscMessage.arguments().length; i++) {
        if (theOscMessage.arguments()[i].equals(1.0)) {
        
          for (int j = 0; j < activeAddr.length; j++) {
            if (activeAddr[j].equals(1.0)) {
              
              LightGroup l = (LightGroup)lightGroups.get(j);
              l.setMapping(i+1); // hack.
              l.sendMessage();

            }
          }

        }
      }

    } else if (theOscMessage.addrPattern().equals("/Addr/x")) { 


      setActiveAddr(theOscMessage.arguments());
      sendFeedback();
      

    } else if (theOscMessage.addrPattern().equals("/Stealth/x")) {

      if (theOscMessage.arguments()[0].equals(1.0)) {
      
        stealth = true;
      
      } else { 
        
        stealth = false;

        for (int j = 0; j < activeAddr.length; j++) {
          if (activeAddr[j].equals(1.0)) {
            LightGroup l = (LightGroup)lightGroups.get(j);
            l.sendMessage();
          }
        }

      }

    } else if (theOscMessage.addrPattern().equals("/RedSlider1/x")) { 

      for (int i = 0; i < activeAddr.length; i++) {
        if (activeAddr[i].equals(1.0)) {

          int v = (int)(theOscMessage.get(0).floatValue() * 255);
          LightGroup l = (LightGroup)lightGroups.get(i);
          l.setColor1(v, l.g1, l.b1);
          l.sendMessage();


        }
      }

    } else if (theOscMessage.addrPattern().equals("/GreenSlider1/x")) { 

      for (int i = 0; i < activeAddr.length; i++) {
        if (activeAddr[i].equals(1.0)) {

          int v = (int)(theOscMessage.get(0).floatValue() * 255);
          LightGroup l = (LightGroup)lightGroups.get(i);
          l.setColor1(l.r1, v, l.b1);
          l.sendMessage();


        }
      }

    } else if (theOscMessage.addrPattern().equals("/BlueSlider1/x")) { 

      for (int i = 0; i < activeAddr.length; i++) {
        if (activeAddr[i].equals(1.0)) {

          int v = (int)(theOscMessage.get(0).floatValue() * 255);
          LightGroup l = (LightGroup)lightGroups.get(i);
          l.setColor1(l.r1, l.g1, v);
          l.sendMessage();


        }
      }

    } else if (theOscMessage.addrPattern().equals("/RedSlider2/x")) { 

      for (int i = 0; i < activeAddr.length; i++) {
        if (activeAddr[i].equals(1.0)) {

          int v = (int)(theOscMessage.get(0).floatValue() * 255);
          LightGroup l = (LightGroup)lightGroups.get(i);
          l.setColor2(v, l.g2, l.b2);
          l.sendMessage();


        }
      }

    } else if (theOscMessage.addrPattern().equals("/GreenSlider2/x")) { 

      for (int i = 0; i < activeAddr.length; i++) {
        if (activeAddr[i].equals(1.0)) {

          int v = (int)(theOscMessage.get(0).floatValue() * 255);
          LightGroup l = (LightGroup)lightGroups.get(i);
          l.setColor2(l.r2, v, l.b2);
          l.sendMessage();


        }
      }

    } else if (theOscMessage.addrPattern().equals("/BlueSlider2/x")) { 

      for (int i = 0; i < activeAddr.length; i++) {
        if (activeAddr[i].equals(1.0)) {

          int v = (int)(theOscMessage.get(0).floatValue() * 255);
          LightGroup l = (LightGroup)lightGroups.get(i);
          l.setColor2(l.r2, l.g2, v);
          l.sendMessage();


        }
      }

    } else if (theOscMessage.addrPattern().equals("/RateSlider/x")) { 

      for (int i = 0; i < activeAddr.length; i++) {
        if (activeAddr[i].equals(1.0)) {

          int v = (int)(theOscMessage.get(0).floatValue() * 127);
          LightGroup l = (LightGroup)lightGroups.get(i);
          l.setRate(v);
          l.sendMessage();

        }
      }

    } else if (theOscMessage.addrPattern().equals("/BrightnessSlider/x")) { 

      for (int i = 0; i < activeAddr.length; i++) {
        if (activeAddr[i].equals(1.0)) {

          int v = (int)(theOscMessage.get(0).floatValue() * 127);
          LightGroup l = (LightGroup)lightGroups.get(i);
          l.setBrightness(v);
          l.sendMessage();

        }
      }

    }
    
  }


  void sendFeedback() {

    int feedbackIndex = 0;

    for (int i = 0; i < activeAddr.length; i++) {
      if (activeAddr[i].equals(1.0)) {
        feedbackIndex = i;
        break;
      }
    }

    LightGroup l = (LightGroup)lightGroups.get(feedbackIndex);

    oscMessage("/RedSlider1/x",   map(red(l.color1), 0, 255, 0, 1));
    oscMessage("/GreenSlider1/x", map(green(l.color1), 0, 255, 0, 1));
    oscMessage("/BlueSlider1/x",  map(blue(l.color1), 0, 255, 0, 1));

    oscMessage("/RedSlider2/x",   map(red(l.color2), 0, 255, 0, 1));
    oscMessage("/GreenSlider2/x", map(green(l.color2), 0, 255, 0, 1));
    oscMessage("/BlueSlider2/x",  map(blue(l.color2), 0, 255, 0, 1));

    oscMessage("/RateSlider/x",   map(l.rate, 0, 127, 0, 1));
    oscMessage("/BrightnessSlider/x", map(l.brightness, 0, 127, 0, 1));

    // Send pattern message.
    
    int patternIndex = 0;
    for (int i = 0; i < patternsToIndeces.length; i++) {
      if (patternsToIndeces[i] == l.pattern) {
        patternIndex = i;
      }
    }

    float[] patternMessage = new float[patternsToIndeces.length];
    for (int i = 0; i < patternMessage.length; i++) {
      if (i == patternIndex) {
        patternMessage[i] = 1.0;
      }
    }

    OscMessage message = new OscMessage("/Patterns/x");
    message.add(patternMessage);
    oscP5.send(message, myRemoteLocation);

    // Send mapping message.

    int mappingIndex = l.mapping - 1; // HACK
    message = new OscMessage("/Mappings/x");
    float[] mappingMessage = new float[mappings.length];
    for (int i = 0; i < mappings.length; i++) {
      if (i == mappingIndex) {
        mappingMessage[i] = 1.0;
      }
    }
    message.add(mappingMessage);
    oscP5.send(message, myRemoteLocation);

  }


}