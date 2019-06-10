#include <Wire.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp; 
float alt;

//State Variables
int softwareState;
float lastAlt = 0;
float lastAlt2 = 0;
float lastAlt3 = 0;
boolean hasFlown = false;
boolean isDescending = false;

//Software States
#define PreFlight 1
#define Ascending 2
#define Descending 3
#define Spinning 4
#define Landed 5

// Pin Declarations
#define deployPinA 7
int count=0;

void setup() {
  // put your setup code here, to run once:
  pinMode(deployPinA,OUTPUT);
  int count = 0;
  digitalWrite(deployPinA, LOW);
  Serial.begin(9600);
  Wire.begin();
  setupBMP();
}

void loop() {
  // put your main code here, to run repeatedly:
  alt = getAlt();
  checkState();
  delay (500);
}

/*
 * Checks current software state
 */
void checkState(){
  checkFlown(alt); //Did it fly
  checkDescend(alt); // Is it descending?
  softwareState = PreFlight;
  // If the container is flying but is not descending, it must be
  // ascending or has landed
  if (hasFlown == true && isDescending == false){
    softwareState = Ascending;
  }
  // But in order to have landed, it must have descended first
  else if (isDescending == true){
    softwareState = Descending;
    if (alt < 4){
      softwareState = Landed;
    }
    else if (alt < 60){  //480m
      softwareState = Spinning;
      digitalWrite(deployPinA, HIGH);
      count++;
      if (count > 20) // Each delay count is 500 ms, so 10 seconds would be 20 intervals
      {
        digitalWrite(deployPinA, LOW);
      }
    }
  }
}

/*
 * Checks if the container is flying
 * If the container is above the ground by a certain amount, 
 * it can be considered flying
 */
void checkFlown(float alt){
  if (alt > 20){
    hasFlown = true;
  }
}

/*
 * Checks if the container is descending by comparing the last two altitudes
 * to the current altitude. If the current altitude is less than the previous two,
 * the container must be descending
 */
void checkDescend (float alt)
{
  if (hasFlown == true)
  {
    if (alt < lastAlt)
    {
      if (alt < lastAlt2) // Double check in case a bad altitude was measured
      {
        if (alt < lastAlt3){
        isDescending = true;
        }
      }
    }
  }
  lastAlt3 = lastAlt2;
  lastAlt2 = lastAlt;
  lastAlt = alt;
}
