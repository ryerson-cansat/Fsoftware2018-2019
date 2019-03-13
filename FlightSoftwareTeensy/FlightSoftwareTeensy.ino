#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <quaternionFilters.h>
#include <MPU9250.h>
#include <SD.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

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
#define TeleTiltY 12 //Tilt Y (Pitch)
#define TeleTiltZ 13 //Tilt Z (Roll)
#define TeleRPM 14 //Gyro RPM
#define TeleState 15 //Flight Staging
#define TeleDirection 16 //Direction of camera to magnetic north

#define TeleArrayLength 17 //Array Length
#define TeamID 1092 //Team ID

// Pin Declarations
#define GPS_SS_Rx 4 
#define GPS_SS_Tx 3
#define deployPinA 7
#define deployPinB 8
#define buzzerPin 9

// Sensor Declarations
Adafruit_BMP280 bmp; 
SoftwareSerial gpsSerial(GPS_SS_Rx, GPS_SS_Tx); 
Adafruit_GPS GPS(&gpsSerial); //GPS Initialization
RTC_DS3231 rtc;
MPU9250 myIMU;
File root;
RF24 radio(7, 8); // CE, CSN

//Global Variables
int packetCount = 0;
float TeleArray[TeleArrayLength];
long teleTime;
long teleTime2 = 0;
boolean hasReset = false;

//Software States
#define PreFlight 1
#define Ascending 2
#define Descending 3
#define Spinning 4
#define Landed 5

//State Variables
int softwareState;
float lastAlt = 0;
float lastAlt2 = 0;
boolean hasFlown = false;
boolean isDescending = false;

void setup() {
  Serial.begin(9600);
  setupFunctions(); 
  checkSD();
  TeleArray[TeleID] = (float)TeamID;
  teleTime = millis();
  teleTime2 = millis();
}

void loop() {
  if (teleTime - teleTime2 > 1000){
    packetCount++;
    getData();
    checkState();
    transmitData();
    teleTime2 = millis();
    storeData();
  }
  getData2();
  teleTime = millis();
}
