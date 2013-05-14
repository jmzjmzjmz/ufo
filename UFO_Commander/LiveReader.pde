
class LiveReader implements OscReader {

  void read(OscMessage theOscMessage) {

    String p = theOscMessage.addrPattern();

    Matcher presets = Pattern.compile("/preset/(\\d+)").matcher(p);
    Matcher patterns = Pattern.compile("/pattern/(\\d+)").matcher(p);
    Matcher mappings = Pattern.compile("/mapping/(\\d+)").matcher(p);
    Matcher address = Pattern.compile("/address/(\\d+)").matcher(p);
    
    if (presets.matches()) {
  
      applyPreset(parseInt(presets.group()));

    } else if (patterns.matches()) {
      
      for (int j = 0; j < activeAddr.length; j++) {
        if (activeAddr[j].equals(1.0)) {
          
          LightGroup l = (LightGroup)lightGroups.get(j);
          l.setPattern(patternsToIndeces[parseInt(patterns.group())]);
          l.sendMessage();

        }
      }

    } else if (mappings.matches()) {
      
      for (int j = 0; j < activeAddr.length; j++) {
        if (activeAddr[j].equals(1.0)) {
          
          LightGroup l = (LightGroup)lightGroups.get(j);
          l.setMapping(parseInt(mappings.group())+1); // hack.
          l.sendMessage();

        }
      }

    } else if (address.matches()) { 

      int a = parseInt(address.group());
      for (int i = 0; i < activeAddr.length; i++) {
        if (a == i) {
          activeAddr[i] = 1.0;
        } else {
          activeAddr[i] = 0.0;          
        }
      }
      
    } else if (p.equals("/red1")) { 

      for (int i = 0; i < activeAddr.length; i++) {
        if (activeAddr[i].equals(1.0)) {

          int v = (int)(theOscMessage.get(0).floatValue() * 255);
          LightGroup l = (LightGroup)lightGroups.get(i);
          l.setColor1(v, l.g1, l.b1);
          l.sendMessage();

        }
      }

    } else if (p.equals("/green1")) { 

      for (int i = 0; i < activeAddr.length; i++) {
        if (activeAddr[i].equals(1.0)) {

          int v = (int)(theOscMessage.get(0).floatValue() * 255);
          LightGroup l = (LightGroup)lightGroups.get(i);
          l.setColor1(l.r1, v, l.b1);
          l.sendMessage();

        }
      }

    } else if (p.equals("/blue1")) { 

      for (int i = 0; i < activeAddr.length; i++) {
        if (activeAddr[i].equals(1.0)) {

          int v = (int)(theOscMessage.get(0).floatValue() * 255);
          LightGroup l = (LightGroup)lightGroups.get(i);
          l.setColor1(l.r1, l.g1, v);
          l.sendMessage();


        }
      }

    } else if (p.equals("/red2")) { 

      for (int i = 0; i < activeAddr.length; i++) {
        if (activeAddr[i].equals(1.0)) {

          int v = (int)(theOscMessage.get(0).floatValue() * 255);
          LightGroup l = (LightGroup)lightGroups.get(i);
          l.setColor2(v, l.g2, l.b2);
          l.sendMessage();


        }
      }

    } else if (p.equals("/green2")) { 

      for (int i = 0; i < activeAddr.length; i++) {
        if (activeAddr[i].equals(1.0)) {

          int v = (int)(theOscMessage.get(0).floatValue() * 255);
          LightGroup l = (LightGroup)lightGroups.get(i);
          l.setColor2(l.r2, v, l.b2);
          l.sendMessage();


        }
      }

    } else if (p.equals("/blue2")) { 

      for (int i = 0; i < activeAddr.length; i++) {
        if (activeAddr[i].equals(1.0)) {

          int v = (int)(theOscMessage.get(0).floatValue() * 255);
          LightGroup l = (LightGroup)lightGroups.get(i);
          l.setColor2(l.r2, l.g2, v);
          l.sendMessage();


        }
      }

    } else if (p.equals("/rate")) { 

      for (int i = 0; i < activeAddr.length; i++) {
        if (activeAddr[i].equals(1.0)) {

          int v = (int)(theOscMessage.get(0).floatValue() * 127);
          LightGroup l = (LightGroup)lightGroups.get(i);
          l.setRate(v);
          l.sendMessage();

        }
      }

    } else if (p.equals("/brightness")) { 

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

}