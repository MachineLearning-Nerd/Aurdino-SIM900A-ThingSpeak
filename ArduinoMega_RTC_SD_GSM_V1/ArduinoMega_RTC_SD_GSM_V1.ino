#include <string.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_BMP085.h>
#include <Wire.h>

// You will need to create an SFE_BMP180 object, here called "pressure":

Adafruit_BMP085 pressure;
double baseline; // baseline pressure

class DateTime 
{ 
    // Access specifier 
    public: 
  
    // Data Members 
    unsigned int years=0; 
    unsigned int months=0; 
    unsigned int days=0; 
    unsigned int hours=0; 
    unsigned int minutes=0; 
    unsigned int seconds=0; 
    unsigned int timeinsec=0;  
    char error=0;
}; 

#define buttonPin  5

////Variables
int buttonState = 0;
unsigned int SWPrestime = 0;
unsigned int SWReleatime = 0;
int flag = 0;
#define pressurethreshold 101900

void GSMinitialization();
char ShowSerialData();
char SendData(unsigned int datadine);
char CalculateDigit(unsigned int num);
void SleepGSM(void);
void UnsleepGSM(void);
 
DateTime getTimeStamp();
unsigned int CalcDays(int iMonth, int iDay, int iYear);

// SD Card Initialization
File myFile;
const int chipSelect = 53;
char* filename = "ctest.txt";
/*This are the functions for SD CARD*/
void initializeSD();
char createFile(char filename[]);
int writeToFile(char text[]);
void closeFile();
char openFile(char filename[]);


void setup() {
  Serial1.begin(4800);  /* Define baud rate for software serial communication */
  Serial.begin(115200); /* Define baud rate for serial communication */
  if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.

    Serial.println("BMP180 init fail (disconnected?)\n\n");
    while(1); // Pause forever.
  }

  // Get the baseline pressure:
  
  baseline = pressure.readPressure();
  Serial.print("baseline pressure: ");
  Serial.print(baseline);
  Serial.println(" mb");

  
  // Button initialization
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);       // turn on pullup resistors

  initializeSD();

  createFile(filename);
  closeFile();
  delay(2000);
  
  Serial.println("HTTP post method :");
  Serial.print("AT\r\n");
  Serial1.println("AT"); /* Check Communication */
  delay(2000);
  ShowSerialData();  /* Print response on the serial monitor */
  delay(2000);
  Serial.print("AT+CIPSHUT\r\n");
  Serial1.println("AT+CIPSHUT"); /* Query the GPRS context */
  delay(2000);
  ShowSerialData();
  delay(2000);
  Serial.print("AT+CIPMUX=0\r\n");
  Serial1.println("AT+CIPMUX=0");  /* Set parameters for HTTP session */
  delay(2000);
  ShowSerialData();
  delay(2000);
  Serial.print("AT+CGDCONT=1,\"IP\",\"airtelgprs.com\"\r\n");
  Serial1.println("AT+CGDCONT=1,\"IP\",\"airtelgprs.com\""); /* Set parameters for HTTP session */
  delay(2000);
  ShowSerialData();
  delay(2000);
  Serial.print("AT+CSTT=\"airtelgprs.com\",\"\",\"\"\r\n");
  Serial1.println("AT+CSTT=\"airtelgprs.com\",\"\",\"\""); /* POST data of size 33 Bytes with maximum latency time of 10seconds for inputting the data*/
  delay(2000);
  ShowSerialData();
  delay(2000);
  Serial.print("AT+CIICR\r\n"); /* Data to be sent */
  Serial1.println("AT+CIICR");
  delay(2000);
  ShowSerialData();
  delay(2000);
  Serial.print("AT+CIFSR\r\n");
  Serial1.println("AT+CIFSR"); /* Start POST session */
  delay(2000);
  ShowSerialData();
  delay(2000);
  Serial.print("AT+CCLK?\r\n");
  Serial1.println("AT+CCLK?"); /* Start POST session */
  delay(2000);
  ShowSerialData();
  
  flag = 0;
}

void loop() {
  double DifferencePressure,PressureState = 0;
  
  // Get a new pressure reading:

  PressureState = pressure.readPressure();


  //Read button state (pressed or not pressed?)
  buttonState = digitalRead(buttonPin);
  DifferencePressure = PressureState ;
//  Serial.print("baseline pressure: ");
//  Serial.print(PressureState);
//  Serial.println(" Pa"); 
  //If button pressed...
//  delay(500);
  if (DifferencePressure > pressurethreshold) {
//    Serial.print("baseline pressure: ");
//    Serial.print(DifferencePressure);
//    Serial.println(" mb");  
    
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW

    //...ones, turn led on!
    if ( flag == 0) {

      //Serial.println("buttonState is low");

      DateTime SwitchPressed = getTimeStamp();
      SWPrestime = SwitchPressed.timeinsec;

      myFile = SD.open(filename, FILE_WRITE);
      if (myFile) {
        myFile.print("Sitting Time : ");
        myFile.print(SwitchPressed.days, DEC);
        myFile.print('/');
        myFile.print(SwitchPressed.months, DEC);
        myFile.print('/');
        myFile.print(SwitchPressed.years, DEC);
        myFile.print(' ');
        myFile.print(SwitchPressed.hours, DEC);
        myFile.print(':');
        myFile.print(SwitchPressed.minutes, DEC);
        myFile.print(':');
        myFile.print(SwitchPressed.seconds, DEC);
        myFile.print(",   ");
      }

      myFile.close();
            
      Serial.print("Sitting Time : ");
      Serial.print(SwitchPressed.days, DEC);
      Serial.print('/');
      Serial.print(SwitchPressed.months, DEC);
      Serial.print('/');
      Serial.print(SwitchPressed.years, DEC);
      Serial.print(' ');
      Serial.print(SwitchPressed.hours, DEC);
      Serial.print(':');
      Serial.print(SwitchPressed.minutes, DEC);
      Serial.print(':');
      Serial.print(SwitchPressed.seconds, DEC);
      Serial.print(",   ");

      flag = 1; //change flag variable
    }
  }
  else if (DifferencePressure < pressurethreshold && flag == 1) {
      flag = 0;
      delay(1000);
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    
      DateTime Switchreleased = getTimeStamp();
      SWReleatime = Switchreleased.timeinsec;

      myFile = SD.open(filename, FILE_WRITE);
      if (myFile) {
        myFile.print("Standing Time : ");
        myFile.print(Switchreleased.days, DEC);
        myFile.print('/');
        myFile.print(Switchreleased.months, DEC);
        myFile.print('/');
        myFile.print(Switchreleased.years, DEC);
        myFile.print(' ');
        myFile.print(Switchreleased.hours, DEC);
        myFile.print(':');
        myFile.print(Switchreleased.minutes, DEC);
        myFile.print(':');
        myFile.print(Switchreleased.seconds, DEC);
        myFile.print(",   ");
      }

      myFile.close();
      
    Serial.print("Standing Time : ");
    Serial.print(Switchreleased.days, DEC);
    Serial.print('/');
    Serial.print(Switchreleased.months, DEC);
    Serial.print('/');
    Serial.print(Switchreleased.years, DEC);
    Serial.print(' ');
    Serial.print(Switchreleased.hours, DEC);
    Serial.print(':');
    Serial.print(Switchreleased.minutes, DEC);
    Serial.print(':');
    Serial.print(Switchreleased.seconds, DEC);
    Serial.print(",   ");
    
    Serial.print("Total Sitting Time : ");

    myFile = SD.open(filename, FILE_WRITE);
    if (myFile) {
      myFile.print("Total Sitting Time : ");
    }
    myFile.close();

    unsigned int sittime = 0;
    sittime = SWReleatime - SWPrestime;

    myFile = SD.open(filename, FILE_WRITE);
    if (myFile) {
      myFile.print(sittime / 60, DEC);
      myFile.print(" minutes,");
      myFile.println();

    }
    myFile.close();


    Serial.print(sittime / 60, DEC);
    Serial.print(" minutes");
    Serial.println();

    unsigned int minut = sittime / 60;
    
    UnsleepGSM();
    char mistake = 1;
    while(mistake != 0){
        mistake  = SendData(minut);
      }
    SleepGSM();
    
  }
}

char ShowSerialData()
{
  char myConcatenation[2]={'0','0'};
  myConcatenation[0] = '0';
  myConcatenation[1] = '0';
  char index = 0;
  char error = 0;
  while (Serial1.available() != 0) /* If data is available on serial port */
  {
    char pchar = char(Serial1.read());
    if (pchar == 'E' || myConcatenation[0] == 'E')
    {
      myConcatenation[index] = pchar;
      index = index + 1;      
      if (myConcatenation[1] == 'R')
      {
        Serial.println("There is an ERROR"); 
        error = 1; 
      }
      else if(myConcatenation[1] != '0' && myConcatenation[1] != 'R')
      {
        index = 0;
        myConcatenation[0] = '0';
        myConcatenation[1] = '0';
      
      }      
    }
    Serial.write(pchar); /* Print character received on to the serial monitor */
    
  }
  return error;

}


char SendData(unsigned int data)
{
  char error = 0;
  delay(2000);
  Serial1.println("AT+CIPSHUT"); /* Query the GPRS context */
  delay(2000);
  error = ShowSerialData();
  if (error == 1)
  {
    Serial.println("There is an ERROR");
    return error;
  }
  delay(5000);   
  Serial1.println("AT+CIPSTART= \"TCP\",\"184.106.153.149\",\"80\"");  /* Terminate HTTP service */
  delay(2000);
  error = ShowSerialData();
  if (error == 1)
  {
    Serial.println("There is an ERROR");
    return error;
  }
  delay(5000);
  char digits = CalculateDigit(data);
  if(digits == 1)
  {
    Serial1.println("AT+CIPSEND=43"); /* Close GPRS context */
  } 
  else if(digits == 2)
  {
    Serial1.println("AT+CIPSEND=44"); /* Close GPRS context */   
  } 
  else if(digits == 3)
  {
    Serial1.println("AT+CIPSEND=45"); /* Close GPRS context */

  } 

    
  delay(2000);
  error = ShowSerialData();
  if (error == 1)
  {
    Serial.println("There is an ERROR");
    return error;
  }
  delay(2000);    
  unsigned int sensorValue = data;
  String stringOne = "GET /update?key=3ACH418J0ADT3H0J&field1=";
  String stringThree = stringOne + sensorValue;
  Serial1.println(stringThree); /* Close GPRS context */
  delay(3000);
  error = ShowSerialData();
  if (error == 1)
  {
    Serial.println("There is an ERROR");
    return error;
  }  
  delay(5000);
  return 0;
}

char CalculateDigit(unsigned int num)
{
    int count=0;
    if(num==0){
      count = 1;
      return count;  
    }
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

void SleepGSM(void)
{
  Serial1.println("AT+CSCLK=2"); /* Query the GPRS context */
  delay(2000);
  ShowSerialData();
}

void UnsleepGSM(void)
{
  Serial1.println("AT"); /* Query the GPRS context */
  delay(2000);
  ShowSerialData();
  Serial1.println("AT+CSCLK=0"); /* Query the GPRS context */
  delay(2000);
  ShowSerialData();
}


DateTime getTimeStamp(){
  char  timeStamp[50];
  static DateTime switchtime;
  delay(2000);
  Serial1.println("AT+CCLK?"); /* Start POST session */
  delay(2000);
  ShowSerialData();
  
  delay(2000);
  Serial1.println("AT+CLTS=1"); //Check whether it has registered in the network
  delay(2000);
  ShowSerialData();
  
  delay(2000);
  Serial1.println("AT+CCLK?"); /* Start POST session */
  delay(2000);
  
  if (Serial1.available()>0){
   int i = 0;
           while (Serial1.available()>0){
                 char temp = Serial1.read();
                 timeStamp[i]= temp;
  //                 Serial.print(i,DEC);
  //                 Serial.write(temp);
              i++;              
           }
  
       }
  //  Serial1.println("AT+CLTS=0"); //Check whether it has registered in the network
  //  updateSerial();
  char indexd = 20;
  switchtime.years = (((timeStamp[indexd])-48)*10)+((timeStamp[indexd+1])-48);
  int years = (((timeStamp[indexd])-48)*10)+((timeStamp[indexd+1])-48);
  
  switchtime.months = (((timeStamp[indexd+3])-48)*10)+((timeStamp[indexd+4])-48);
  switchtime.days  = (((timeStamp[indexd+6])-48)*10)+((timeStamp[indexd+7])-48);
  switchtime.hours = (((timeStamp[indexd+9])-48)*10)+((timeStamp[indexd+10])-48);
  switchtime.minutes = (((timeStamp[indexd+12])-48)*10)+((timeStamp[indexd+13])-48);
  switchtime.seconds = (((timeStamp[indexd+15])-48)*10)+((timeStamp[indexd+16])-48);
  unsigned int totaldays = CalcDays(switchtime.months, switchtime.days,  switchtime.years);
  switchtime.timeinsec = switchtime.seconds+switchtime.minutes*60+switchtime.hours*3600+totaldays*3600*7;
  return switchtime;
}


unsigned int CalcDays(int iMonth, int iDay, int iYear)
{
    unsigned long startdate = (1 + (153 * (1 + 12 * ((14 - 1) / 12) - 3) + 2) / 5 + 365 *
        (19 + 4800 - ((14 - 1) / 12)) + (19 + 4800 - ((14 - 1) / 12)) / 4 - 32083);
    unsigned long lastdate =   (iDay + (153 * (iMonth + 12 * ((14 - iMonth) / 12) - 3) + 2) / 5 + 365 *
        (iYear + 4800 - ((14 - iMonth) / 12)) + (iYear + 4800 - ((14 - iMonth) / 12)) / 4 - 32083);
        
    return (lastdate-startdate);
}


void initializeSD()
{
  Serial.println("Initializing SD card...");
  pinMode(chipSelect, OUTPUT);

  if (SD.begin(chipSelect))
  {
    Serial.println("SD card is ready to use.");
  } else
  {
    Serial.println("SD card initialization failed");
    return;
  }
}

char createFile(char filename[])
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

char openFile(char filename[])
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



void GSMinitialization()
{
    Serial.println("HTTP post method :");
    Serial.print("AT\r\n");
    Serial1.println("AT"); /* Check Communication */
    delay(2000);
    ShowSerialData();  /* Print response on the serial monitor */
    delay(2000);
    Serial.print("AT+CIPSHUT\r\n");
    Serial1.println("AT+CIPSHUT"); /* Query the GPRS context */
    delay(2000);
    ShowSerialData();
    delay(2000);
    Serial.print("AT+CIPMUX=0\r\n");
    Serial1.println("AT+CIPMUX=0");  /* Set parameters for HTTP session */
    delay(2000);
    ShowSerialData();
    delay(2000);
    Serial.print("AT+CGDCONT=1,\"IP\",\"airtelgprs.com\"\r\n");
    Serial1.println("AT+CGDCONT=1,\"IP\",\"airtelgprs.com\""); /* Set parameters for HTTP session */
    delay(2000);
    ShowSerialData();
    delay(2000);
    Serial.print("AT+CSTT=\"airtelgprs.com\",\"\",\"\"\r\n");
    Serial1.println("AT+CSTT=\"airtelgprs.com\",\"\",\"\""); /* POST data of size 33 Bytes with maximum latency time of 10seconds for inputting the data*/
    delay(2000);
    ShowSerialData();
    delay(2000);
    Serial.print("AT+CIICR\r\n"); /* Data to be sent */
    Serial1.println("AT+CIICR");
    delay(2000);
    ShowSerialData();
    delay(2000);
    Serial.print("AT+CIFSR\r\n");
    Serial1.println("AT+CIFSR"); /* Start POST session */
    delay(2000);
    ShowSerialData();
    delay(2000);
    Serial.print("AT+CCLK?\r\n");
    Serial1.println("AT+CCLK?"); /* Start POST session */
    delay(2000);
    ShowSerialData();
}
