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
  TeleArray[TeleDirection] = 120.0;
  TeleArray[TeleState] = 1;
  TeleArray[TeleRPM] = 2200;
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
  Serial3.print(TeleArray[TeleID]);
  Serial3.print(", ");
  Serial3.print(TeleArray[TeleMissionTime]);
  Serial3.print(", ");
  Serial3.print(TeleArray[TelePacket]);
  Serial3.print(", ");
  Serial3.print(TeleArray[TeleAlt]);
  Serial3.print(", ");
  Serial3.print(TeleArray[TelePressure]);
  Serial3.print(", ");
  Serial3.print(TeleArray[TeleTemp]);
  Serial3.print(", ");
  Serial3.print(TeleArray[TeleVolt]);
  Serial3.print(", ");
  Serial3.print(TeleArray[TeleGTime]);
  Serial3.print(", ");
  Serial3.print(TeleArray[TeleGLat]);
  Serial3.print(", ");
  Serial3.print(TeleArray[TeleGLong]);
  Serial3.print(", ");
  Serial3.print(TeleArray[TeleGAlt]);
  Serial3.print(", ");
  Serial3.print(TeleArray[TeleGSats]);
  Serial3.print(", ");
  Serial3.print(TeleArray[TeleTiltY]);
  Serial3.print(", ");
  Serial3.print(TeleArray[TeleTiltZ]);
  Serial3.print(", ");
  Serial3.print(TeleArray[TeleRPM]);
  Serial3.print(", ");
  Serial3.print(TeleArray[TeleState]);
  Serial3.print(", ");
  Serial3.println(TeleArray[TeleDirection]);
}
