interface OscReader {
  void read(OscMessage theOscMessage);
}

OscReader reader = new LiveReader(); //new LemurReader();

void oscEvent(OscMessage theOscMessage) {

  if (DEBUG) println("[OSC] " + theOscMessage.addrPattern());

  reader.read(theOscMessage);

}

void oscMessage(String addr, float value) {
  OscMessage message = new OscMessage(addr);
  message.add(value);
  oscP5.send(message, myRemoteLocation);
}


void setActiveAddr(Object[] activeAddr) {
  
  this.activeAddr = activeAddr;

}