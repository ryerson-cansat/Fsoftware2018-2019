#include <SD.h>
#include <SPI.h>

File root;

const int chipSelect = BUILTIN_SDCARD;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  Serial.print("Initializing SD card...");

  //basically checks if the SD is correctly initialized
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  //looks for and open the specified file to write to it. If the file isn't found it creates that file and proceeds to write to it
  root = SD.open("example.txt", FILE_WRITE);

  //can have as many of this lines as you want. Basically you're writing to the specified file. Will be used to keep track of data
  root.println("Trial 1");

  //MUST CLOSE THE FILE AFTER WRITING TO IT
  root.close();

  //uncomment if you want to print the data from the specified file into the serial monitor (also must specify what file you're opening)
/*  root = SD.open("example.txt");

  while (root.available()) {
    Serial.write(root.read());

  }

  root.close();

*/
  Serial.println("done!");
}

void loop()
{
  // nothing happens after setup finishes.
}
