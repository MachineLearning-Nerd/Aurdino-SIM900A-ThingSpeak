#include <SoftwareSerial.h>
//Create software serial object to communicate with SIM900A

// SIM900A's TXD = ARDUINO's #3
// SIM900A's RXD = ARDUINO's #2
/* Create object named SIM900A of the class SoftwareSerial */
SoftwareSerial SIM900(7, 6);

void setup() {
  SIM900.begin(4800);	/* Define baud rate for software serial communication */
  Serial.begin(4800);	/* Define baud rate for serial communication */

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

  SendData(5);
  SendData(85);
  SendData(185);


}

void loop() {

}

void ShowSerialData()
{
  while (SIM900.available() != 0)	/* If data is available on serial port */
  {
    Serial.write(char (SIM900.read()));	/* Print character received on to the serial monitor */
  }
//  Serial.println(" ");
}


void SendData(int data)
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
  int digits = CalculateDigit(data);
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
    int sensorValue = data;
    String stringOne = "GET /update?key=D483VK7ABLHOIMA5&field1=";
    String stringThree = stringOne + sensorValue;
    SIM900.println(stringThree); /* Close GPRS context */
    delay(2000);
    ShowSerialData();  
    delay(2000);
}

int CalculateDigit(int num)
{
    int count=0;
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
