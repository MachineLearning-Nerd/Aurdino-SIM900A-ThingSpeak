////////////////////////////////////////////////////////////////
//              Dinesh SD Card and RTC Code                   //
////////////////////////////////////////////////////////////////
#include <SD.h>
#include <SPI.h>



#include <Wire.h>
#include "RTClib.h"
#include <SoftwareSerial.h>

//Create software serial object to communicate with SIM900A

// SIM900A's TXD = ARDUINO's #3
// SIM900A's RXD = ARDUINO's #2
/* Create object named SIM900A of the class SoftwareSerial */
SoftwareSerial SIM900(7, 6);

/* This is the pin assignments*/
//int CS_PIN = 10;
//const int buttonPin = 5;
#define CS_PIN 10
#define buttonPin  5
/*This is the variable to save the file in the SD Card*/
File myFile;
RTC_DS1307 RTC;
char* filename = "ctest.txt";

////Variables
int buttonState = 0;
int SWPrestime = 0;
int SWReleatime = 0;
int flag = 0;

/*This are the functions for SD CARD*/
 void initializeSD();
 char createFile(char filename[]);
 int writeToFile(char text[]);
 void closeFile();
 char openFile(char filename[]);

 void GSMinitialization();
 void ShowSerialData();
 void SendData(unsigned int datadine);
 char CalculateDigit(unsigned int num);

void setup()
{
  SIM900.begin(4800);  /* Define baud rate for software serial communication */
  // Serial.begin(115200);
  // TWI is enabled for RTC
  Wire.begin();
  RTC.begin();
  //RTC.adjust(DateTime(__DATE__, __TIME__));
  // Check to see if the RTC is keeping time.  If it is, load the time from your computer.
  if (! RTC.isrunning())
  {
//    Serial.println("RTC Date : Compile Time:");
    // This will reflect the time that your sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }

  // Button initialization
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);       // turn on pullup resistors

  initializeSD();

  createFile(filename);
  closeFile();


  
  delay(2000);
  GSMinitialization();
//  delay(2000);

//  SendData(85);
//  SendData(185);

  // TO CHECK THE DATA
  // re-open the file for reading:
  //    myFile = SD.open(filename);
  //    if (myFile) {
  //      Serial.println(filename);
  //      Serial.print(" :");
  //      // read from the file until there's nothing else in it:
  //      while (myFile.available()) {
  //        Serial.write(myFile.read());
  //      }
  //      // close the file:
  //      myFile.close();
  //    } else {
  //      // if the file didn't open, print an error:
  //      Serial.println("error opening test.txt");
  //    }
  flag = 0;
}

void loop () {
  
  //Read button state (pressed or not pressed?)
  buttonState = digitalRead(buttonPin);

  //If button pressed...
  if (buttonState == LOW) {
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW

    //...ones, turn led on!
    if ( flag == 0) {

      //Serial.println("buttonState is low");

      DateTime SwitchPressed = RTC.now();
      SWPrestime = SwitchPressed.unixtime();

      myFile = SD.open(filename, FILE_WRITE);
      if (myFile) {
        myFile.print("Sitting Time : ");
        myFile.print(SwitchPressed.day(), DEC);
        myFile.print('/');
        myFile.print(SwitchPressed.month(), DEC);
        myFile.print('/');
        myFile.print(SwitchPressed.year(), DEC);
        myFile.print(' ');
        myFile.print(SwitchPressed.hour(), DEC);
        myFile.print(':');
        myFile.print(SwitchPressed.minute(), DEC);
        myFile.print(':');
        myFile.print(SwitchPressed.second(), DEC);
        myFile.print("   ");
      }

      myFile.close();

//      Serial.print("Sitting Time : ");
//      Serial.print(SwitchPressed.day(), DEC);
//      Serial.print('/');
//      Serial.print(SwitchPressed.month(), DEC);
//      Serial.print('/');
//      Serial.print(SwitchPressed.year(), DEC);
//      Serial.print(' ');
//      Serial.print(SwitchPressed.hour(), DEC);
//      Serial.print(':');
//      Serial.print(SwitchPressed.minute(), DEC);
//      Serial.print(':');
//      Serial.print(SwitchPressed.second(), DEC);
//      Serial.print("   ");

      flag = 1; //change flag variable
    }
  }
  else if (buttonState == HIGH && flag == 1) {
    flag = 0;
    delay(1000);
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)

    DateTime Switchreleased = RTC.now();
    SWReleatime = Switchreleased.unixtime();

    //        myFile.print("Standing Time : ");
    //        Serial.println("Standing Time : ");

    myFile = SD.open(filename, FILE_WRITE);
    if (myFile) {
      myFile.print("Standing Time : ");
      myFile.print(Switchreleased.day(), DEC);
      myFile.print('/');
      myFile.print(Switchreleased.month(), DEC);
      myFile.print('/');
      myFile.print(Switchreleased.year(), DEC);
      myFile.print(' ');
      myFile.print(Switchreleased.hour(), DEC);
      myFile.print(':');
      myFile.print(Switchreleased.minute(), DEC);
      myFile.print(':');
      myFile.print(Switchreleased.second(), DEC);
      myFile.print("   ");
    }
    myFile.close();

//    Serial.print("Standing Time : ");
//    Serial.print(Switchreleased.day(), DEC);
//    Serial.print('/');
//    Serial.print(Switchreleased.month(), DEC);
//    Serial.print('/');
//    Serial.print(Switchreleased.year(), DEC);
//    Serial.print(' ');
//    Serial.print(Switchreleased.hour(), DEC);
//    Serial.print(':');
//    Serial.print(Switchreleased.minute(), DEC);
//    Serial.print(':');
//    Serial.print(Switchreleased.second(), DEC);
//    Serial.print("   ");
    myFile = SD.open(filename, FILE_WRITE);
    if (myFile) {
      myFile.print("Total Sitting Time : ");
    }
    myFile.close();

//    Serial.print("Total Sitting Time : ");

    int sittime = 0;
    sittime = SWReleatime - SWPrestime;
    myFile = SD.open(filename, FILE_WRITE);
    if (myFile) {
      myFile.print(sittime / 60, DEC);
      myFile.print(" minutes");
      myFile.println();

    }
    myFile.close();
//    Serial.print(sittime / 60, DEC);
//    Serial.print(" minutes");
//    Serial.println();

    unsigned int minut = sittime / 60;
//    Serial.println(minut);
//    Serial.println("Minutes");
//    delay(2000);
    SIM900.println("AT+CIPSHUT"); /* Query the GPRS context */
//    delay(2000);
    ShowSerialData();
    delay(2000);   
    SIM900.println("AT+CIPSTART= \"TCP\",\"184.106.153.149\",\"80\"");  /* Terminate HTTP service */
//    delay(2000);
    ShowSerialData();
    delay(2000);
  
    char digits = CalculateDigit(minut);
    if(digits == 1)
    {
      SIM900.println("AT+CIPSEND=43"); /* Close GPRS context */
    } 
    else if(digits == 2)
    {
      SIM900.println("AT+CIPSEND=44"); /* Close GPRS context */   
    } 
    else if(digits == 3)
    {
      SIM900.println("AT+CIPSEND=45"); /* Close GPRS context */
  
    } 
      delay(2000);
      ShowSerialData();
      delay(2000);    
  //    unsigned int sensorValue = datadine;
      String stringOne = "GET /update?key=D483VK7ABLHOIMA5&field1=";
      String stringThree = stringOne + minut;
      SIM900.println(stringThree); /* Close GPRS context */
      delay(2000);
      ShowSerialData();  
      delay(2000);  
    SendData(5);
  }
}
 void initializeSD()
{
//  Serial.println("Initializing SD card...");
//  int CS_PIN = 10;
  pinMode(CS_PIN, OUTPUT);

  if (SD.begin())
  {
//    Serial.println("SD card is ready to use.");
  } else
  {
//    Serial.println("SD card initialization failed");
    return;
  }
}

 char createFile(char filename[])
{
  myFile = SD.open(filename, FILE_WRITE);

  if (myFile)
  {
//    Serial.println("File created successfully.");
    return 1;
  } else
  {
//    Serial.println("Error while creating file.");
    return 0;
  }
}

 int writeToFile(char text[])
{
  if (myFile)
  {
    myFile.println(text);
//    Serial.println("Writing to file: ");
//    Serial.println(text);
    return 1;
  } else
  {
//    Serial.println("Couldn't write to file");
    return 0;
  }
}

 void closeFile()
{
  if (myFile)
  {
    myFile.close();
//    Serial.println("File closed");
  }
}

 char openFile(char filename[])
{
  myFile = SD.open(filename);
  if (myFile)
  {
//    Serial.println("File opened with success!");
    return 1;
  } else
  {
//    Serial.println("Error opening file...");
    return 0;
  }
}


 void GSMinitialization()
{
//  Serial.println("HTTP post method :");
//  Serial.print("AT\r\n");
  SIM900.println("AT"); /* Check Communication */
  delay(2000);
  ShowSerialData();  /* Print response on the serial monitor */
  delay(2000);
//  Serial.print("AT+CGATT?\r\n");
  SIM900.println("AT+CGATT?");  /* Open GPRS context */
  delay(2000);
  ShowSerialData();
  delay(2000);
//  Serial.print("AT+CIPSHUT\r\n");
  SIM900.println("AT+CIPSHUT"); /* Query the GPRS context */
  delay(2000);
  ShowSerialData();
  delay(2000);
//  Serial.print("AT+CIPSTATUS\r\n");
  SIM900.println("AT+CIPSTATUS"); /* Initialize HTTP service */
  delay(2000);
  ShowSerialData();
  delay(2000);
//  Serial.print("AT+CIPMUX=0\r\n");
  SIM900.println("AT+CIPMUX=0");  /* Set parameters for HTTP session */
  delay(2000);
  ShowSerialData();
  delay(2000);
//  Serial.print("AT+CGDCONT=1,\"IP\",\"airtelgprs.com\"\r\n");
  SIM900.println("AT+CGDCONT=1,\"IP\",\"airtelgprs.com\""); /* Set parameters for HTTP session */
  delay(2000);
  ShowSerialData();
  delay(2000);
//  Serial.print("AT+CSTT=\"airtelgprs.com\",\"\",\"\"\r\n");
  SIM900.println("AT+CSTT=\"airtelgprs.com\",\"\",\"\""); /* POST data of size 33 Bytes with maximum latency time of 10seconds for inputting the data*/
  delay(2000);
  ShowSerialData();
  delay(2000);
//  Serial.print("AT+CIICR\r\n"); /* Data to be sent */
  SIM900.println("AT+CIICR");
  delay(2000);
  ShowSerialData();
  delay(2000);
//  Serial.print("AT+CIFSR\r\n");
  SIM900.println("AT+CIFSR"); /* Start POST session */
  delay(2000);
  ShowSerialData();
}

 void ShowSerialData()
{
  while (SIM900.available() != 0)  /* If data is available on serial port */
  {
//    Serial.write(char (SIM900.read())); /* Print character received on to the serial monitor */
  }
//  Serial.println(" ");
}


 void SendData(unsigned int datadine)
{
  delay(2000);
  SIM900.println("AT+CIPSHUT"); /* Query the GPRS context */
  delay(2000);
  ShowSerialData();
  delay(5000);   
  SIM900.println("AT+CIPSTART= \"TCP\",\"184.106.153.149\",\"80\"");  /* Terminate HTTP service */
  delay(2000);
  ShowSerialData();
  delay(5000);
  char digits = CalculateDigit(datadine);
  if(digits == 1)
  {
    SIM900.println("AT+CIPSEND=43"); /* Close GPRS context */
  } 
  else if(digits == 2)
  {
    SIM900.println("AT+CIPSEND=44"); /* Close GPRS context */   
  } 
  else if(digits == 3)
  {
    SIM900.println("AT+CIPSEND=45"); /* Close GPRS context */

  } 
    delay(2000);
    ShowSerialData();
    delay(2000);    
//    unsigned int sensorValue = datadine;
    String stringOne = "GET /update?key=D483VK7ABLHOIMA5&field1=";
    String stringThree = stringOne + datadine;
    SIM900.println(stringThree); /* Close GPRS context */
    delay(2000);
    ShowSerialData();  
    delay(2000);
}

 char CalculateDigit(unsigned int num)
{
    char count=0;
    /* Run loop till num is greater than 0 */
    while(num != 0)
    {
        /* Increment digit count */
        count++;

        /* Remove last digit of 'num' */
        num /= 10;
    }
    return count;
}
