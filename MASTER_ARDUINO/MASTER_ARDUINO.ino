
#include <Wire.h>
char inData[12] = {};
char outData[12] = {};

#define ADV_PATTERN 101
#define RESET_FRAME 100
#define IGNORE_DATA 0

#define numPoles 2

boolean light = false;


void setup()
{
  pinMode(13, OUTPUT);   
  Serial.begin(9600);
  Wire.begin(); // join i2c bus (address optional for master)
  //   DmxSimple.usePin(3);
  //  DmxSimple.maxChannel(4);
}

void loop()
{
//  if(millis()< 5000){
//     Wire.beginTransmission(1);
//  Wire.write(IGNORE_DATA);       
//  Wire.write(RESET_FRAME);
//  Wire.write(IGNORE_DATA);  
//  Wire.write(IGNORE_DATA);  
//  Wire.write(IGNORE_DATA);  
//  Wire.write(IGNORE_DATA);  
//  Wire.write(IGNORE_DATA);  
//  Wire.write(IGNORE_DATA);  
//  Wire.write(IGNORE_DATA);  
//  Wire.write(IGNORE_DATA);  
//  Wire.write(IGNORE_DATA);  
//  Wire.write(IGNORE_DATA);  
//  Wire.endTransmission(); 
//  }


  Wire.beginTransmission(1);
  Wire.write(outData[0]);       
  Wire.write(outData[1]);
  Wire.write(outData[2]);
  Wire.write(outData[3]);
  Wire.write(outData[4]);
  Wire.write(outData[5]);
  Wire.write(outData[6]);
  Wire.write(outData[7]);
  Wire.write(outData[8]);
  Wire.write(outData[9]);
  Wire.write(outData[10]);
  Wire.write(outData[11]);  
  Wire.endTransmission();

  outData[1] = ADV_PATTERN;
  
//  if (light)
//    digitalWrite(13, HIGH);
//  else
//    digitalWrite(13, LOW);
//    
//  light = !light;
  
}

void serialEvent() {
  Serial.readBytes(inData,12);
  // Wire.beginTransmission((int)random(2)+1); // transmit to device #1
  //for(int i = 1; i <=numPoles; i++){
 for(int i = 0; i<12; i++){
   outData[i] = inData[i];
 }
  // }
}



