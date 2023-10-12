/* Memory Verse Helper
 *  by: J.Koerlin
 *  This sketch will read an uploaded file and display one piece at a time on the screen.
 *  the File must use 2 tokens to delineate verses. Each on their own line 
 *  followed by a CarrageReturn and a LineFeed. 
 *    1. '|' is used to identify the end of verse
 *    2. '#' to designate the end of file.
 *  The 4 buttons are used to navigate as follows:
 *    gPB - Green PushButton - Next Verse: just displays the reference
 *    bPB - Blue PushButton - Next Line: reveals 1 line of the verse at a time till the end
 *    yPB - Yellow PushButton - All Verse: reveals the remainder of the verse
 *    rPB - Red PushButton - Previous Verse: moves back to the previous verse reference
 *    
 *  What I learned.  This is a great way to store text data.  It is read one byte at a time.
 *  There are not many ways to parse data like we have in reading from a Serial Port.
 *  You must read data into a char array and then convert with atoxxx
 *  detaild of how to set up the data file, load the Library and Install the "Pico LittleFS Filesystem Uploader"
 *  are available from this link: https://arduino-pico.readthedocs.io/_/downloads/en/latest/pdf/
 *      See File Systems
 *  Please note that the instructions indicate it can toake a long tome for large files.  
 *  It takes over 2 minutes to load the daya file.  
 *  I believe this is due to the large (16M) memory size
 *  
 *  At the time of this writing the Arduino IDE does not support tools.  
 *  This is required to load the data file.  You will need to use the 1.8.x version
 *  While it is possible to data log to a file.  Very useful!  I don't know of a way to 
 *  retrieve the file back to the PC.
 *    
 */
// Be sure to install the Pico LittleFS Data Upload extension for the
// Arduino IDE from:
//    https://github.com/earlephilhower/arduino-pico-littlefs-plugin/
// The latest release is available from:
//    https://github.com/earlephilhower/arduino-pico-littlefs-plugin/releases

// Before running:
// 1) Select Tools->Flash Size->(some size with a FS/filesystem)
// 2) Use the Tools->Pico Sketch Data Upload tool to transfer the contents of
//    the sketch data directory to the Pico
#include <SPI.h>
#include <TFT_eSPI.h>       // Graphics and font library for ILI9341 driver chip
TFT_eSPI tft = TFT_eSPI();  // Invoke library
#include "Free_Fonts.h" // Include the header file attached to this sketch
#include <LittleFS.h>
#include "PicoPins2.h"
//#define FSS12 &FreeSans12pt7b
//#define FSS9 &FreeSans9pt7b
char buff[32];
int cnt = 1;
char c, LF = 10;
File f, i;
int Marker[20];
int verseNum = 0;
bool viewing = true, endofVerse = true, End = false;

void setup() {
  pinSet();
  Serial.begin(115200);
  Marker[0] = 0;
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_SKYBLUE);
  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(FSB24);
  tft.setCursor(10, 90);
  tft.print("Memory Verse");
  tft.setCursor(90, 150);
  tft.print("Helper");
  tft.setFreeFont(FSB9);

  Serial.println("Running...");
  LittleFS.begin();
  i = LittleFS.open("Verses.txt", "r");
}


void loop() {
  if (i) {  //File found ready to display first
    while (viewing)
    {
      if (digitalRead(rPB) == 0) {
        prevVerse();
        delay(50);
        while (digitalRead(rPB) == 0) {}
        delay(50);
      }
      if (digitalRead(gPB) == 0) {
        nextVerse();
        delay(50);
        while (digitalRead(gPB) == 0) {}
        delay(50);
      }
      if (digitalRead(yPB) == 0) {
        toEOVerse();
        delay(50);
        while (digitalRead(yPB) == 0) {}
        delay(50);
      }
      if (digitalRead(bPB) == 0) {
        nextLine();
        delay(50);
        while (digitalRead(bPB) == 0) {}
        delay(50);
      }
      if (endofVerse)
        digitalWrite(gLED, HIGH);
      else
        digitalWrite(gLED, LOW);
      if (End)
        digitalWrite(bLED, HIGH);
      else
        digitalWrite(bLED, LOW);

    }
    i.close();
  }

}
void nextVerse()
{
  if (!End)
  {
    tft.fillScreen(TFT_SKYBLUE);
    tft.setCursor(30, 15);
    if (endofVerse)
    {
      while (c != LF) {
        c = i.read();
        if (c == '#')
        {
          End = true;
          break;
        }
        tft.print(c);
      }
      c = 'x';
      endofVerse = false;
    }
    else
    {
      while (c != '|')
        c = i.read();
      c = i.read();
      c = i.read();
      c = 'x';
      verseNum++;
      Marker[verseNum] = i.position() ;
      while (c != LF) {
        c = i.read();
        if (c == '#')
        {
          End = true;
          break;
        }
        tft.print(c);
      }
      c = 'x';
      endofVerse = false;
    }
  }
}
void prevVerse()
{
  End = false;
  tft.fillScreen(TFT_SKYBLUE);
  tft.setCursor(30, 15);
  verseNum--;
  if (verseNum < 0)verseNum = 0;
  i.seek(Marker[verseNum], SeekSet);
  while (c != LF) {
    c = i.read();
    tft.print(c);
  }
  endofVerse = false;
  End = false;
  c = 'x';
}
void nextLine()
{
  if (!End && !endofVerse)
  {
    while (c != LF) {
      c = i.read();
      if (c == '|')
      {
        c = i.read();
        c = i.read();
        c = 'x';
        verseNum++;
        Marker[verseNum] = i.position() ;
        endofVerse = true;
        break;
      }
      else if (c == '#')
      {
        End = true;
        break;
      }
      else
        tft.print(c);
    }
  }
  c = 'x';
}

void toEOVerse()
{
  if (!End && !endofVerse)
  {
    while (c != '|') {
      c = i.read();
      tft.print(c);
    }
    c = i.read();
    c = i.read();
    c = 'x';
    verseNum++;
    Marker[verseNum] = i.position() ;
    endofVerse = true;
  }

}
void Display() {
  tft.fillScreen(TFT_SKYBLUE);
  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(FSB9);
  tft.setCursor(30, 15);  // Set "cursor" at top left corner of display (30,40) and select font 4
  while (i.available()) {
    c = i.read();
    if (c == '|')
      break;
    tft.print(c);
  }
}
