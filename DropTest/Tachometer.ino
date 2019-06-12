// Uses Digital Pin 2 on Arduino UNO but can be changed for different microcontrollers
// For Teensy 3.5, all digital pins have interrupt capability
// 3.5V and GND and a digital pin

volatile byte REV;       //  VOLATILE DATA TYPE TO STORE REVOLUTIONS
unsigned long int rpm;
int futureTime = 0; 
int prevTime = 0; 
int currentTime = 0;
int idleTime = 1000; // Idle time. This is when the Arduino is doing nothing
int runTime = 1000; // Run time. This time interval will be used to calculate the rpm

void setupTachometer (){
  REV = 0;
  rpm = 0;
}

void getRPM (){
  currentTime = teleTime; // We don't use millis() here so we match up with the other telemetry timing
  if (currentTime - prevTime >= idleTime){
      futureTime = currentTime + runTime;
      prevTime = futureTime; // To prevent relasping, we need to say prevTime is at futureTime for proper idle time
      REV = 0;
      attachInterrupt(0, RPMCount, RISING); // If not using Arduino Uno, change 0 to digitalPinToInterrupt(x) where x is a digital pin
  }
  if (futureTime - currentTime <=0 && futureTime != 0){ // When the current time has exceeded the expected future time, we stop
      futureTime = 0;
      detachInterrupt(0);
      rpm = 30*1000/(runTime)*REV; // Rev/second * 60s/1min but we divide by 2 for some reason
      TeleArray[TeleRPM] = rpm;
      // Reset REV
      REV = 0;
  }
}

 void RPMCount()                                // EVERYTIME WHEN THE SENSOR GOES FROM LOW TO HIGH , THIS FUNCTION WILL BE INVOKED 
{
   REV++;                                         // INCREASE REVOLUTIONS
}
