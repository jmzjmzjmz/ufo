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
  public color color1, color2;
  
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