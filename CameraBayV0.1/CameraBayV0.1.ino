// Magenetometer Libraries
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
// Transmitter Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

/*
 * For the transceiver Pins,
 * When facing upward, & the pins are oriented like:
 *    ________________________________________
 *   |1* 2*                                   |
 *   |3* 4*  Rest of the module on this side  |
 *   |5* 6*                                   |
 *   |7* 8*___________________________________|
 *   
 * 1 3.3V
 * 2 GND
 * 3 Pin 7
 * 4 Pin 8
 * 5 Pin 13
 * 6 Pin 11
 * 7 Pin 12
 * 8 EMPTY
 * For more info,
 * https://howtomechatronics.com/tutorials/arduino/arduino-wireless-communication-nrf24l01-tutorial/
 */

Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
float timer;
float heading = 0;
boolean detection = true;

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

/*
 * Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
 * Find yours here: http://www.magnetic-declination.com/
 * Toronto's Declination is 10.5 degrees which is about 0.18 rad
*/

float declinationAngle = 0.18;

void setup(void) 
{
  Serial.begin(9600);
  /* Initialise the sensor */
  if(!mag.begin())
  {
    /* There was a problem detecting the HMC5883 ... check your connections */
    detection = false;
  }
  /* Initialize the transceiver */
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  
  timer = millis();
}

void loop(void) 
{
  if (millis() - timer > 1000){
    timer = millis();
    if (detection == true){
      // Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
      // Calculate heading when the magnetometer is level, then correct for signs of axis.
      sensors_event_t event; 
      mag.getEvent(&event);
      heading = atan2(event.magnetic.y, event.magnetic.x);
      heading += declinationAngle;
  
      // Correct for when signs are reversed.
      if(heading < 0)
        heading += 2*PI;
    
      // Check for wrap due to addition of declination.
      if(heading > 2*PI)
        heading -= 2*PI;
   
      // Convert radians to degrees for readability.
      heading = heading * 180/M_PI; 
    
      
    }
  Serial.println(heading);
  radio.write (&heading, sizeof(heading));
  }
}
