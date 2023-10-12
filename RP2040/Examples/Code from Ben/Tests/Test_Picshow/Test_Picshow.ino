/*
   TicTacToe1
   by - B.Jack
   This program...

*/
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();
#include"PicoPins2.h"
#include <SPI.h>
#include <EEPROM.h>
#include <PNGdec.h>
#include"touchCalibrate.h"

#define WHITE_SPOT
#define X_or_O
#include "FireWorks.h"
PNG png; // PNG decoder inatance

#define MAX_IMAGE_WDITH 180// Adjust for your images

int16_t xpos = 0;
int16_t ypos = 0;

uint16_t x, y;
int initiateTime;
bool Winner=false;
int touchX;
int touchY;
int touch;
int Xcoord;
int Ycoord;
void setup() {
  pinSet();
  //Serial.begin(9600);
  //attachInterrupt(digitalPinToInterrupt(2), Encode, FALLING);

}

void loop()
{
int16_t rc = png.openFLASH((uint8_t *)FireWorks, sizeof(FireWorks), pngDraw);
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
}
//####################################################################
  
void pngDraw(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WDITH];
  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
  tft.pushImage(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}
