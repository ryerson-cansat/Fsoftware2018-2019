/*
   RF24 Radio Transceiver
   https://howtomechatronics.com/tutorials/arduino/arduino-wireless-communication-nrf24l01-tutorial/
   For connections, look at slack
*/

const byte data_pipe[][6] = {"00001", "00010"}; //communication addresses [send,recv]

boolean enable = false; // once false, then the e-bay no longer sends data and only recieves
float yawDir = 0; //return value of this script in type float

void setupRF24() {
  //configurations
  radio.begin();
  //radio.setRetries(15, 15);
  radio.setPALevel(RF24_PA_MAX); //power level
  radio.openWritingPipe(data_pipe[0]);
  radio.openReadingPipe(1, data_pipe[1]);
}

void firstWrite() {
  radio.stopListening(); //stop any listening activities
  delay(10);
  char data[] = "this thing finally works god damnit"; //enable message
  radio.write(data, strlen(data));
  delay(10);
}

float enableGetData() {
//  if (enable == false) { //only send while enable is false
//    for (int x = 0; x < 3; x++) {
//      firstWrite(); //send enable message 10 times just in case
//    }
//  }
  radio.startListening();  //enable listening
  delay(200);             //vv important delay to allow registers on nrf to setup
  if (radio.available())
  {
    int payload_size = radio.getDynamicPayloadSize();
    if (payload_size > 1)
    {
      char* payload = new char[payload_size + 1];
      radio.read(payload, payload_size);
      payload[payload_size] = '\0'; //null terminates data VV important
      //printf("Got Message: %s\r\n", payload); // output message not needed
      yawDir = atof(payload);
    }
    enable = true;
  }
  return yawDir;
}
