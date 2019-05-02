volatile byte REV;       //  VOLATILE DATA TYPE TO STORE REVOLUTIONS
unsigned long int rpm, maxRPM;  //  DEFINE RPM AND MAXIMUM RPM
unsigned long time;         //  DEFINE TIME TAKEN TO COVER ONE REVOLUTION
int ledPin = 12;           //   STATUS LED
int led = 0,RPMlen , prevRPM;  //  INTEGERS TO STORE LED VALUE AND CURRENT RPM AND PREVIOUS RPM
long prevtime = 0;       //  STORE IDLE TIME TO TOGGLE MENU
    
  void setup()
{
     Serial.begin(9600);   // GET VALUES USING SERIAL MONITOR
     attachInterrupt(0, RPMCount, RISING);     //  ADD A HIGH PRIORITY ACTION ( AN INTERRUPT)  WHEN THE SENSOR GOES FROM LOW TO HIGH
     REV = 0;      //  START ALL THE VARIABLES FROM 0
     rpm = 0;
     time = 0;
     pinMode(ledPin, OUTPUT);
     pinMode(3, OUTPUT);                
     pinMode(4, OUTPUT);
     digitalWrite(3, HIGH);             //  VCC PIN FOR SENSOR
     digitalWrite(4, LOW);              // GND PIN FOR SENSOR
     Serial.println("TACHOMETER");           //   STARTUP TEXT
     delay(2000);
}
 
  void loop()
{
     long currtime = millis();                 // GET CURRENT TIME
     long idletime = currtime - prevtime;        //  CALCULATE IDLE TIME
     if(REV >= 5 )                  //  IT WILL UPDATE AFETR EVERY 5 READINGS
   {          
     Serial.println("SENSOR MEASURING1");
     
     rpm = 30*1000/(millis() - time)*REV;       //  CALCULATE  RPM USING REVOLUTIONS AND ELAPSED TIME
     
     if(rpm > maxRPM)
     maxRPM = rpm;                             //  GET THE MAX RPM THROUGHOUT THE RUN
    
     time = millis();                            
     REV = 0;
     
     int x = rpm;                                //  CALCULATE NUMBER OF DIGITS IN RPM
     while(x!=0)
     {
       x = x/10;
       RPMlen++;
     }           
         
     if(RPMlen!=prevRPM)                             // IF THE RPM FALLS TO A LOWER NUMBER WITH LESS DIGITS , THE LCD WILL GET CLEARED
     {
       prevRPM = RPMlen;
       Serial.println("SENSOR MEASURING2");
     }
     Serial.print ("RPM");
     Serial.println(rpm);                        //  PRINT RPM IN DECIMAL SYSTEM
     delay(500);
     
     prevtime = currtime;                        // RESET IDLETIME
   }
   
   if(idletime > 5000 )                      //  IF THERE ARE NO READING FOR 5 SEC , THE SCREEN WILL SHOW MAX RPM
   {     
     Serial.print("MAXIMUM RPM");
     Serial.print(maxRPM,DEC);                     // DISPLAY MAX RPM
     Serial.println("   RPM");
     delay(2000);
     Serial.print("IDLE STATE");
     Serial.println("READY TO MEASURE");
     delay(2000);
     prevtime = currtime;
  }
}
 
 void RPMCount()                                // EVERYTIME WHEN THE SENSOR GOES FROM LOW TO HIGH , THIS FUNCTION WILL BE INVOKED 
{
   REV++;                                         // INCREASE REVOLUTIONS
      if (led == LOW)
   {
      led = HIGH;                                 //  TOGGLE STATUS LED
   } 
      else
   {
     led = LOW;
   }
   digitalWrite(ledPin, led);
}
//////////////////////////////////////////////////////////////  END OF THE PROGRAM  ///////////////////////////////////////////////////////////////////////
