// Uses 3.5V, GND, and digital pin 2 on Arduino Uno

volatile byte REV;       //  VOLATILE DATA TYPE TO STORE REVOLUTIONS
unsigned long int rpm, maxRPM;  //  DEFINE RPM AND MAXIMUM RPM
int RPMlen , prevRPM;  //  INTEGERS TO STORE LED VALUE AND CURRENT RPM AND PREVIOUS RPM
int prevTime = 0; 
int currentTime = 0;
int futureTime = 0; // This variable is used to stop the run time
int idleTime = 1000; // Idle time. This is when the Arduino is doing nothing
int runTime = 500; // Run time. This time interval will be used to calculate the rpm
int ledPin = 12;           //   STATUS LED
int led = 0;

  void setup()
{
     Serial.begin(9600);   // GET VALUES USING SERIAL MONITOR
     //attachInterrupt(0, RPMCount, RISING);     //  ADD A HIGH PRIORITY ACTION ( AN INTERRUPT)  WHEN THE SENSOR GOES FROM LOW TO HIGH
     REV = 0;      //  START ALL THE VARIABLES FROM 0
     rpm = 0;
     pinMode(ledPin, OUTPUT);
     
     Serial.println("TACHOMETER");           //   STARTUP TEXT
     //delay(2000);
     prevTime = millis();
}
 
  void loop()
{
     currentTime = millis();
     if (currentTime - prevTime >= idleTime){
      futureTime = currentTime + runTime;
      prevTime = futureTime; // To prevent relasping, we need to say prevTime is at futureTime for proper idle time
      REV = 0;
      Serial.println ("START MEASURING");
      attachInterrupt(0, RPMCount, RISING);
     }
     if (futureTime - currentTime <=0 && futureTime != 0){
      futureTime = 0;
      detachInterrupt(0);
      rpm = 30*1000/(runTime)*REV; // Rev/second * 60s/1min but we divide by 2 for some reason
      Serial.println ("STOP MEASURING");
      Serial.print ("RPM ");
      Serial.println (rpm);
      // Reset REV
      REV = 0;
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
