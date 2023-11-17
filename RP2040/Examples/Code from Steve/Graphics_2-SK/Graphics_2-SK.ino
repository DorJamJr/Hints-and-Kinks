/*
   Graphics2
   by - J.Koerlin
   This program...

*/
#include <SPI.h>
#include <EEPROM.h>
#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip

TFT_eSPI tft = TFT_eSPI();  // Invoke library

#include "touchCalibrate.h"
#include "PicoPins2.h"


TFT_eSprite img = TFT_eSprite(&tft);  // Create Sprite object "img" with pointer to "tft" object
// the pointer is used by pushSprite() to push it onto the TFT
#define CALIBRATION_FILE "/TouchCalData3"

// #define NUM_KEYS 6 // one more than the number of buttons.
#define TFT_GREY 0x5AEB // New colour
#define REPEAT_CAL false

//TFT_eSPI_Button key[NUM_KEYS];


bool isOn = false;  // 'On' puts the word "ON" in the green button else "OFF" in the red button
char msg;
int color = TFT_RED;

#define BLACK_SPOT   // Comment out to stop drawing black spots where the pointer touches

// Switch position and size
#define FRAME_X 165 
#define FRAME_Y 5
#define FRAME_W 150
#define FRAME_H 30

// Red zone size
#define REDBUTTON_X FRAME_X // Top Left
#define REDBUTTON_Y FRAME_Y
#define REDBUTTON_W (FRAME_W/2)
#define REDBUTTON_H FRAME_H

// Green zone size
#define GREENBUTTON_X (REDBUTTON_X + REDBUTTON_W)
#define GREENBUTTON_Y FRAME_Y
#define GREENBUTTON_W (FRAME_W/2)
#define GREENBUTTON_H FRAME_H

int printCounter = 0;
long startTime = millis();

int btnLEDTestCount = 0;
bool ledOn = false;
int ledId = 0;

uint16_t touchedX, touchedY;  // X, Y location of a stylus touch on the screen

// Class to create a button zone with a sprite and push it to the screen
// Also detects if the zone was touched on the touch screen
class ButtonZone {
   public:
      String btnText; 
      int btnUpperLeft_X;
      int btnUpperLeft_Y;
      int btnWidth;
      int btnHeight;
      int btnColor;
      bool touchNeedsProcessed = false;

   ButtonZone (int upperLeft_X, int upperLeft_Y, int width, int height, int color) {
      btnUpperLeft_X = upperLeft_X;
      btnUpperLeft_Y = upperLeft_Y;
      btnWidth = width;
      btnHeight = height;
      btnColor = color;
   }

  void pushToScreen(String text) {
    btnText = text;
    img.setColorDepth(8);
    img.createSprite(btnWidth, btnHeight);
    img.fillSprite(btnColor);
    img.setTextColor(TFT_BLACK);
    img.setTextSize(2);
    img.setTextDatum(MC_DATUM);  // MC = middle center
    img.drawString(text, img.width()/2, img.height()/2);
    img.drawRect(btnUpperLeft_X, btnUpperLeft_Y, btnWidth, btnHeight, TFT_BLACK);
    img.pushSprite(btnUpperLeft_X, btnUpperLeft_Y);
    img.deleteSprite();  // Delete it to free memory
  }

  bool touchDetected(int x, int y) {
      if ((x > btnUpperLeft_X) && (x < (btnUpperLeft_X + btnWidth)) &&
          (y > btnUpperLeft_Y) && (y < (btnUpperLeft_Y + btnHeight))) {
        
        touchNeedsProcessed = true;
        return true;   
      }    
    return false;
  }
};

// Buttons in a row across the bottom
// ButtonZone test1Button(8, 200, 96, 30, TFT_PINK);
// ButtonZone test2Button(113, 200, 96, 30, TFT_CYAN);
// ButtonZone test3Button(218, 200, 96, 30, TFT_GREENYELLOW);

// Buttons vertically on the lower left side
ButtonZone test1Button(8, 130, 96, 30, TFT_PINK);  // Creating 3 here rather than in loop(), saves 140 mSec
ButtonZone test2Button(8, 165, 96, 30, TFT_CYAN);
ButtonZone test3Button(8, 200, 96, 30, TFT_GREENYELLOW);

ButtonZone redButton(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, TFT_RED);
ButtonZone greenButton(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, TFT_GREEN);

void setup() {
  pinSet();
  Serial.begin(115200);
  EEPROM.begin(512);

  tft.init();
  tft.setRotation(3);
  delay(1000);  // needed to allow Serial.print statements to work in setup()
  touch_calibrate(tft);
  attachInterrupt(digitalPinToInterrupt(2), Encode, FALLING);

  tft.fillScreen(TFT_ORANGE);
  tft.setTextColor(TFT_BLACK, TFT_ORANGE);  tft.setTextSize(1);
  test1Button.pushToScreen("Test 1");
  test2Button.pushToScreen("Test 2");
  test3Button.pushToScreen("Test 3");

  Serial.println("End Setup");
}

void loop() {
  if (isOn){
    redButton.pushToScreen("");
    greenButton.pushToScreen("ON");
    Serial.println(millis() - startTime);
  }
  else {
    redButton.pushToScreen("Off");
    greenButton.pushToScreen("");
  }

  // test1Button.pushToScreen("Test 1");  // moving these 3 lines to setup changed the loop from 220 to 80 msec
  // test2Button.pushToScreen("Test 2");
  // test3Button.pushToScreen("Test 3");

  // See if there's any touch data for processing and if so, set a
  // flag to process the touch.  The flag is set in the button class
  if (tft.getTouch(&touchedX, &touchedY))
  {
    #ifdef BLACK_SPOT
      //tft.fillCircle(touchedX, touchedY, 2, TFT_BLACK);
      Serial.print("x= "); Serial.print(touchedX);Serial.print(" y= "); Serial.println(touchedY);
    #endif

    if (test1Button.touchDetected(touchedX, touchedY)) {
      // Serial.print ("--");
      // Serial.println(millis() - startTime);
      tone(bzr, 170, 75);
      // Serial.println(millis() - startTime);
    }
    
    if (test2Button.touchDetected(touchedX, touchedY))
      tone(bzr, 170, 75);
     
    if (test3Button.touchDetected(touchedX, touchedY)) {
      tone(bzr, 170, 75);
      tft.fillRect(113, 160, 300, 100, TFT_ORANGE);
    }

    if (redButton.touchDetected(touchedX, touchedY) && isOn) {
      isOn = false;
    }

    if (greenButton.touchDetected(touchedX, touchedY) && !isOn) {
      isOn = true;
    }
  }

  if (test1Button.touchNeedsProcessed){
     printCounter++;
    // if (printCounter % 20 == 0){
    //   Serial.print("Test Count: ");Serial.print(btnLEDTestCount );Serial.print(ledId); Serial.print("=");Serial.println(ledOn);
    // }

    if (btnLEDTestCount == 0) {
      tft.drawString("Verify: red LED      ", 113, 160, 4);
      tft.drawString("on when pushing      ", 113, 185, 4);
      tft.drawString("the red button       ", 113, 210, 4);
      if (!digitalRead(rPB) && !ledOn){
        btnLEDTestCount ++;
        startTime = millis();
        ledOn = true;
        ledId = rLED;
      }
    }
 
    if (btnLEDTestCount == 1) {
      tft.drawString("Verify: yellow LED   ", 113, 160, 4);
      tft.drawString("on when pushing      ", 113, 185, 4);
      tft.drawString("the yellow button    ", 113, 210, 4);
      if (!digitalRead(yPB) && !ledOn) {
        btnLEDTestCount ++;
        startTime = millis();
        ledOn = true;
        ledId = yLED;
      }
    }

    if (btnLEDTestCount == 2) {
      tft.drawString("Verify: blue LED     ", 113, 160, 4);
      tft.drawString("on when pushing      ", 113, 185, 4);
      tft.drawString("the blue button      ", 113, 210, 4);
      if (!digitalRead(bPB) && !ledOn) {
        btnLEDTestCount ++;
        startTime = millis();
        ledOn = true;
        ledId = bLED;
      }
    }
    
    if (btnLEDTestCount == 3) {
      tft.drawString("Verify: green LED    ", 113, 160, 4);
      tft.drawString("on when pushing      ", 113, 185, 4);
      tft.drawString("the green button     ", 113, 210, 4);
      if (!digitalRead(gPB) && !ledOn) {
          btnLEDTestCount ++;
          startTime = millis();
          ledOn = true;
          ledId = gLED;
      }
    }
  
    // Turn on the LED for 1 seconds
    digitalWrite(ledId, ledOn); 
    if (millis() > startTime + 1000){
      ledOn = false;
      digitalWrite(ledId, ledOn); 
    }
  
    // is the test finished?
    if (btnLEDTestCount == 4 && !ledOn){
      tft.fillRect(112, 160, 300, 100, TFT_ORANGE);  // clear the text field area
      tft.drawString("LED Test Passed     ", 113, 160, 4);
      test1Button.touchNeedsProcessed = false;
      btnLEDTestCount = 0;
    }
  }

  tft.setCursor(0, 0, 4);  // Set "cursor" at top left corner of display (0,0) and select font 2
  // Set the font colour to be white with a black background, set text size multiplier to 1
  tft.setTextColor(TFT_WHITE, TFT_BLACK);  tft.setTextSize(1);
  // We can now plot text on screen using the "print" class
  tft.println("Hello World ");
  // delay(500);
  tft.setCursor(100, 40, 6);  // Set "cursor" at top left corner of display (0,0) and select font 2
  // Set the font colour to be white with a black background, set text size multiplier to 1
  tft.setTextColor(TFT_BLACK, TFT_ORANGE);  tft.setTextSize(1);
  // We can now plot text on screen using the "print" class
  tft.print(count);  // Number font -  notice the letters are not printed
  tft.println("   ");  // add some space to blank out previous larger numbers
  if (count < 0)
    tft.fillCircle(50, 90, 30, TFT_RED);
  else
    tft.fillCircle(50, 90, 30, TFT_GREEN);

  // SK text experiment
  // tft.setTextFont(4);  // Font 2 is tiny.  Font 4 is readable
  // int posCursorX = tft.getCursorX();
  // int posCursorY = tft.getCursorY();  // y position is top left of the character to print
  // tft.print("Yo");tft.print(" Ho ");
  // printCounter++;
  // if (printCounter % 50 == 0){
  //   Serial.print("x= ");Serial.print(posCursorX);Serial.print("   y= ");Serial.println(posCursorY);
  //   printCounter = 0;
  // }
  // tft.println("You there now?");
  // tft.setCursor(tft.getCursorX()+100, tft.getCursorY());
  // tft.print("I");tft.println(" don't ");
  // tft.print("think so since this should be a really long");tft.println(" line");
  // tft.println("I will figure it out");
  // tft.println("This line could be off the screen");
  
  // tft.drawLine(10, 200, 210, 100, TFT_BLACK);
  // tft.drawRect(60, 180, 30, 50, TFT_CYAN);
  // tft.fillRect(100, 180, 30, 50, TFT_CYAN);
  // tft.fillCircle(50, 90, 30, color);
  // tft.drawCircle(55, 110, 50, color);
  // tft.fillTriangle(200, 220, 300, 220, 250, 133, tft.color565(250, 0, 250));
  // //tft.drawTriangle(217, 210, 283, 210, 250, 153, tft.color565(250, 250, 0));
  // tft.fillRoundRect(140, 140, 50, 90, 20, tft.color565(0, 30, 150));
  drawStar(216, 40, TFT_VIOLET);  // about 20-25 mSec to create/draw 2 sprites
  drawStar(249, 68, TFT_NAVY);
  
  // tft.setTextSize(2);
  // tft.drawString("And Kensi", 115, 125, 2);
}


// #########################################################################
// Create sprite, plot graphics in it, plot to screen, then delete sprite
// #########################################################################
void drawStar(int x, int y, int star_color)
{
  // Create an 8 bit sprite 70x 80 pixels (uses 5600 bytes of RAM)
  img.setColorDepth(8);
  img.createSprite(70, 80);
  //Serial.print("w= ");Serial.println(img.width());  //return sprite width = 70
  // Fill Sprite with a "transparent" colour
  // TFT_TRANSPARENT is already defined for convenience
  // We could also fill with any colour as "transparent" and later specify that
  // same colour when we push the Sprite onto the screen.
  img.fillSprite(TFT_TRANSPARENT);

  // Draw 2 triangles to create a filled in star. One is rotated 45 degrees
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
