class DateTime 
{ 
    // Access specifier 
    public: 
  
    // Data Members 
    unsigned int years; 
    unsigned int months; 
    unsigned int days; 
    unsigned int hours; 
    unsigned int minutes; 
    unsigned int seconds; 
    unsigned int timeinsec;  
}; 

DateTime getTimeStamp();

void setup()
{
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(4800);
  
  //Begin serial communication with Arduino and SIM800L
  Serial1.begin(4800);

  Serial.println("Initializing...");
  delay(1000);

  Serial1.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  Serial1.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  updateSerial();
  Serial1.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  updateSerial();
  Serial1.println("AT+CREG?"); //Check whether it has registered in the network
  updateSerial();
  delay(1000);
  
  DateTime firsttime = getTimeStamp();
  Serial.print("First Seconds:");
  Serial.println(firsttime.timeinsec,DEC);
  
  delay(2000);
  DateTime secondtime = getTimeStamp();
  Serial.print("Second Seconds:");
  Serial.println(secondtime.timeinsec,DEC);
  
  unsigned int difference = secondtime.timeinsec - firsttime.timeinsec;
  Serial.print("Minutes:");
  Serial.println(difference/60);
//  Serial1.println("AT+COPS=?"); //Check whether it has registered in the network
//  updateSerial();
//  Serial1.println("AT+CBC"); //Check whether it has registered in the network
//  updateSerial();
//  Serial1.println("AT+CMGF=1"); // Configuring TEXT mode
//  updateSerial();
}

void loop()
{
//  updateSerial();
    if (Serial1.available()){
        Serial.write(Serial1.read());
      }
    
  if (Serial.available()){
      Serial1.write(Serial.read());  
    }
    
}

void updateSerial()
{
  delay(1000);
  while (Serial.available()) 
  {
    Serial1.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(Serial1.available()) 
  {
    Serial.write(Serial1.read());//Forward what Software Serial received to Serial Port
  }
}

DateTime getTimeStamp(){
 char  timeStamp[50];
 DateTime switchtime;
 Serial1.println("AT+CLTS=1"); //Check whether it has registered in the network
 updateSerial();
 Serial1.print("AT+CCLK?");      //SIM900 AT command to get time stamp
 Serial1.write(13);
 delay(2000);
 if (Serial1.available()>0){
   int i = 0;
           while (Serial1.available()>0){
                 char temp = Serial1.read();
                 timeStamp[i]= temp;
//                 Serial.print(i);
//                 Serial.print(temp);
              i++;              
           }

       }
//  Serial1.println("AT+CLTS=0"); //Check whether it has registered in the network
//  updateSerial();
  switchtime.years = (((timeStamp[19])-48)*10)+((timeStamp[20])-48);
  switchtime.months = (((timeStamp[22])-48)*10)+((timeStamp[23])-48);
  switchtime.days  = (((timeStamp[25])-48)*10)+((timeStamp[26])-48);
  switchtime.hours = (((timeStamp[28])-48)*10)+((timeStamp[29])-48);
  switchtime.minutes = (((timeStamp[31])-48)*10)+((timeStamp[32])-48);
  switchtime.seconds = (((timeStamp[34])-48)*10)+((timeStamp[35])-48);
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
