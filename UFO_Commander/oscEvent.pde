void oscEvent(OscMessage theOscMessage) {
  
  println("[OSC] " + theOscMessage.addrPattern());

  if (theOscMessage.addrPattern().equals("/Presets/x")) {
    
    for (int i = 0; i < theOscMessage.arguments().length; i++) {
      if (theOscMessage.arguments()[i].equals(1.0)) {
  
        applyPreset(i);

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

    activeAddr = theOscMessage.arguments();

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