// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include"PicoPins2.h"
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel pixels(1, 23, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

//Colors are RGB for red, green, blue values
int rValue = 255;  
int gValue = 0;
int bValue = 0;


void setup() {
  pinSet();
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.setBrightness(50); // Set BRIGHTNESS 10 = dim 255 = MAX
}

void loop() {
  pixels.clear(); // Set all pixel colors to 'off'

  if (digitalRead(rPB) == 0) { // Make the neoPixel   Red
    rValue = 255;  gValue = 0;  bValue = 0;  // RGB for Red
  }

  if (digitalRead(bPB) == 0) { // Make the neoPixel   Blue
    rValue = 0;  gValue = 0;  bValue = 255;  // RGB for Blue
  }

  if (digitalRead(gPB) == 0){  // Make the neoPixel   Green
    rValue = 0;  gValue = 255;  bValue = 0;  // RGB for Green
  }

  if (digitalRead(yPB) == 0){  // Turn the neoPixel OFF 
    rValue = 0;  gValue = 0;  bValue = 0; 
  }

  pixels.setPixelColor(0, pixels.Color(rValue, gValue, bValue)); //  RGB colors (Red,Green,Blue)
  pixels.show();   // Send the updated pixel colors to the hardware.

  delay(DELAYVAL); // Pause before next pass through loop

}
