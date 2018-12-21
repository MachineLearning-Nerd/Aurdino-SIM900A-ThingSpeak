#include <SoftwareSerial.h>
//Create software serial object to communicate with SIM900A

// SIM900A's TXD = ARDUINO's #3
// SIM900A's RXD = ARDUINO's #2 
SoftwareSerial mySerial(3, 2); 

void setup()
{
  mySerial.begin(4800);                 
  Serial.begin(4800);                  
}

void loop()
{
  if (mySerial.available())
    Serial.write(mySerial.read());
  if (Serial.available())
    mySerial.write(Serial.read());  

}
