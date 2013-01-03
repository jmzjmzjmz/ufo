
#include <Wire.h>
char myData[6+25*3] = {};



int loopTime = 25;

void setup()
{
  Serial.begin(115200);
  Wire.begin(); // join i2c bus (address optional for master)
//   DmxSimple.usePin(3);
//  DmxSimple.maxChannel(4);

}

void loop()
{

Serial.readBytes(myData, 81);
    Wire.beginTransmission(1);

   Wire.write(myData); //send serial data to slave arduino
  Wire.endTransmission();    // stop transmitting
 //Serial.println(myData);

 
}

void serialEvent() {



}

