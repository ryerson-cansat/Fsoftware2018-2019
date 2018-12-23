void setupFunctions(){
  Wire.begin();
  setupBMP();
  setupRTC();
  setupGPS();
  setupMPU();
}

/*
 * Obtains data from sensors
 * Used in FlightSoftware.ino
 * Different from getData2() since this data only needs to be updated
 * once per second or whenever it is needed
 */
void getData(){
  TeleArray[TeleMissionTime] = getSeconds();
  TeleArray[TeleTemp] = getTemp();
  TeleArray[TelePressure] = getPressure();
  TeleArray[TeleAlt] = getAlt();
  getGPS();
  TeleArray[TelePacket] = packetCount;
  
}

/*
 * Obtains data from sensors
 * Used in FlightSoftware.ino
 * Different from getData() since this data must be constantly updated
 */
void getData2(){
  getSatellites();
  getTilt();
}

void transmitData()
{
  Serial.print(TeleArray[TeleID]);
  Serial.print(", ");
  Serial.print(TeleArray[TeleMissionTime]);
  Serial.print(", ");
  Serial.print(TeleArray[TelePacket]);
  Serial.print(", ");
  Serial.print(TeleArray[TeleAlt]);
  Serial.print(", ");
  Serial.print(TeleArray[TelePressure]);
  Serial.print(", ");
  Serial.print(TeleArray[TeleTemp]);
  Serial.print(", ");
  Serial.print(TeleArray[TeleVolt]);
  Serial.print(", ");
  Serial.print(TeleArray[TeleGTime]);
  Serial.print(", ");
  Serial.print(TeleArray[TeleGLat]);
  Serial.print(", ");
  Serial.print(TeleArray[TeleGLong]);
  Serial.print(", ");
  Serial.print(TeleArray[TeleGAlt]);
  Serial.print(", ");
  Serial.print(TeleArray[TeleGSats]);
  Serial.print(", ");
  Serial.print(TeleArray[TeleRPM]);
  Serial.print(", ");
  Serial.print(TeleArray[TeleTiltY]);
  Serial.print(", ");
  Serial.print(TeleArray[TeleTiltZ]);
  Serial.print(", ");
  Serial.print(TeleArray[TeleState]);
  Serial.print("\n");
}

/*
 * Checks if any commands have been received through the Serial Monitor
 * This would be done through the XBee and the groundstation
 */
void receiveRadioData() {
  if (Serial.available()) {
    String RadioReceive = Serial.readString();
    if (RadioReceive == "A")
    {
      digitalWrite(deployPinA, HIGH);
    }
    if (RadioReceive == "B"){
      digitalWrite(deployPinB, HIGH);
    }
  }
}

/*
 * Checks the internal storage if there is data inside
 * If there is data, the container must have reset
 */
void checkStorage(){
  
}

/*
 * Checks current software state
 */
void checkState(){
  checkFlown(TeleArray[TeleAlt]); //Did it fly
  checkDescend(TeleArray[TeleAlt]); // Is it descending?
  softwareState = PreFlight;
  // If the container is flying but is not descending, it must be
  // ascending or has landed
  if (hasFlown == true && isDescending == false){
    softwareState = Ascending;
  }
  // But in order to have landed, it must have descended first
  else if (isDescending == true){
    softwareState = Descending;
    if (TeleArray[TeleAlt] < 4){
      softwareState = Landed;
    }
    else if (TeleArray[TeleAlt] < 450){
      softwareState = Spinning;
    }
  }
}

/*
 * Checks if the container is flying
 * If the container is above the ground by a certain amount, 
 * it can be considered flying
 */
void checkFlown(float alt){
  if (alt > 5){
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
        isDescending = true;
      }
    }
  }
  lastAlt2 = lastAlt;
  lastAlt = alt;
}
