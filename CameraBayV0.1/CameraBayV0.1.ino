// Magenetometer Libraries
// We have removed the HMC5883 for the MPU9250
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "MPU9250.h"
#include "quaternionFilters.h"
// Transmitter Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

/*
   For the transceiver Pins,
   When facing upward, & the pins are oriented like:
      ________________________________________
     |1* 2*                                   |
     |3* 4*  Rest of the module on this side  |
     |5* 6*                                   |
     |7* 8*___________________________________|

   1 3.3V
   2 GND
   3 Pin 7
   4 Pin 8
   5 Pin 13
   6 Pin 11
   7 Pin 12
   8 EMPTY
   For more info,
   https://howtomechatronics.com/tutorials/arduino/arduino-wireless-communication-nrf24l01-tutorial/
*/



#define AHRS false         // Set to false for basic data read
#define SerialDebug true  // Set to true to get Serial output for debugging

MPU9250 myIMU;
float timer;
float heading = 0;
boolean detection = true;

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

/*
   Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
   Find yours here: http://www.magnetic-declination.com/
   Toronto's Declination is 10.5 degrees which is about 0.18 rad
*/

float declinationAngle = 0.18;

void setup(void)
{
  Wire.begin();
  Serial.begin(9600);
  /* Initialise the sensor */
  byte c = myIMU.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);
  if (c == 0x68)
  {
    myIMU.MPU9250SelfTest(myIMU.selfTest);
    myIMU.calibrateMPU9250(myIMU.gyroBias, myIMU.accelBias);
    myIMU.initMPU9250();
    byte d = myIMU.readByte(AK8963_ADDRESS, WHO_AM_I_AK8963);
    myIMU.initAK8963(myIMU.factoryMagCalibration);
    myIMU.getAres();
    myIMU.getGres();
    myIMU.getMres();
    //myIMU.magCalMPU9250(myIMU.magBias, myIMU.magScale);
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
  //  if (millis() - timer > 1000){
  //    timer = millis();
  //    if (detection == true){
  //      // Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
  //      // Calculate heading when the magnetometer is level, then correct for signs of axis.
  //      sensors_event_t event;
  //      mag.getEvent(&event);
  //      heading = atan2(event.magnetic.y, event.magnetic.x);
  //      heading += declinationAngle;
  //
  //      // Correct for when signs are reversed.
  //      if(heading < 0)
  //        heading += 2*PI;
  //
  //      // Check for wrap due to addition of declination.
  //      if(heading > 2*PI)
  //        heading -= 2*PI;
  //
  //      // Convert radians to degrees for readability.
  //      heading = heading * 180/M_PI;
  //
  //
  //    }
  //  Serial.println(heading);
  //  radio.write (&heading, sizeof(heading));
  //}
  if (myIMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)
  {
    myIMU.readAccelData(myIMU.accelCount);  // Read the x/y/z adc values
    // Now we'll calculate the accleration value into actual g's
    // This depends on scale being set
    myIMU.ax = (float)myIMU.accelCount[0] * myIMU.aRes; // - myIMU.accelBias[0];
    myIMU.ay = (float)myIMU.accelCount[1] * myIMU.aRes; // - myIMU.accelBias[1];
    myIMU.az = (float)myIMU.accelCount[2] * myIMU.aRes; // - myIMU.accelBias[2];
    myIMU.readGyroData(myIMU.gyroCount);  // Read the x/y/z adc values
    // Calculate the gyro value into actual degrees per second
    // This depends on scale being set
    myIMU.gx = (float)myIMU.gyroCount[0] * myIMU.gRes;
    myIMU.gy = (float)myIMU.gyroCount[1] * myIMU.gRes;
    myIMU.gz = (float)myIMU.gyroCount[2] * myIMU.gRes;
    myIMU.readMagData(myIMU.magCount); // Read the x/y/z adc values
    myIMU.mx = (float)myIMU.magCount[0] * myIMU.mRes
               * myIMU.factoryMagCalibration[0] - myIMU.magBias[0];
    myIMU.my = (float)myIMU.magCount[1] * myIMU.mRes
               * myIMU.factoryMagCalibration[1] - myIMU.magBias[1];
    myIMU.mz = (float)myIMU.magCount[2] * myIMU.mRes
               * myIMU.factoryMagCalibration[2] - myIMU.magBias[2];
  }
  myIMU.updateTime();
  // Sensors x (y)-axis of the accelerometer is aligned with the y (x)-axis of
  // the magnetometer; the magnetometer z-axis (+ down) is opposite to z-axis
  // (+ up) of accelerometer and gyro! We have to make some allowance for this
  // orientationmismatch in feeding the output to the quaternion filter. For the
  // MPU-9250, we have chosen a magnetic rotation that keeps the sensor forward
  // along the x-axis just like in the LSM9DS0 sensor. This rotation can be
  // modified to allow any convenient orientation convention. This is ok by
  // aircraft orientation standards! Pass gyro rate as rad/s
  MahonyQuaternionUpdate(myIMU.ax, myIMU.ay, myIMU.az, myIMU.gx * DEG_TO_RAD,
                         myIMU.gy * DEG_TO_RAD, myIMU.gz * DEG_TO_RAD, myIMU.my,
                         myIMU.mx, myIMU.mz, myIMU.deltat);
  myIMU.pitch = -asin(2.0f * (*(getQ()+1) * *(getQ()+3) - *getQ()
                    * *(getQ()+2)));
  myIMU.roll  = atan2(2.0f * (*getQ() * *(getQ()+1) + *(getQ()+2)
                    * *(getQ()+3)), *getQ() * *getQ() - *(getQ()+1)
                    * *(getQ()+1) - *(getQ()+2) * *(getQ()+2) + *(getQ()+3)
                    * *(getQ()+3));
  myIMU.pitch *= RAD_TO_DEG;
  myIMU.roll *= RAD_TO_DEG;
  // Declination of Ryerson University (43°39'31.5"N 79°22'45.5"W) is
  // 10.46° W  ± 0.38° at 2018-10-19
  // - http://www.ngdc.noaa.gov/geomag-web/#declination
  Serial.println(myIMU.pitch);
  Serial.println(myIMU.roll);
}
