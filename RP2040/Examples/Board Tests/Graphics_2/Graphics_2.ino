/*
   Graphics2
   by - J.Koerlin
   This program...

*/
#include <SPI.h>
#include <EEPROM.h>
#include"touchCalibrate.h"


TFT_eSprite img = TFT_eSprite(&tft);  // Create Sprite object "img" with pointer to "tft" object
// the pointer is used by pushSprite() to push it onto the TFT
//#define CALIBRATION_FILE "/TouchCalData3"

#define NUM_KEYS 6 // one more than the number of buttons.
#define TFT_GREY 0x5AEB // New colour

#include"PicoPins2.h"

bool SwitchOn = false;
char msg;
int color = TFT_RED;

const int CalTest = 101;  // if the board is calibrated this should be written into the 0 reg

// Comment out to stop drawing black spots
#define BLACK_SPOT

// Switch position and size
#define FRAME_X 180
#define FRAME_Y 5
#define FRAME_W 150
#define FRAME_H 30

// Red zone size
#define REDBUTTON_X FRAME_X
#define REDBUTTON_Y FRAME_Y
#define REDBUTTON_W (FRAME_W/2)
#define REDBUTTON_H FRAME_H

// Green zone size
#define GREENBUTTON_X (REDBUTTON_X + REDBUTTON_W)
#define GREENBUTTON_Y FRAME_Y
#define GREENBUTTON_W (FRAME_W/2)
#define GREENBUTTON_H FRAME_H


void setup() {
  pinSet();
  Serial.begin(9600);
   //while (!Serial);
  EEPROM.begin(512);

  tft.init();
  tft.setRotation(3);
  //    delay(5000);
  touch_calibrate();
  attachInterrupt(digitalPinToInterrupt(2), Encode, FALLING);

  tft.fillScreen(TFT_ORANGE);

  redBtn();
}

void loop() {
  if (SwitchOn)
    greenBtn();
  else
    redBtn();

  buttonCheck();  tft.setCursor(0, 0, 4);  // Set "cursor" at top left corner of display (0,0) and select font 2
  // Set the font colour to be white with a black background, set text size multiplier to 1
  tft.setTextColor(TFT_WHITE, TFT_BLACK);  tft.setTextSize(1);
  // We can now plot text on screen using the "print" class
  tft.println("Hello World!");
  // delay(500);
  tft.setTextColor(TFT_BLACK, TFT_ORANGE);  tft.setTextSize(1);
  tft.setCursor(30, 40, 4);  // Set "cursor" at top left corner of display (30,40) and select font 4
  tft.print("count");  // Print Encoder Count
  tft.setCursor(100, 40, 6);  // Set "cursor" at top left corner of display (100,40) and select font 6
  // Set the font colour to be white with a black background, set text size multiplier to 1
  // We can now plot text on screen using the "print" class
  tft.print(count);  // Number font -  notice the letters are not printed
  tft.println("   ");  // add some space to cover larger numbers
  tft.drawLine(10, 200, 210, 100, TFT_BLACK);
  tft.drawRect(60, 180, 30, 50, TFT_CYAN);
  tft.fillRect(100, 180, 30, 50, TFT_CYAN);
  tft.fillCircle(55, 110, 30, color);
  tft.drawCircle(55, 110, 50, color);
  tft.fillTriangle(200, 220, 300, 220, 250, 133, tft.color565(250, 0, 250));
  //tft.drawTriangle(217, 210, 283, 210, 250, 153, tft.color565(250, 250, 0));
  tft.fillRoundRect(140, 140, 50, 90, 20, tft.color565(0, 30, 150));
  drawStar(216, 40, TFT_VIOLET);
  drawStar(249, 68, TFT_NAVY);
}


void buttonCheck() {
  uint16_t x, y;

  // See if there's any touch data for us
  if (tft.getTouch(&x, &y))
  {
    // Draw a block spot to show where touch was calculated to be
#ifdef BLACK_SPOT
    tft.fillCircle(x, y, 2, TFT_BLACK);
#endif

    if (SwitchOn)
    {
      if ((x > REDBUTTON_X) && (x < (REDBUTTON_X + REDBUTTON_W))) {
        if ((y > REDBUTTON_Y) && (y <= (REDBUTTON_Y + REDBUTTON_H))) {
          Serial.println("Red btn hit");
          redBtn();
          SwitchOn = false;
          color = TFT_RED;
          analogWrite(rLED, 50);
          digitalWrite(gLED, LOW);
        }
      }
    }
    else //Record is off (SwitchOn == false)
    {
      if ((x > GREENBUTTON_X) && (x < (GREENBUTTON_X + GREENBUTTON_W))) {
        if ((y > GREENBUTTON_Y) && (y <= (GREENBUTTON_Y + GREENBUTTON_H))) {
          Serial.println("Green btn hit");
          greenBtn();
          SwitchOn = true;
          color = TFT_GREEN;
          analogWrite(gLED, 50);
          digitalWrite(rLED, LOW);

        }
      }
    }

    Serial.println(SwitchOn);

  }
}
// #########################################################################
// Create sprite, plot graphics in it, plot to screen, then delete sprite
// #########################################################################
void drawStar(int x, int y, int star_color)
{
  // Create an 8 bit sprite 70x 80 pixels (uses 5600 bytes of RAM)
  img.setColorDepth(8);
  img.createSprite(70, 80);

  // Fill Sprite with a "transparent" colour
  // TFT_TRANSPARENT is already defined for convenience
  // We could also fill with any colour as "transparent" and later specify that
  // same colour when we push the Sprite onto the screen.
  img.fillSprite(TFT_TRANSPARENT);

  // Draw 2 triangles to create a filled in star
  img.fillTriangle(35, 0, 0, 59, 69, 59, star_color);
  img.fillTriangle(35, 79, 0, 20, 69, 20, star_color);

  // Punch a star shaped hole in the middle with a smaller transparent star
  img.fillTriangle(35, 7, 6, 56, 63, 56, TFT_TRANSPARENT);
  img.fillTriangle(35, 73, 6, 24, 63, 24, TFT_TRANSPARENT);

  // Push sprite to TFT screen CGRAM at coordinate x,y (top left corner)
  // Specify what colour is to be treated as transparent.
  img.pushSprite(x, y, TFT_TRANSPARENT);

  // Delete it to free memory
  img.deleteSprite();

}

void drawFrame()
{
  tft.drawRect(FRAME_X, FRAME_Y, FRAME_W, FRAME_H, TFT_BLACK);
}

// Draw a red button
void redBtn()
{
  tft.fillRect(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, TFT_RED);
  tft.fillRect(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, TFT_DARKGREY);
  drawFrame();
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("ON", GREENBUTTON_X + (GREENBUTTON_W / 2), GREENBUTTON_Y + (GREENBUTTON_H / 2), 2);
}

// Draw a green button
void greenBtn()
{
  tft.fillRect(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, TFT_GREEN);
  tft.fillRect(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, TFT_DARKGREY);
  drawFrame();
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("OFF", REDBUTTON_X + (REDBUTTON_W / 2) + 1, REDBUTTON_Y + (REDBUTTON_H / 2), 2);
  //SwitchOn = true;
}
