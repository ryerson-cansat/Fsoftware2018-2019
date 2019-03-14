void setupFunctions(){
  Wire.begin();
  setupSD();
  setupRTC();
  setupBMP();
  setupMPU();
  setupBuzzer();
  setupGPS();
  setupVoltage();
  //pinMode(23, OUTPUT);
  //digitalWrite(23, HIGH);
}

void getData(){
  TeleArray[TeleMissionTime] = getSeconds();
  TeleArray[TeleTemp] = getTemp();
  TeleArray[TelePressure] = getPressure();
  TeleArray[TeleAlt] = getAlt();
  getGPS();
  TeleArray[TelePacket] = packetCount;
  TeleArray[TeleVolt] = getVolt();
}

void getData2(){
  getTilt();
  getSatellites();
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
  Serial.print(TeleArray[TeleRPM]);
  Serial.print(", ");
  Serial.print(TeleArray[TeleTiltY]);
  Serial.print(", ");
  Serial.print(TeleArray[TeleTiltZ]);
  Serial.print(", ");
  Serial.print(TeleArray[TeleState]);
  Serial.print("\n");
}
