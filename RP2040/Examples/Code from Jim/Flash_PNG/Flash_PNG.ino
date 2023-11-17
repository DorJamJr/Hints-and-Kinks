/*  
*   This example starts from the Arduino example Flash_PNG.ino and 
 *   explores using other sources of png files.
 *   Several files and file sizes are tested. A file too large fails, but others 
 *    seem to work ok. The converted file "blue_mandel.h" does not work and fails png.openFLASH with
 *    error INVALID_FILE. (I'm guessing that the file conversion was flawed in some respect.)
 *    One weird thing is that the file TestPNG.png/TestPNG.h (130x110) only shows about the
 *    top half of the pic. (Color depth??)
 *
 *  I discovered that the png.decode() fuction will hang if the png file is larger than 240x320 (ie, a 320x240
 *    file will hang). Fix is to rotate the png befor putting it through the online "FileToCArray" tool.
 *
 */


// This example renders a png file that is stored in a FLASH array
// using the PNGdec library (available via library manager).

// Image files can be converted to arrays using the tool here:
// https://notisrac.github.io/FileToCArray/
// To use this tool:
//   1. Drag and drop file on "Browse..." button
//   2. Tick box "Treat as binary"
//   3. Click "Convert"
//   4. Click "Save as file" and move the header file to sketch folder
//   5. Open the sketch in IDE
//   6. Include the header file containing the array (panda.h in this example)

// Include the PNG decoder library
#include <PNGdec.h>

#include "panda.h" // Image is stored here in an 8 bit array
//#include "blue_mandel.h"
#include "TestPNG.h"
// #include "Test2.h"
// #include "Test3.h"
// #include "Test4.h"
// #include "Test5.h"
// #include "flake.h"
//#include "Test6.h"
//#include "Stage.h"
//#include "Forest_8.h"
//#include "Forest_240.h"
#include "elephants.h"

PNG png; // PNG decoder inatance

#define MAX_IMAGE_WDITH 240 // Adjust for your images

int16_t xpos = 0;
int16_t ypos = 0;

// Include the TFT library https://github.com/Bodmer/TFT_eSPI
//#include "SPI.h"
#include <TFT_eSPI.h>              // Hardware-specific library
TFT_eSPI tft = TFT_eSPI();         // Invoke custom library

//====================================================================================
//                                    Setup
//====================================================================================
void setup()
{
  Serial.begin(115200);
  delay(5000);
  Serial.println("\n\n Using the PNGdec library");

  // Initialise the TFT
  tft.begin();
  tft.setRotation(2);           // we are using the screen in rotation orientation 3 (landscape) (1 gives x=0, y=0 at bottom right)
  tft.fillScreen(TFT_BLACK);

  Serial.println("\r\nInitialization done.");
}

//====================================================================================
//                                    Loop
//====================================================================================
void loop()
{
 int16_t rc = png.openFLASH((uint8_t *)elephants, sizeof(elephants), pngDraw);                     // works image 240x320
// int16_t rc = png.openFLASH((uint8_t *)Forest_240, sizeof(Forest_240), pngDraw);                     // works image 240x320
// int16_t rc = png.openFLASH((uint8_t *)Forest, sizeof(Forest), pngDraw);                     // works image 240x320
// int16_t rc = png.openFLASH((uint8_t *)panda1, sizeof(panda1), pngDraw);                     // works image 240x320
//  int16_t rc = png.openFLASH((uint8_t *)Blue_ice_flake, sizeof(Blue_ice_flake), pngDraw);  // fails: INVALID_FILE
//  int16_t rc = png.openFLASH((uint8_t *)TestPNG, sizeof(TestPNG), pngDraw);     // partial picture... top 1/2 ?? 130x110
//  int16_t rc = png.openFLASH((uint8_t *)Test2, sizeof(Test2), pngDraw);         // fails: PNG_TOO_BIG (708x410)
//  int16_t rc = png.openFLASH((uint8_t *)Test3, sizeof(Test3), pngDraw);           // works image 240x320
//  int16_t rc = png.openFLASH((uint8_t *)Test4, sizeof(Test4), pngDraw);           // works image 180x240
//  int16_t rc = png.openFLASH((uint8_t *)Test5, sizeof(Test5), pngDraw);           // works image 130x110
//  int16_t rc = png.openFLASH((uint8_t *)flake, sizeof(flake), pngDraw);           // fails: INVALID_FILE (copy of blue_mandel.h)
// int16_t rc = png.openFLASH((uint8_t *)Test6, sizeof(Test6), pngDraw);           // works: image 130x110
// int16_t rc = png.openFLASH((uint8_t *)Stage, sizeof(Stage), pngDraw);           // works: image 130x110

  Serial.print("Top of loop.  rc = ");
  Serial.println(rc);

  if (rc == PNG_SUCCESS) {
    Serial.println("Successfully png file");
    Serial.printf("image specs: (%d x %d), %d bpp, pixel type: %d\n", png.getWidth(), png.getHeight(), png.getBpp(), png.getPixelType());
    tft.startWrite();
    Serial.print("...after startWrite. ");
    uint32_t dt = millis();
    rc = png.decode(NULL, 1);
    Serial.print("rc= ");Serial.println(rc);
    Serial.print(millis() - dt); Serial.println("ms");
    tft.endWrite();
    // png.close(); // not needed for memory->memory decode
  }
  delay(3000);
  //tft.fillScreen(random(0x10000));
  tft.fillRectHGradient(0+30,150,240-60,320-240, TFT_GREEN, TFT_WHITE);
  tft.fillRectHGradient(0+30,250,240-60,320-240, TFT_GREEN, TFT_BLACK);
}


//=========================================v==========================================
//                                      pngDraw
//====================================================================================
// This next function will be called during decoding of the png file to
// render each image line to the TFT.  If you use a different TFT library
// you will need to adapt this function to suit.
// Callback function to draw pixels to the display
void pngDraw(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WDITH];
  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
  tft.pushImage(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}
