/*
  http://www.bajdi.com
  SD card datalogger for Arduino Mega (this sketch will ONLY work with Arduino Mega) 
 
 This example shows how to log data from three analog sensors 
 to an SD card using the SD library.
   
 The circuit:
 * analog sensors on analog ins 0, 1, and 2
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 51
 ** MISO - pin 50
 ** CLK - pin 52
 ** CS - pin 53
 
 created  24 Nov 2010
 updated 2 Dec 2010
 by Tom Igoe
 updated 22 Jan 2012 for Arduino Mega
 by Bajdi.com 
 
 This example code is in the public domain.
   
 */

//#include <SPI.h>
//#include <SD.h>
//
//// On the Ethernet Shield, CS is pin 4. Note that even if it's not
//// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
//// 53 on the Mega) must be left as an output or the SD library
//// functions will not work.
//const int chipSelect = 53;
//
//void setup()
//{
//  Serial.begin(9600);
//  Serial.print("Initializing SD card...");
//  // make sure that the default chip select pin is set to
//  // output, even if you don't use it:
//  pinMode(chipSelect, OUTPUT);
//  digitalWrite(chipSelect, LOW);    // turn the LED off by making the voltage LOW
//  // see if the card is present and can be initialized:
//  if (!SD.begin(chipSelect)) {
//    Serial.println("Card failed, or not present");
//    // don't do anything more:
//    return;
//  }
//  Serial.println("card initialized.");
//
//    // make a string for assembling the data to log:
//  String dataString = "";
//
//  // read three sensors and append to the string:
//  for (int analogPin = 0; analogPin < 3; analogPin++) {
//    int sensor = analogRead(analogPin);
//    dataString += String(sensor);
//    if (analogPin < 2) {
//      dataString += ","; 
//    }
//  }
//
//  // open the file. note that only one file can be open at a time,
//  // so you have to close this one before opening another.
//  File dataFile = SD.open("datalog.txt", FILE_WRITE);
//
//  // if the file is available, write to it:
//  if (dataFile) {
//    dataFile.println(dataString);
//    dataFile.close();
//    // print to the serial port too:
//    Serial.println(dataString);
//  }  
//  // if the file isn't open, pop up an error:
//  else {
//    Serial.println("error opening datalog.txt");
//  } 
//  
//}
//
//void loop() {
//  // nothing happens after setup
//}


/*
  SD card read/write

 This example shows how to read and write data to and from an SD card file
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */

#include <SPI.h>
#include <SD.h>

File myFile;
const int chipSelect = 53;
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  pinMode(chipSelect, OUTPUT);
//  digitalWrite(chipSelect, LOW);    // turn the LED off by making the voltage LOW

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void loop() {
  // nothing happens after setup
}
