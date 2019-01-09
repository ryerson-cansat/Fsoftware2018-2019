#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
float heading, timer;
boolean detection = true;

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
    
      Serial.println(heading);
    }
    // At least print 0 so we know that its not connected properly instead of blank data or errors
    else {
      Serial.println (0);
    }
  }
}
