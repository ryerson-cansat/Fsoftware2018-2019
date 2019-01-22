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
  root = SD.open("data.csv", FILE_WRITE);

  //can have as many of this lines as you want. Basically you're writing to the specified file.
  //Until you dont print a "," the text wont switch to the next column
  //If you want to move onto the next row, simply do a println the row before

  /* uncomment the below commented code to test out how writing to a csv file works
  root.print("R1C1");
  root.print(",");
  root.println("R1C2");

  root.print("R2C1");
  root.println("R2C2");
*/
  //MUST CLOSE THE FILE AFTER WRITING TO IT
  root.close();

  Serial.println("done!");
}

void loop()
{
  // nothing happens after setup finishes.
}
