float oldPacketCount = 0;
float oldSoftwareState = 0;

// This union is for converting floats into bytes for storing into the EEPROM
union{
  float fValue;
  byte bValue[4];
} floatToByte;

// This union is for converting the bytes back to floats to retrieve the data but mostly for testing
union {
  float fValue;
  byte bValue[4];
} byteToFloat;

/*
 * Stores the essentials in the EEPROM, specifcally the packet count, and software state
 */
void storeEssentials(){
  writeFloat (4, packetCount);
  if (softwareState != oldSoftwareState){
    writeFloat (8, softwareState);
  }
}

/*
 * 
 */
void setupEEPROM(){
  if (dataReset() == true){
    seaLevelPressure = readFloat(0);
    packetCount = readFloat (4);
    softwareState = readFloat (8);
    TeleArray[TelePacket] = packetCount; // If this line is not added, a packet count of 1 will be transmitted
    hasReset = true;
  }
  else{
    writeFloat (0, seaLevelPressure);
  }
}

/*
 * This method checks if the arduino has reset by using the Packet Count
 */
boolean dataReset()
{
  boolean answer;
  oldPacketCount = readFloat(4);
  // old if statement: packetCount == 0 && oldPacketCount > 1
  if (SD.exists("Reset.txt")) // If the SD Card has stuff in it, we need to get the old data back
  {
    answer = true; // The arduino has reset
    packetCount = oldPacketCount; // Take the old packet count
  }
  else
  {
    answer = false; // Arduino has not reset
    root = SD.open ("Reset.txt", FILE_WRITE);
    delay(10);
    root.close();
  } 
  return answer;
}

// Since each address of an EEPROM can only hold a byte, a float must be split up and stored as separate bytes
void writeFloat(int address, float val)
{
  floatToByte.fValue = val;
  writeAddress(address, floatToByte.bValue[0]); 
  address++;
  writeAddress(address, floatToByte.bValue[1]); 
  address++;
  writeAddress(address, floatToByte.bValue[2]); 
  address++;
  writeAddress(address, floatToByte.bValue[3]); 
}

// This function assumes that the next four bytes are a float from the starting address
float readFloat (int address)
{
  byteToFloat.bValue[0] = readAddress(address);
  address++;
  byteToFloat.bValue[1] = readAddress(address);
  address++;
  byteToFloat.bValue[2] = readAddress(address);
  address++;
  byteToFloat.bValue[3] = readAddress(address);
  float answer = byteToFloat.fValue;
  return answer;
}

// Writes a byte to an address in an external EEPROM
void writeAddress(int address, byte val)
{
//  Wire.beginTransmission(EEPROM_I2C_ADDRESS);
//  Wire.write((int)(address >> 8));   // MSB
//  Wire.write((int)(address & 0xFF)); // LSB
//  Wire.write(val);
//  Wire.endTransmission();
//  // Delay must be at least 4 milliseconds, but 5 milliseconds is used just in case
//  delay(5);
  if (EEPROM.read(address) != val){
    EEPROM.write(address, val);
  }
  delay(10);
}

// Reads the address on the external EEPROM, which will be a byte
byte readAddress(int address)
{
//  byte rData = 0xFF;
//  Wire.beginTransmission(EEPROM_I2C_ADDRESS);
//  Wire.write((int)(address >> 8));   // MSB
//  Wire.write((int)(address & 0xFF)); // LSB
//  Wire.endTransmission();  
//  Wire.requestFrom(EEPROM_I2C_ADDRESS, 1);  
//  rData =  Wire.read();
//  return rData;
  delay (5);
  return EEPROM.read(address);
}
