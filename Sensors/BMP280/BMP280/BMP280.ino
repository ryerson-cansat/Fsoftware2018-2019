/*
* BMP 280 from Adafruit
* Measures Altitude, Temperature, and Pressure
* Vin - 5V or 3V
* GND - Ground
* SCK - SCL or A5
* SDI - SDA or A4
* Pull-up Resistors - Connect Voltage to SDK and SDI using a resistor (~10kohm)
*/
#include <Wire.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp; 
float initialPressure;

void setupBMP(){
  initialPressure = bmp.readPressure()/100;
}

float getTemp(){
  return bmp.readTemperature();
}

float getAlt(){
  return bmp.readAltitude(initialPressure);
}

float getPressure(){
  return bmp.readPressure();
}

void setup(){
  Serial.begin(9600);
  Serial.println("BMP280 test");
  
  if (!bmp.begin()) {  
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }
  setupBMP();
  Serial.println (initialPressure);

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void loop() {
    Serial.print("Temperature = ");
    Serial.print(getTemp());
    Serial.println(" *C");
    
    Serial.print("Pressure = ");
    Serial.print(getPressure());
    Serial.println(" Pa");

    Serial.print("Approx altitude = ");
    Serial.print(getAlt()); // Adjusted to your local forecast! //
    Serial.println(" m");
    
    Serial.println();
    delay(1000);
}
