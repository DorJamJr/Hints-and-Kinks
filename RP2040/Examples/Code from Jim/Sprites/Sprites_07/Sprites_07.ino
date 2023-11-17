/*
 *  Demonstration of moving a ball sprite across a background image
 *  
 *  Discoveries:
 *  - Creating a graphic sprite from jpg:
 *    - Use tool at http://rinkydinkelectronics.com/t_imageconverter565.php to convert jpg to hex (.c file)
 *    - include the .c file in the sketch (note size of image... found in the .c file header)
 
    // Recipie to move a ball across a background picture
         From a Bodmer answer to a similar question at https://github.com/Bodmer/TFT_eSPI/issues/508
          We only need the picture in memory (done with a #include of the image) and one sprite (ballSprite)

      - get the image into program memory (done with a "#include <BackgroundName.c">)
      - push the image to the screen with "tft.pushImage(0,0,bg_size_x,bg_size_y,BackgroundName);"
      - push the image to the ballSprite with "ballSprite.pushImage(-x,-y,bg_size_x,bg_size_y,BackgroundName);"
          where x,y are position on screen that the ball sprite will be placed and
          bg_size_x, bg_size_y are are the size of the background picture
          (Note that the -x,-y set the upper left corner of the background off the upper left corner of the ballSprite
          so that the result is the background image at the desired location of the ball on the background is placed 
          onto the ball sprite.) (Think about it and it will be clear).
      - create the ball itself on the ball sprite at a location 2 pixels inside of sprite size
      - push the ball sprite to the tft display with "ballSprite.pushSprite(x,y);"
      - see the printSprite() function in the example below

      Note: The tft screen does not display the Forest picture well due to gradients of light green being
             rendered as light grey. The "elephants" picture displays much better (deeper colors) and so is used 
             in this demo.

*/


#include <TFT_eSPI.h>
//#include "Forest.c"
#include "elephants.c"
TFT_eSPI tft = TFT_eSPI();                      // create tft object

// specify background image size (found in the Forest.c header)
int bg_size_x = 320;
//int bg_size_y = 193;    // Forest picture size
int bg_size_y = 180;      // elephants picture size

// define the ball sprite
TFT_eSprite ballSprite = TFT_eSprite(&tft);    // the Sprite for the arrow... will be placed on the canvas

// ball characteristics
int ball_size = 40;      // both x and y
int ballColor = TFT_YELLOW;
int x = 0;                                  // x location of Sprite starting at x=0
int y = 60;
int x_inc = 1;
int y_inc = 1;
 
void setup() {
  Serial.begin(9600);
  delay(5000);
  tft.init();                                // init the TFT screen
  tft.setRotation(3);                        // landscape (and rotated 180)
  tft.setSwapBytes(true);                    // handles endian-ness for 16 bit images
  tft.fillScreen(TFT_BLACK);                // direct write to TFT screen to fill

// push the Forest/elephants image to the screen (Forest/elephants is in memory)
//tft.pushImage(0,0,bg_size_x,bg_size_y,Forest_1);
tft.pushImage(0,0,bg_size_x,bg_size_y,elephants);

// create a ball sprite
ballSprite.createSprite(ball_size,ball_size);
ballSprite.setSwapBytes(true);

// call the printSprite() function to put the ball on the screen in the initial position
 printSprite(); 
}


void loop() {
Serial.println("Top of loop");
  printSprite();    // printSprite() does all the work to put the ball on the background
                                                       
 // move the ball around on the background
 x += x_inc;
 y += y_inc; 
 if ((x>(bg_size_x - ball_size)) || (x ==0)){
  x_inc = -x_inc;  
  }
  if ((y > (bg_size_y - ball_size)) || (y == 0)){
    y_inc = -y_inc;
  }
}

/*
printSprite() function

This function copies the background image at x,y onto the ballSprite, creates the ball itself on top
 of the bacground image leaving a 2 pixel surround and then pushes the sprite onto the tft. This technique
 allows the sprite to be successively placed at new positions (1 pixel position change max) to create motion
 over a background image. (We use a ball for demo purposes... other objects just require writing the object 
 to the sprite.)
*/
void printSprite(){
// locations x,y and size bg_size_x, bg_size_y are globals
//  ballSprite.pushImage(-x,-y,bg_size_x,bg_size_y,Forest_1);
  ballSprite.pushImage(-x,-y,bg_size_x,bg_size_y,elephants);
  ballSprite.fillCircle(ball_size/2,ball_size/2,ball_size/2-2,ballColor); // paint ball object on top of background
  ballSprite.pushSprite(x,y);             // push the sprite to the screen
}
