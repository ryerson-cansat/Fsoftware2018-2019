/*
* BMP 280 from Adafruit
* Measures Altitude, Temperature, and Pressure
* Vin - 5V or 3.3V
* GND - Ground
* SCK - SCL or A5
* SDI - SDA or A4
* Pull-up Resistors - Connect Voltage to SDK and SDI using a resistor (~10kohm)
* I2C address of 0x0C and 0x77
*/

float seaLevelPressure;

void setupBMP(){
  bmp.begin();
  // Sea Level Pressure must be in hPa for the readAltitude() function
  seaLevelPressure = bmp.readPressure()/100;
}

float getTemp(){
  return bmp.readTemperature();
}

float getAlt(){
  return bmp.readAltitude(seaLevelPressure); // Takes pressure in hPa
}

float getPressure(){
  return bmp.readPressure();
}
