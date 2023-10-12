/*
 *  This code from YouTube - Volos Projects
 *                           Transparent Sprites - Programming Tutorial (TFT_eSPI library)
 *                           www.youtube.com/watch?v=U4jOFLFNZBL
 *                           
 *  Sprites_06.ino
 *  This is an extension of Sprites_03 to try to speed up the arrow by using an arrowSprite of size 2 pixels wider and taller than 
 *    the size of the arrow graphic. If the background on the arrow image is the same color as the background, we SHOULD be able 
 *    to move the arrowSprite around on the TFT (moves of 1 pixel in any direction) and not have to redraw the background (since
 *    the arrowSprite background will do that for free). Note we will not be able to use "transparent" features of sprites with
 *    this approach. We will use a black canvas for expediency since our current arrow image uses a black background.
 *  
 *  
 *  Discoveries:
 *  - Creating a graphic sprite from jpg:
 *    - Start with jpg image of sprite... black background
 *    - Trim to smallest square encompassing image
 *    - Use tool at http://rinkydinkelectronics.com/t_imageconverter565.php to convert jpg to hex (.c file)
 *    - Open .c in TextEdit (text editor) and copy entire file
 *    - Create new file in arduino IDE (ex: arrow.h) and paste entire contents there. (This is now the bitmap of the image)
 *    - Note size of sprite and name of sprite from the .h file and use that when using function pushImage()
 *  
 *  - A Sprite is a graphics screen kept in RAM... You can draw a completed sprit to TFT using pushSprite(x,y) 
 *  - This example uses 3 sprites: canvasSprite, textSprite and arrowSprite
 *  - We build up our frame to display by treating the canvasSprite as a hidden version of the TFT display, clearing graphics off
 *     of our canvas at the top of each loop, then putting our textSprite and arrowSprite onto the canvasSprite with their new
 *     positions/information and when the canvasSprite is complete, we do a canvasSprite.pushSprite() to write the completed page
 *     to the TFT screen in one instruction.
 *  - You can push a sprite onto another sprite screen to build up a full frame using pushToSprite(&parent,x,y,color)   
 *     where "parent" is the bigger sprite you are building, and color is the color of the image that will be made transparent
 * 
 *  - This example runs pretty slow (200 px x movement in one minute)
 *     - If I remove the final write of canvas to the TFT display, I get 3.92 seconds around the loop... (vs 96 seconds 
 *        with the canvas write to TFT).
 *     - So 92 seconds of the loop time is in TFT write... 320 writes/loop... => screen write takes 288 msec.   
 *     - Sanity check... our screen is 320x240, color 8 bit (?), Pico clock speed is 125 MHz. SPI is serial... so a full screen 
 *        refresh would require transfer of 320x240x8 bits (= 614,400 bits). Search on net shows "SPI speed on RPi upward of 8 MBits/s" 
 *        => 76.8 msec / refresh
 *     - SCOPE TESTS:   
 *        - SPI clock measured at 200 ns == 5MHz
 *        - SPI data transfer for 1 screen refresh = 280mS
 *     - BOTTOM LINE: "Slow" animation due to SPI full screen writes once each loop. To speed animation up, we need to abandon
 *        the canvasSprite concept and write small sprites (arrow and text) to screen directly...
 *        Test out a way to do that in Sprites_05
 */


#include <TFT_eSPI.h>
#include "arrow.h"
TFT_eSPI tft = TFT_eSPI();                      // create tft object

// define 3 sprites
TFT_eSprite arrowSprite = TFT_eSprite(&tft);    // the Sprite for the arrow... will be placed on the canvas
TFT_eSprite txtSprite = TFT_eSprite(&tft);      // the Sprite for the text box... will also be placed on the canvas



 
void setup() {
 tft.init();                                // init the TFT screen
 tft.setRotation(3);                        // landscape (and rotated 180)
 tft.setSwapBytes(true);                    // handles endian-ness for 16 bit images
 tft.fillScreen(TFT_BLACK);                // direct write to TFT screen to fill

// Create 2 sprites
 txtSprite.createSprite(40,40);             // the text sprite... box for text is 80x80
 txtSprite.setTextColor(TFT_WHITE,TFT_PURPLE);   // white text on black background (background black so we can make it transparent)
 arrowSprite.createSprite(66,56);
 arrowSprite.fillSprite(TFT_BLACK);
 arrowSprite.setSwapBytes(true);            // without this the colors are "a mess"

 arrowSprite.pushImage(1,1,62,54,arrow);    // write the arrow into the sprite space at location 1,1 of the Sprite space
                                            //  leaving a black boarder around the Sprite... (which will clear the previous Sprite 
                                            //  when plotting a new sprite)
                                            //  (this sprite won't change so we can do this once as long as we don't do deleteSprite())
 arrowSprite.pushSprite(40,60, TFT_BLACK);  // Push the sprite to the screen, 
                                            //  the last parm is the colow which will NOT be drawn (ie, transparent) 
}

int x = 0;                                  // x location of Sprite starting at x=0


void loop() {

 txtSprite.fillSprite(TFT_BLACK);                     // fill text sprite box with black wiping out earlier text
 txtSprite.drawString(String(x),0,0,4);               // string, x=0, y=0 (in sprite space), size = 6)
 txtSprite.pushSprite(20,150);                        // push the text to the background sprite making canvas (TFT_BLACK) transparent                                           

 arrowSprite.pushSprite(x,60);   //  Push the arrow sprite to the screen 
                                            
                                                        
 x+=1; 
 if (x>320){
  x=0;  
  }

}
