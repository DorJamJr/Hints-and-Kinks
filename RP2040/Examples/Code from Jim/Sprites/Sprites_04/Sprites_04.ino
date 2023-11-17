/*
 *  This code from YouTube - Volos Projects
 *                           Transparent Sprites - Programming Tutorial (TFT_eSPI library)
 *                           www.youtube.com/watch?v=U4jOFLFNZBL
 *                           
 *  This example shows the use of transparent Sprites
 *  
 *  Discoveries:
 *  - Start with jpg image of sprite... black background
 *  - Trim to smallest square encompassing image
 *  - Use tool at http://rinkydinkelectronics.com/t_imageconverter565.php to convert jpg to hex (.c file)
 *  - Open .c in TextEdit (text editor) and copy entire file
 *  - Create new file in arduino IDE (ex: arrow.h) and paste entire contents there (This is now the bitmap of the image)
 *  - Note size of sprite and name of sprite from the .h file and use that when using function pushImage()
 *  
 *  - This example is same as Sprites_03 with a arrow sprite half size (arrowS). It still runs pretty slow (200 px x movement in one minute)
 *  - The order of calling function pushToSprite() will dictate which sprite is on top (the last one pushed is on top)
 *  
 *  
 */


#include <TFT_eSPI.h>
#include "arrowS.h"
TFT_eSPI tft = TFT_eSPI();                      // create tft object
TFT_eSprite arrowSprite = TFT_eSprite(&tft);    // the Sprite for the arrow
TFT_eSprite background = TFT_eSprite(&tft);     // the Sprite for the background
TFT_eSprite txtSprite = TFT_eSprite(&tft);     // the Sprite for the background



 
void setup() {
 tft.init();
 tft.setRotation(3);
 tft.setSwapBytes(true);
 tft.fillScreen(TFT_PURPLE);

 background.createSprite(320,240);          // the background sprite... the full screen
 txtSprite.createSprite(80,80);             // the text sprite... box for text is 80x80
 txtSprite.setTextColor(TFT_WHITE,TFT_BLACK);   // white text on black background (background black so we can make it transparent)
 arrowSprite.createSprite(30,26);
 arrowSprite.setSwapBytes(true);            // without this the colors are "a mess"

 arrowSprite.pushImage(0,0,30,26,arrowS);   // push the arrow onto the sprite space and draw at the 0,0 of the Sprite space
                                            // only need to do this once since the sprite image isn't changing in this example
 arrowSprite.pushSprite(40,60, TFT_BLACK);  //  and now we push the sprite to the screen, 
                                            //  the last parm is the colow which will NOT be drawn (ie, transparent) 
}

int x = 0;       // x location of Sprite starting at x=40


void loop() {
 background.fillSprite(TFT_PURPLE);         // fill the background sprite with color
 txtSprite.fillSprite(TFT_BLACK);           // fill text sprite box with black
 txtSprite.drawString(String(x),0,0,6);     // string, x=0, y=0 (in sprite space), size = 6)

// arrowSprite.pushImage(0,0,62,54,arrow);    // if the image of arrowSprite were to change in the loop, we change it here

// The order in which the sprites are pushed determins which is on top (last pushed is on top)
 arrowSprite.pushToSprite(&background,x,60, TFT_BLACK);   //  and now we push the sprite to the background Sprite (not to screen) 
                                                          //  the last parm is the colow which will NOT be drawn (ie, transparent) 
 txtSprite.pushToSprite(&background,20,65,TFT_BLACK);     // push the text to the bacground sprite making background (TFT_BLACK) transparent                                           
                                            
 //So we have built up a big sprite (background) with two imbedded sprites (arrow and text) and now push it to the screen
 background.pushSprite(0,0);  
                                                        
 x+=1; 
 if (x>320) x=0;                                           

}
