/*
   SD Card that works with the build-in SD card reader on the Teensy
   No pins needed
*/

float something;
void setupSD() {
  //SD.begin(chipSelect);
  if (SD.begin(chipSelect)) {
    root = SD.open("Tomahawk.csv", FILE_WRITE);
    root.close();
    //delay (100);
    //ess = SD.open ("Essential.csv", FILE_WRITE);
    //ess.close();
  }
}

/*
   Checks the SD card if there is any data in it and extract the essentials
   If the SD card doesn't have any data in it, store the essentials
*/
void checkSD() {
  //ess = SD.open("Essential.csv");
  //if (ess.available()) {
    //hasReset = true;
    //seaLevelPressure = ess.read();
    //ess.close();
  //}
  //else {
    //storeEssentials();
  //}
  //root = SD.open ("Tomahawk.csv");
  //if (root.available()){
    //hasReset = true;
    //while (root.available()){
      //Serial.println (root.read());
    //}
  //}
  //root.close();
}

/*
   Stores the essentials to allow the Cansat to resume progress after a power reset
*/
void storeEssentials() {
  ess = SD.open("Essential.csv", FILE_WRITE);
  ess.print (seaLevelPressure);
  ess.close();
}

/*
   Stores the data into the SD card in a similar format as how the telemetry is transmitted
*/
void storeData() {
  root = SD.open("Tomahawk.csv", FILE_WRITE);
  root.print(TeleArray[TeleID]);
  root.print(",");
  root.print(TeleArray[TeleMissionTime]);
  root.print(",");
  root.print(TeleArray[TelePacket]);
  root.print(",");
  root.print(TeleArray[TeleAlt]);
  root.print(",");
  root.print(TeleArray[TelePressure]);
  root.print(",");
  root.print(TeleArray[TeleTemp]);
  root.print(",");
  root.print(TeleArray[TeleVolt]);
  root.print(",");
  root.print(TeleArray[TeleGTime]);
  root.print(",");
  root.print(TeleArray[TeleGLat]);
  root.print(",");
  root.print(TeleArray[TeleGLong]);
  root.print(",");
  root.print(TeleArray[TeleGAlt]);
  root.print(",");
  root.print(TeleArray[TeleGSats]);
  root.print(",");
  root.print(TeleArray[TeleRPM]);
  root.print(",");
  root.print(TeleArray[TeleTiltY]);
  root.print(",");
  root.print(TeleArray[TeleTiltZ]);
  root.print(",");
  root.print(TeleArray[TeleState]);
  root.print(",");
  root.println(TeleArray[TeleDirection]);
  //root.println();
  closeSD();
}

void closeSD() {
  root.close();
}
