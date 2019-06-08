#include "RF24.h"
#include "printf.h"
#include "SPI.h"

RF24 radio(7, 8);
const byte data_pipe[][6] = {"00001","00010"}; //communication addresses [send,recv]

bool enable = false; // once false, then the e-bay no longer sends data and only recieves

void setup() {
  printf_begin(); //not really needed

  //configurations
  radio.begin();
  radio.setRetries(15, 15);
  radio.setPALevel(RF24_PA_MAX); //power level
  radio.openWritingPipe(data_pipe[0]);
  radio.openReadingPipe(1, data_pipe[1]);
}

void firstWrite(){
  radio.stopListening(); //stop any listening activities
  char data[] = "this thing finally works god damnit"; //enable message
  radio.write(data, strlen(data));
}

void loop() {
  if (enable == false){ //only send while enable is false
    for (int x = 0; x < 10; x++){
      firstWrite(); //send enable message 10 times just in case
    }
  }
  radio.startListening();  //enable listening
  delay(100);             //vv important delay to allow registers on nrf to setup
  if (radio.available())
  {
    int payload_size = radio.getDynamicPayloadSize();
    if (payload_size > 1)
    {
      char* payload = new char[payload_size + 1];
      radio.read(payload, payload_size);
      payload[payload_size] = '\0'; //null terminates data VVimportant
      printf("Got Message: %s\r\n", payload); // output message not needed
    }
    enable = true;
  }
}
