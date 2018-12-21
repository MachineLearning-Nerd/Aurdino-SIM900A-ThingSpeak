#include <SoftwareSerial.h>
//Create software serial object to communicate with SIM900A

// SIM900A's TXD = ARDUINO's #3
// SIM900A's RXD = ARDUINO's #2 
/* Create object named SIM900A of the class SoftwareSerial */
SoftwareSerial SIM900(3, 2);

void setup() {
  SIM900.begin(4800);	/* Define baud rate for software serial communication */
  Serial.begin(4800);	/* Define baud rate for serial communication */

  Serial.println("HTTP post method :");
  Serial.print("AT\r\n");
  SIM900.println("AT"); /* Check Communication */
  delay(2000);
  ShowSerialData();  /* Print response on the serial monitor */
  delay(2000);
  /* Configure bearer profile 1 */
  Serial.print("AT+CPIN?\r\n");
  SIM900.println("AT+CPIN?"); /* Connection type GPRS */
  delay(2000);
  ShowSerialData();
  delay(2000);
  Serial.print("AT+CREG?\r\n");
  SIM900.println("AT+CREG?"); /* APN of the provider */
  delay(2000);
  ShowSerialData();
  delay(2000);
  Serial.print("AT+CGATT?\r\n");
  SIM900.println("AT+CGATT?");  /* Open GPRS context */
  delay(2000);
  ShowSerialData();
  delay(2000);
  Serial.print("AT+CIPSHUT\r\n");
  SIM900.println("AT+CIPSHUT"); /* Query the GPRS context */
  delay(2000);
  ShowSerialData();
  delay(2000);
  Serial.print("AT+CIPSTATUS\r\n");
  SIM900.println("AT+CIPSTATUS"); /* Initialize HTTP service */
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
  delay(2000);
  Serial.print("AT+CIPSTART= \"TCP\",\"184.106.153.149\",\"80\"\r\n");  
  SIM900.println("AT+CIPSTART= \"TCP\",\"184.106.153.149\",\"80\"");  /* Terminate HTTP service */
  delay(2000);
  ShowSerialData();
  delay(2000);
  Serial.print("AT+CIPSEND=44\r\n");
  SIM900.println("AT+CIPSEND=44"); /* Close GPRS context */
  delay(2000);
  ShowSerialData();
  delay(2000);
  Serial.print("GET /update?key=D483VK7ABLHOIMA5&field1=32\r\n");
  SIM900.println("GET /update?key=D483VK7ABLHOIMA5&field1=32"); /* Close GPRS context */
  delay(2000);
  ShowSerialData();
  delay(2000);

}

void loop() {

}

void ShowSerialData()
{
  while(SIM900.available()!=0)	/* If data is available on serial port */
  Serial.write(char (SIM900.read()));	/* Print character received on to the serial monitor */
}
