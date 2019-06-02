void setupFunctions(){
  Wire.begin();
  setupSD();
  setupRTC();
  setupBMP();
  setupMPU();
  setupBuzzer();
  setupGPS();
  setupVoltage();
  setupReceiver();
  setupTachometer();
  pinMode(deployPinA, OUTPUT);
  //pinMode(5,OUTPUT);
  //pinMode(7,OUTPUT);
  //digitalWrite(23, HIGH);
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
  TeleArray[TeleVolt] = getVolt();
  checkState();
}

/*
 * Obtains data from sensors
 * Used in FlightSoftware.ino
 * Different from getData() since this data must be constantly updated
 */
void getData2(){
  getTilt();
  getSatellites();
  getDirection();
  getRPM();
}

void transmitData(){
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
  Serial.print(TeleArray[TeleTiltY]);
  Serial.print(", ");
  Serial.print(TeleArray[TeleTiltZ]);
  Serial.print(", ");
  Serial.print(TeleArray[TeleRPM]);
  Serial.print(", ");
  Serial.print(TeleArray[TeleState]);
  Serial.print(", ");
  Serial.print(TeleArray[TeleDirection]);
  Serial.print("\n");
  Serial4.print(TeleArray[TeleID]);
  Serial4.print(", ");
  Serial4.print(TeleArray[TeleMissionTime]);
  Serial4.print(", ");
  Serial4.print(TeleArray[TelePacket]);
  Serial4.print(", ");
  Serial4.print(TeleArray[TeleAlt]);
  Serial4.print(", ");
  Serial4.print(TeleArray[TelePressure]);
  Serial4.print(", ");
  Serial4.print(TeleArray[TeleTemp]);
  Serial4.print(", ");
  Serial4.print(TeleArray[TeleVolt]);
  Serial4.print(", ");
  Serial4.print(TeleArray[TeleGTime]);
  Serial4.print(", ");
  Serial4.print(TeleArray[TeleGLat]);
  Serial4.print(", ");
  Serial4.print(TeleArray[TeleGLong]);
  Serial4.print(", ");
  Serial4.print(TeleArray[TeleGAlt]);
  Serial4.print(", ");
  Serial4.print(TeleArray[TeleGSats]);
  Serial4.print(", ");
  Serial4.print(TeleArray[TeleTiltY]);
  Serial4.print(", ");
  Serial4.print(TeleArray[TeleTiltZ]);
  Serial4.print(", ");
  Serial4.print(TeleArray[TeleRPM]);
  Serial4.print(", ");
  Serial4.print(TeleArray[TeleState]);
  Serial4.print(", ");
  Serial4.println(TeleArray[TeleDirection]);
}

/*
 * Checks if any commands have been received through the Serial Monitor
 * This would be done through the XBee and the groundstation
 */
void receiveRadioData() {
  if (Serial4.available()>0) {
    byte input = Serial4.read();
    //String RadioReceive = Serial4.readString();
    //Serial4.println (Serial4.read());
    if (input == 65) // Translates to "A"
    {
      digitalWrite(deployPinA, HIGH);
    }
    if (input == 66){ // Translates to "B"
      digitalWrite(deployPinA, LOW);
    }
  }
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
      startBuzzer();
      closeSD();
    }
    else if (TeleArray[TeleAlt] < 50){ // Use 480m instead of 450 due to any lag during the one second the altitude isnt measured
      softwareState = Spinning;
    }
  }
  TeleArray [TeleState] = softwareState;
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
