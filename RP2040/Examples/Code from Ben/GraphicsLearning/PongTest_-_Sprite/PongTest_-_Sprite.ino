/*
   Pong Start
   by - J.Koerlin
   This program...

*/
#include <SPI.h>
#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
TFT_eSPI tft = TFT_eSPI();  // Invoke library
TFT_eSprite img = TFT_eSprite(&tft);  // Create Sprite object "img" with pointer to "tft" object


#include "PicoPins2.h"

int color = TFT_GOLD;
int pkx = 8, pky = 8;
int pkr = 15;
int pkVel = 3, scW = 340, scH = 240;
int oldpkx, oldpky;
float angle = .4;  // Angle in Radians for the puck travel
int xDir = 1, yDir = 1;

void setup() {
  pinSet();
  Serial.begin(9600);
  tft.init();
  tft.setRotation(3);
  //attachInterrupt(digitalPinToInterrupt(encA), Encode, FALLING);
  tft.fillScreen(TFT_BLACK);
}

void loop() {

  puck();
  bounce();
  drawPuck(pkx, pky, TFT_VIOLET);
}

void puck()
{
  pkx += (cos(angle) * pkVel) * xDir;
  pky += (sin(angle) * pkVel) * yDir;
}
void bounce()
{
  if (pkx > scW - pkr)
    xDir = -1;
  if (pkx < 0 + pkr)
    xDir = 1;
  if (pky < 0 + pkr)
    yDir = 1;
  if (pky > scH - pkr)
    yDir = -1;
}

// #########################################################################
// Create sprite, plot graphics in it, plot to screen, then delete sprite
// #########################################################################
void drawPuck(int x, int y, int puck_color)
{
  // Create an 8 bit sprite 70x 80 pixels (uses 5600 bytes of RAM)
  img.setColorDepth(8);
  img.createSprite(40, 40);

  // Fill Sprite with a "transparent" colour
  // TFT_TRANSPARENT is already defined for convenience
  // We could also fill with any colour as "transparent" and later specify that
  // same colour when we push the Sprite onto the screen.
  img.fillSprite(TFT_TRANSPARENT);

  // Draw 2 triangles to create a filled in star
  //  img.drawCircle(35, 0, 0, 59, 69, 59, star_color);
  //  img.fillTriangle(35, 79, 0, 20, 69, 20, star_color);
  img.fillCircle(20, 20, 20, TFT_BLACK);
  img.fillCircle(20, 20, 15, puck_color);


  // Push sprite to TFT screen CGRAM at coordinate x,y (top left corner)
  // Specify what colour is to be treated as transparent.
  img.pushSprite(x, y, TFT_TRANSPARENT);

  // Delete it to free memory
  img.deleteSprite();

}
