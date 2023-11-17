/*
 *  This code from YouTube - Volos Projects
 *                           How and why to use Sprites (TFT_espi Programming tutorial)
 *                           www.youtube.com/watch?v=sRGXQg7028A
 *                           
 *  The main advantage in using Sprites is to reduce flicker in the display                         
 *  This example is the first stepping stone to understanding Sprites and demos what it looks like without them
 *  
 *  Discoveries:
 *     - To prepare the Pico for code download, hold down "Boot" button, press/release "RST" button, release "Boot" button
 *     - The simplest version of this code didn't clear the background buffer so we need a fill screen in the setup()
 *     - Screen orientation "1" puts circle at bottom right... use screen orientation "3"
 *     - drawString() function 4th param is font size
 */

#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

void setup() {
  tft.init();                   // initialize the tft object
  tft.setRotation(3);           // we are using the screen in rotation orientation 3 (landscape) (1 gives x=0, y=0 at bottom right)
  tft.fillScreen(TFT_BLACK);    // empty the screen buffer so our canvas is BLACK
  
}

int x=0;                        // contains horizontal position of our circle
int x_prev=0;

void loop() {
  x += 1;
  tft.fillCircle(x_prev,36,30,TFT_BLACK);;  // remove the previously drawn circle
  tft.fillCircle(x,36,30,TFT_RED);          // draw at horizontal position = x
                                            //         vertical position = 36
                                            //         radius = 30
                                            //         color = TFT_RED
                                      
  x_prev=x;                                 // remember this position to blank it next time 
  tft.fillRect(18,148,100,50,TFT_BLUE);     // fill rectangle... x, y, width, height, color (use blue so we can see where it is drawn)
  tft.drawString(String(x), 20,150, 7);     // text draw... string, x_pos, y_pos, font size (fornt 7 is about 50 px high, 30 px wide)
  if (x>320){                               // screen is 320x240
    x=0;                                   
  }
                                      
}
