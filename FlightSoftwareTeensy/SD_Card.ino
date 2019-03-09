/*
 * SD Card that works with the build-in SD card reader on the Teensy
 * No pins needed
 */
 
const int chipSelect = BUILTIN_SDCARD;

void setupSD(){
  if (SD.begin(chipSelect)) {
    root = SD.open ("TeamTomahawk1092.txt", FILE_WRITE);
  }
}

/*
 * Checks the SD card if there is any data in it and extract the essentials
 * If the SD card doesn't have any data in it, store the essentials
 */
void checkSD(){
  if (root.available()){
    hasReset = true;
    // Get the sea level pressure and packet count
  }
  else{
    storeEssentials();
  }
}

/*
 * Stores the essentials to allow the Cansat to resume progress after a power reset
 */
void storeEssentials(){
  root.print (seaLevelPressure);
}

/*
 * Stores the data into the SD card in a similar format as how the telemetry is transmitted
 */
void storeData(){
  root.print(TeleArray[TeleID]);
  root.print(", ");
  root.print(TeleArray[TeleMissionTime]);
  root.print(", ");
  root.print(TeleArray[TelePacket]);
  root.print(", ");
  root.print(TeleArray[TeleAlt]);
  root.print(", ");
  root.print(TeleArray[TelePressure]);
  root.print(", ");
  root.print(TeleArray[TeleTemp]);
  root.print(", ");
  root.print(TeleArray[TeleVolt]);
  root.print(", ");
  root.print(TeleArray[TeleGTime]);
  root.print(", ");
  root.print(TeleArray[TeleGLat]);
  root.print(", ");
  root.print(TeleArray[TeleGLong]);
  root.print(", ");
  root.print(TeleArray[TeleGAlt]);
  root.print(", ");
  root.print(TeleArray[TeleGSats]);
  root.print(", ");
  root.print(TeleArray[TeleRPM]);
  root.print(", ");
  root.print(TeleArray[TeleTiltY]);
  root.print(", ");
  root.print(TeleArray[TeleTiltZ]);
  root.print(", ");
  root.print(TeleArray[TeleState]);
  root.print("\n");
}

void closeSD(){
  root.close();
}

