#include <SoftwareSerial.h>
#include <Wire.h>
#include "RTClib.h"
#include <string.h>

//Create software serial object to communicate with SIM900A

// SIM900A's TXD = ARDUINO's #3
// SIM900A's RXD = ARDUINO's #2
/* Create object named SIM900A of the class SoftwareSerial */
SoftwareSerial SIM900(7, 6);

#define buttonPin  5

RTC_DS1307 RTC;

////Variables
int buttonState = 0;
int SWPrestime = 0;
int SWReleatime = 0;
int flag = 0;


 void GSMinitialization();
 char ShowSerialData();
 char SendData(unsigned int datadine);
 char CalculateDigit(unsigned int num);
 void SleepGSM(void);
 void UnsleepGSM(void);
void setup() {
  SIM900.begin(4800);  /* Define baud rate for software serial communication */
  Serial.begin(115200); /* Define baud rate for serial communication */

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
 
  Serial.println("HTTP post method :");
  Serial.print("AT\r\n");
  SIM900.println("AT"); /* Check Communication */
  delay(2000);
  ShowSerialData();  /* Print response on the serial monitor */
  delay(2000);
  Serial.print("AT+CIPSHUT\r\n");
  SIM900.println("AT+CIPSHUT"); /* Query the GPRS context */
  delay(2000);
  ShowSerialData();
  delay(2000);
  Serial.print("AT+CIPMUX=0\r\n");
  SIM900.println("AT+CIPMUX=0");  /* Set parameters for HTTP session */
  delay(2000);
  ShowSerialData();
  delay(2000);
  Serial.print("AT+CGDCONT=1,\"IP\",\"airtelgprs.com\"\r\n");
  SIM900.println("AT+CGDCONT=1,\"IP\",\"airtelgprs.com\""); /* Set parameters for HTTP session */
  delay(2000);
  ShowSerialData();
  delay(2000);
  Serial.print("AT+CSTT=\"airtelgprs.com\",\"\",\"\"\r\n");
  SIM900.println("AT+CSTT=\"airtelgprs.com\",\"\",\"\""); /* POST data of size 33 Bytes with maximum latency time of 10seconds for inputting the data*/
  delay(2000);
  ShowSerialData();
  delay(2000);
  Serial.print("AT+CIICR\r\n"); /* Data to be sent */
  SIM900.println("AT+CIICR");
  delay(2000);
  ShowSerialData();
  delay(2000);
  Serial.print("AT+CIFSR\r\n");
  SIM900.println("AT+CIFSR"); /* Start POST session */
  delay(2000);
  ShowSerialData();

//  SendData(5);
//  SendData(85);
//  SendData(185);

  flag = 0;
}

void loop() {

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

      flag = 1; //change flag variable
    }
  }
  else if (buttonState == HIGH && flag == 1) {
    flag = 0;
    delay(1000);
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)

    DateTime Switchreleased = RTC.now();
    SWReleatime = Switchreleased.unixtime();

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

    Serial.print("Total Sitting Time : ");

    unsigned int sittime = 0;
    sittime = SWReleatime - SWPrestime;
    Serial.print(sittime / 60, DEC);
    Serial.print(" minutes");
    Serial.println();

    unsigned int minut = sittime / 60;
    Serial.println(minut);
    Serial.println("Minutes");
    
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
  while (SIM900.available() != 0) /* If data is available on serial port */
  {
    char pchar = char(SIM900.read());
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
  SIM900.println("AT+CIPSHUT"); /* Query the GPRS context */
  delay(2000);
  error = ShowSerialData();
  if (error == 1)
  {
    Serial.println("There is an ERROR");
    return error;
  }
  delay(5000);   
  SIM900.println("AT+CIPSTART= \"TCP\",\"184.106.153.149\",\"80\"");  /* Terminate HTTP service */
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
  error = ShowSerialData();
  if (error == 1)
  {
    Serial.println("There is an ERROR");
    return error;
  }
  delay(2000);    
  unsigned int sensorValue = data;
  String stringOne = "GET /update?key=D483VK7ABLHOIMA5&field2=";
  String stringThree = stringOne + sensorValue;
  SIM900.println(stringThree); /* Close GPRS context */
  delay(2000);
  error = ShowSerialData();
  if (error == 1)
  {
    Serial.println("There is an ERROR");
    return error;
  }  
  delay(2000);
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
  SIM900.println("AT+CSCLK=2"); /* Query the GPRS context */
  delay(2000);
  ShowSerialData();
}

void UnsleepGSM(void)
{
  SIM900.println("AT"); /* Query the GPRS context */
  delay(2000);
  ShowSerialData();
  SIM900.println("AT+CSCLK=0"); /* Query the GPRS context */
  delay(2000);
  ShowSerialData();
}
