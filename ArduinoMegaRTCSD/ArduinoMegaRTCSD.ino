    ////////////////////////////////////////////////////////////////
   //              Dinesh SD Card and RTC Code                   //
  ////////////////////////////////////////////////////////////////
#include <SD.h>
#include <SPI.h>



#include <Wire.h>
#include "RTClib.h"


/* This is the pin assignments*/
int CS_PIN = 53;
const int buttonPin = 5;  

/*This is the variable to save the file in the SD Card*/
File myFile;
RTC_DS1307 RTC;
char* filename = "ctest.txt";

//Variables
int buttonState = 0;
int SWPrestime = 0;
int SWReleatime = 0;
int flag=0;

/*This are the functions for SD CARD*/
void initializeSD();
int createFile(char filename[]);
int writeToFile(char text[]);
void closeFile();
int openFile(char filename[]);
String readLine();



void setup()
{

  Serial.begin(9600);
  // TWI is enabled for RTC
  Wire.begin();
  RTC.begin();
  //RTC.adjust(DateTime(__DATE__, __TIME__));
  // Check to see if the RTC is keeping time.  If it is, load the time from your computer.
  if (! RTC.isrunning()) 
  {
    Serial.println("RTC Date : Compile Time:");
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
        if ( flag == 0){
                  
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
                
                Serial.print("Sitting Time : ");
                Serial.print(SwitchPressed.day(), DEC);
                Serial.print('/');
                Serial.print(SwitchPressed.month(), DEC);
                Serial.print('/');
                Serial.print(SwitchPressed.year(), DEC);
                Serial.print(' ');
                Serial.print(SwitchPressed.hour(), DEC);
                Serial.print(':');
                Serial.print(SwitchPressed.minute(), DEC);
                Serial.print(':');
                Serial.print(SwitchPressed.second(), DEC);
                Serial.print("   "); 
                                
                flag=1; //change flag variable
        }
    }
    else if (buttonState == HIGH && flag == 1){
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
                
                Serial.print("Standing Time : ");
                Serial.print(Switchreleased.day(), DEC);
                Serial.print('/');
                Serial.print(Switchreleased.month(), DEC);
                Serial.print('/');
                Serial.print(Switchreleased.year(), DEC);
                Serial.print(' ');
                Serial.print(Switchreleased.hour(), DEC);
                Serial.print(':');
                Serial.print(Switchreleased.minute(), DEC);
                Serial.print(':');
                Serial.print(Switchreleased.second(), DEC);
                Serial.print("   ");
         myFile = SD.open(filename, FILE_WRITE);
        if (myFile) {               
          myFile.print("Total Sitting Time : ");
        }
        myFile.close();
        
        Serial.print("Total Sitting Time : ");
        
        int sittime = 0;
        sittime = SWReleatime - SWPrestime;
        myFile = SD.open(filename, FILE_WRITE);
        if (myFile) {
            myFile.print(sittime/60, DEC);
            myFile.print(" minutes");
            myFile.println();
               
        }
        myFile.close();
        Serial.print(sittime/60, DEC);
        Serial.print(" minutes");
        Serial.println();
        
        
    }
}
void initializeSD()
{
  Serial.println("Initializing SD card...");
  pinMode(CS_PIN, OUTPUT);

  if (SD.begin(CS_PIN))
  {
    Serial.println("SD card is ready to use.");
  } else
  {
    Serial.println("SD card initialization failed");
    return;
  }
}

int createFile(char filename[])
{
  myFile = SD.open(filename, FILE_WRITE);

  if (myFile)
  {
    Serial.println("File created successfully.");
    return 1;
  } else
  {
    Serial.println("Error while creating file.");
    return 0;
  }
}

int writeToFile(char text[])
{
  if (myFile)
  {
    myFile.println(text);
    Serial.println("Writing to file: ");
    Serial.println(text);
    return 1;
  } else
  {
    Serial.println("Couldn't write to file");
    return 0;
  }
}

void closeFile()
{
  if (myFile)
  {
    myFile.close();
    Serial.println("File closed");
  }
}

int openFile(char filename[])
{
  myFile = SD.open(filename);
  if (myFile)
  {
    Serial.println("File opened with success!");
    return 1;
  } else
  {
    Serial.println("Error opening file...");
    return 0;
  }
}
