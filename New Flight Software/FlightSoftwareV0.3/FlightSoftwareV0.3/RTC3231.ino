/*
* Date and time functions using a DS1307 RTC connected via I2C and Wire lib
* I2C address of 0x68
* 
*/

 void setupRTC(){
  rtc.begin();
  if (hasReset == false){
      rtc.setTime(0,0,0,0,0,0,0);
  }
}

int getSeconds(){
  rtc.update();
  return ((int)rtc.second() + 60*(int)rtc.minute() + 3600*(int)rtc.hour());
}
