/*
* Date and time functions using a DS1307 RTC connected via I2C and Wire lib
* I2C address of 0x68
* DS3231 Model
* 5V
* SDA - A4
* SCL - A5
*/ 

void setupRTC(){
  rtc.begin();
  if (hasReset == false){
      rtc.adjust(DateTime(2019, 1, 5, 0, 0, 0));
  }
}

int getSeconds(){
  DateTime now = rtc.now();
  return ((int)now.second() + 60*(int)now.minute() + 3600*(int)now.hour());
}
