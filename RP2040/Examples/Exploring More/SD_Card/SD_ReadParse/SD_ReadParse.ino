/*
  SD card read and Parse data int integers
  the data is stored on the SDcard in a .csv file
  and looks like:

  100,200,300,400,500,600,700,800
  108,108,52,108,52,52,75,75

  This example shows how to read and write data to and from an SD card file
  The circuit on the RP2040 is:
   SD card attached to SPI bus as follows:
 ** MOSI - pin 19
 ** MISO - pin 16
 ** CLK - pin 18
 ** CS - pin 12

  by J. Koerlin 1-24-23
  the data from the SDcard is read one char at a time with few options.  
  This program reads the 2 lines into an array and parces the line into integers.
*/

#include <SPI.h>
#include <SD.h>

File myFile;

char fileName[] = "Data.csv";
const int cs_SD = 12;  //CS for the SD Card
char line[100];
char*b;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {//wait for serial port to connect.
  }                // Without this you will mis everything...



  Serial.print("Initializing SD card...");

  if (!SD.begin(cs_SD)) {
    Serial.println("initialization failed!");
    while (1); //terminally Stop Here!  Nothing we can do.
  }
  Serial.println("initialization done.");


  // Open the file for reading:
  myFile = SD.open(fileName);
  if (myFile) {
    Serial.println("the file:");
    getLine();
    parseLine();
    getLine();
    parseLine();

    // close the file:
    myFile.close();
    Serial.println("Success!");

  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  Serial.println("All Done");

}

void loop() {
  // nothing happens after setup
}


void getLine()
{
  int x = 0, num;
  // read from the file until there's nothing else in it:
  while (myFile.available()) {
    line[x] = myFile.read();
    if (line[x] == '\n')
    {
      line[x] = NULL;
      break;
    }
    Serial.write(line[x]);
    x++;
  }
}

void parseLine()
{
  char* c = line;
  int x = 0, num;
  Serial.println("");
  b = strtok(c, ",");
  while (b != NULL) {
    Serial.println(b);
    num = atoi(&b[0]);
    b = strtok(NULL, ",");
  }
}
