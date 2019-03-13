/*
 * RF24 Radio Transceiver
 * https://howtomechatronics.com/tutorials/arduino/arduino-wireless-communication-nrf24l01-tutorial/
 */
const byte address[6] = "00001"; // Channel

void setupRecevier(){
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void getDirection(){
  if (radio.available()) {
    radio.read(&TeleArray[TeleDirection], sizeof(TeleArray[TeleDirection]));
  }
}

