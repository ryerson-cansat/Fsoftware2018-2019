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

float alt2 = 0;
boolean check = false;

void setupBMP(){
  bmp.begin();
  // Sea Level Pressure must be in hPa for the readAltitude() function
  seaLevelPressure = bmp.readPressure()/100; //Base line pressure to be used to calculate altitude.
  //seaLevelPressure = 1022;
}

float getTemp(){
  return bmp.readTemperature();
}

/*
 * returns a float of the altitude
 * can use the bmp altitude or testing altitude, which simply rises and falls
 */
float getAlt(){
  // For real altitude use this line
  return bmp.readAltitude(seaLevelPressure); // Takes pressure in hPa

  // For testing altitude, use this bottom block
//  if(alt2==70)
//  {
//   check = true;
//   alt2--; 
//  }
//  if(alt2<70)
//  {
//    if (check == false)
//    {
//      alt2++;
//    }
//    else
//    {
//      alt2--;
//    }
//  }
//  
//  return alt2;
}

float getPressure(){
  return bmp.readPressure();
}
