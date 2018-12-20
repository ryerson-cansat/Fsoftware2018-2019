/*
* Date and time functions using a DS3231 RTC connected via I2C and Wire lib
* I2C address of 0x68
* DS3231 Model
* SDA - A4
* SCL - A5
*/ 
#include <Wire.h>
#include <RTClib.h>
RTC_DS3231 rtc;

//char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

void setupRTC(){
  rtc.adjust(DateTime(2018, 11, 13, 0, 0, 0));
}

int getTime(){
  DateTime now = rtc.now();
  return ((int)now.second() + 60*(int)now.minute() + 3600*(int)now.hour());
}

void setup () 
{
  Serial.begin(9600);
  if (! rtc.begin()) 
  {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));//auto update from computer time
    //rtc.adjust(DateTime(2018, 11, 13, 13, 40, 0));// to set the time manualy 
  setupRTC();
}

void loop () 
{
    Serial.println(getTime());
    delay(1000);
/*    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(" ,");
    Serial.print(now.day());
    Serial.print('/');
    Serial.print(now.month());
    Serial.print('/');
    Serial.print(now.year());
    Serial.println ("");
   */
}
