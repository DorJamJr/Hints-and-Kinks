/*
   Color Maker
   by - Steve Kliewer
   This program uses programmable RGB values for the NeoPixel (or strip of them)
   The 'pot' controls brightness and the encoder wheel makes finer adjustments
   Color presets are included as starting points for the encoder
*/

#include <SPI.h>
#include <EEPROM.h>
#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <Adafruit_NeoPixel.h>

TFT_eSPI tft = TFT_eSPI(); 
#include "touchCalibrate.h"
#include "PicoPins2.h"

Adafruit_NeoPixel pixels(31, 23, NEO_GRB + NEO_KHZ800);

// the pointer is used by pushSprite() to push it onto the TFT
TFT_eSprite img = TFT_eSprite(&tft);  // Create Sprite object "img" with pointer to "tft" object

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

// Set up Red preset buttons
ButtonZone red255Button(8, 130, 96, 30, TFT_RED);
ButtonZone red128Button(8, 165, 96, 30, TFT_RED);
ButtonZone red000Button(8, 200, 96, 30, TFT_RED);

// Set up Green preset buttons
ButtonZone green255Button(112, 130, 96, 30, TFT_GREEN);
ButtonZone green128Button(112, 165, 96, 30, TFT_GREEN);
ButtonZone green000Button(112, 200, 96, 30, TFT_GREEN);

// Set up Blue preset buttons
ButtonZone blue255Button(216, 130, 96, 30, TFT_BLUE);
ButtonZone blue128Button(216, 165, 96, 30, TFT_BLUE);
ButtonZone blue000Button(216, 200, 96, 30, TFT_BLUE);

int redValue = 128;   // default red value
int greenValue = 128; // default green value
int blueValue = 128;  // default blue value

uint16_t touchedX, touchedY;  // X, Y location of a stylus touch on the screen
int brightness;  // This will be between 0 and ~100 based on the 'pot' value
String workingColor = "red";  // default starting selected color

void setup() {
  pinSet();
  attachInterrupt(digitalPinToInterrupt(encA), changeColor, FALLING); //SK: think this should be encA not '2'

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  
  Serial.begin(9600);
  EEPROM.begin(512);

  tft.init();
  tft.setRotation(3);
  // Can't use the line below unless you are connected to the IDE (so that Serial can set up)
  // while (!Serial);  // Should stay on this line until the 'Serial' port is alive
  delay(1500);  // Just some extra margin for the Serial.print statements to work in setup()
  touch_calibrate(tft);
  attachInterrupt(digitalPinToInterrupt(2), Encode, FALLING);

  tft.setTextDatum(MC_DATUM);  // MC = middle center

  tft.fillScreen(TFT_ORANGE);
  tft.setTextColor(TFT_BLACK, TFT_ORANGE);  tft.setTextSize(1);

  tft.drawString("Color Maker", 160, 20, 4);
  
  tft.fillCircle(55, 93, 25, TFT_RED);
  tft.fillCircle(160, 93, 25, TFT_GREEN);
  tft.fillCircle(266, 93, 25, TFT_BLUE);

  red255Button.pushToScreen("255");
  red128Button.pushToScreen("128");
  red000Button.pushToScreen("0");

  green255Button.pushToScreen("255");
  green128Button.pushToScreen("128");
  green000Button.pushToScreen("0");

  blue255Button.pushToScreen("255");
  blue128Button.pushToScreen("128");
  blue000Button.pushToScreen("0");

  circleColor(workingColor);

  showRGBValue(redValue, "red");
  showRGBValue(greenValue, "green");
  showRGBValue(blueValue, "blue");

  Serial.println("End Setup");
}

void loop() {
  brightness = analogRead(pot) / 15; // the /15 dims it a little.  Could have used map()
  pixels.setBrightness(brightness+1);  // +1 prevents it from being completely off

  if (tft.getTouch(&touchedX, &touchedY))
  {
    Serial.print("X= "); Serial.print(touchedX);Serial.print("   Y= "); Serial.println(touchedY);
    // Detect a Red button
    if (red255Button.touchDetected(touchedX, touchedY)) { 
      redValue = 255;
      workingColor = "red";
    }
    
    else if (red128Button.touchDetected(touchedX, touchedY)) {
      redValue = 128;
      workingColor = "red";
    }
    
    else if (red000Button.touchDetected(touchedX, touchedY)) {
      redValue = 0;
      workingColor = "red";   
    }

    // Detect a Green button
    else if (green255Button.touchDetected(touchedX, touchedY)) {
      greenValue = 255;
      workingColor = "green";
    }

    else if (green128Button.touchDetected(touchedX, touchedY)) {
      greenValue = 128;
      workingColor = "green";
    }
    
    else if (green000Button.touchDetected(touchedX, touchedY)) {
      greenValue = 0;
      workingColor = "green";
    }

    // Detect a Blue button
    else if (blue255Button.touchDetected(touchedX, touchedY)) {
      blueValue = 255;
      workingColor = "blue";
    }
    
    else if (blue128Button.touchDetected(touchedX, touchedY)){
      blueValue = 128;
      workingColor = "blue";
    }
     
    else if (blue000Button.touchDetected(touchedX, touchedY)) {
      blueValue = 0;
      workingColor = "blue";
    }

    // These 3 if clauses allow you to select a color without clicking
    // one of the preset (255, 128, 0) buttons
    else if (touchedX < 106)
      workingColor = "red";
    else if (touchedX > 214)
      workingColor = "blue";
    else
      workingColor = "green";
      

    circleColor(workingColor);

    showRGBValue(redValue, "red");
    showRGBValue(greenValue, "green");
    showRGBValue(blueValue, "blue");
  }

  adjustColor();
  // Serial.print("R= ");Serial.print(redValue);
  // Serial.print("  G= ");Serial.print(greenValue);
  // Serial.print("  B= ");Serial.println(blueValue);
  for (int i=0; i<31; i++) // adds support for the full strip if present 
    pixels.setPixelColor(i, pixels.Color(redValue, greenValue, blueValue)); //  RGB colors (Red,Green,Blue)
  pixels.show();   // Send the updated pixel colors to the hardware.

  delay(100);
}

// Manually adjust any of the RGB values based on which circle is highlighted
void adjustColor() {
  if (workingColor == "red") {
    redValue = constrain (redValue + count, 0, 255);  // Keep R portion of RGB betweeen 0 - 255
    showRGBValue(redValue, "red");
  }
  else if (workingColor == "green") {
    greenValue = constrain (greenValue + count, 0, 255);
    showRGBValue(greenValue, "green");
  }
  else if (workingColor == "blue") {
    blueValue = constrain (blueValue + count, 0, 255);
    showRGBValue(blueValue, "blue");
  }
  count = 0;
}

// This is the interupt handler from the encoder wheel.  count is
// used in the adjustColor() routine to make fine adjustments to color
void changeColor(){ // this routine is called if encB transitions from HIGH to LOW
  if (digitalRead(encB)) // Based on the state of encA we know the direction
    count++;
  else
    count--;
}

// Draw a circle (meaning it is selected) around the selected color
void circleColor(String color){ 
  // first delete any old black circles (3 circles make it 'bolder')
  for (int location: {55, 160, 266} ) {
    for (int radius: {26, 27, 28} )
      tft.drawCircle(location, 93, radius, TFT_ORANGE);
  }

  int xLocation = 0;
  if (color == "red") {
    xLocation = 55;
  }
  else if (color == "green") {
    xLocation = 160;
  }
  else if (color == "blue") {
    xLocation = 266;
  }

  // Actually draws 3 circles (slighly different radii) to make them 'bolder'
  for (int i=26; i<=28; i++)
    tft.drawCircle(xLocation, 93, i, TFT_BLACK);

}

// shows/prints the current RGB value above the colored circles
void showRGBValue(int value, String color){ 
  int xLocation = 0;
  if (color == "red") {
    xLocation = 55;
  }
  else if (color == "green") {
    xLocation = 160;
  }
  else if (color == "blue") {
    xLocation = 266;
  }

  tft.drawString("   " + String(value) + "   ", xLocation, 52, 4);
}
