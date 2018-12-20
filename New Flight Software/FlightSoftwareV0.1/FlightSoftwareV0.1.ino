#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <quaternionFilters.h>
#include <MPU9250.h>

//TeleArray Position
#define TeleID 0
#define TeleMissionTime 1 //RTC Time
#define TelePacket 2 //Number of times the telemetry has been transmitted
#define TeleAlt 3 //BMP Altitude
#define TelePressure 4 //BMP Pressure
#define TeleTemp 5 //BMP Temperature
#define TeleVolt 6 //Analog Voltage Reader
#define TeleGTime 7 //GPS Time
#define TeleGLat 8 //GPS Latitude
#define TeleGLong 9 //GPS Longitude
#define TeleGAlt 10 //GPS Altitude
#define TeleGSats 11 //GPS Number of Satellites
#define TeleRPM 12 //Gyro RPM
#define TeleTiltY 13 //Tilt Y (Pitch)
#define TeleTiltZ 14 //Tilt Z (Roll)
#define TeleState 15 //Flight Staging

#define TeleArrayLength 16 //Array Length
#define TeamID 1092 //Team ID

// Pin Declarations
#define GPS_SS_Rx 4 
#define GPS_SS_Tx 3

// Sensor Declarations
Adafruit_BMP280 bmp; 
SoftwareSerial gpsSerial(GPS_SS_Rx, GPS_SS_Tx); 
Adafruit_GPS GPS(&gpsSerial); //GPS Initialization
RTC_DS3231 rtc;
MPU9250 myIMU;

//Global Variables
int packetCount = 0;
float seaLevelPressure;
float TeleArray[TeleArrayLength];
long teleTime;
long teleTime2 = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  setupFunctions(); 
  TeleArray[TeleID] = (float)TeamID;
  teleTime = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (teleTime - teleTime2 > 1000){
    packetCount++;
    transmitData();
    teleTime2 = millis();
  }
  getData();
  teleTime = millis();
  getSatellites();
}
