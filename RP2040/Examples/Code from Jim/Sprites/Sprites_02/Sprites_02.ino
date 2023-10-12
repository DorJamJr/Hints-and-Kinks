/*
 *  This code from YouTube - Volos Projects
 *                           How and why to use Sprites (TFT_espi Programming tutorial)
 *                           www.youtube.com/watch?v=sRGXQg7028A
 *                           
 *  The main advantage in using Sprites is to reduce flicker in the display                         
 *  This example shows improvements to Sprites_01 which can be achieved using Sprites
 *  
 *  Discoveries:
 *     - So far I can't get this to work... I get a blank screen... ??
 */

#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();              // create tft object
// The following is to enable Sprites
TFT_eSprite img = TFT_eSprite(&tft);    // declare the tft Sprite named img


void setup() {
  tft.init();                   // initialize the tft object
  tft.setRotation(3);           // we are using the screen in rotation orientation 3 (landscape) (1 gives x=0, y=0 at bottom right)
  img.createSprite(240,135);    // create the Sprite (this one is the size of the full screen)
  
 // tft.fillScreen(TFT_BLACK);    // empty the screen buffer so our canvas is BLACK
  
}

int x=0;                        // contains horizontal position of our circle

void loop() {
  img.fillCircle(x,36,30,TFT_BLACK);;  // remove the previously drawn circle
  x += 1;
  if (x>320){                               // screen is 320x240
    x=0;                                   
  }
  img.fillCircle(x,36,30,TFT_RED);          // draw at horizontal position = x
                                            //         vertical position = 36
                                            //         radius = 30
                                            //         color = TFT_RED
                                      
  img.fillRect(18,148,100,50,TFT_BLUE);     // fill rectangle... x, y, width, height, color (use blue so we can see where it is drawn)
  img.drawString(String(x), 20,150, 7);     // text draw... string, x_pos, y_pos, font size (fornt 7 is about 50 px high, 30 px wide)
  img.pushSprite(0,0);                                    
}
