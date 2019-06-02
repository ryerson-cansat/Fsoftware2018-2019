/*
* BMP 280 from Adafruit
* Measures Altitude, Temperature, and Pressure
* Vin - 5V or 3.3V
* GND - Ground
* SCK - SCL
* SDI - SDA
* Pull-up Resistors - Connect Voltage to SDK and SDI using a resistor (~10kohm)
* I2C address of 0x0C and 0x77
*/

float alt= 0;
int counter = 0;

void setupBMP(){
  bmp.begin();
  // Sea Level Pressure must be in hPa for the readAltitude() function
  seaLevelPressure = bmp.readPressure()/100; //Base line pressure to be used to calculate altitude.
  //seaLevelPressure = 1022;
}

float getTemp(){
  return bmp.readTemperature();
}

float getAlt(){
  if (alt < 75 && counter == 0){
    alt++;
  }
  else{
    counter = 1;
    alt--;
  }
  return alt;
  //return bmp.readAltitude(seaLevelPressure); // Takes pressure in hPa
}

float getPressure(){
  return bmp.readPressure();
}
