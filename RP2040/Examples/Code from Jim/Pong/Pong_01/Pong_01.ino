/*
 *  Improved Pong. 
 *    02/24/23
 *    Added sound, score (# of successful returns), ball speed increases as score builds up,
 *     paddle speed improvements, globals for screenColor and ballColor.
 *  
 *  
 *  Discoveries:
 *  - The technique of placing and erasing (ball and paddle) is to surround the object with a 
 *     boarder of 2 pixels of the BACKGROUND color so that a one pixel move with automatically
 *     erase the necessary pixels of the previous object placement. The removes the need for an
 *     erase step for the object movement, giving smooth graphics and speeding up movement
 *  - Removing center pixels from a moving object (ball and paddle) also speeds up movement
 *     since those pixels do not need to be re-written (since they haven't changed)
 *  - Note that speed is directly related to the number of pixels that must be written during
 *     a move. Minimize that and you maximize speed.
 *  - Text sprite not needed... just put text to screen with tft.print()
 *  - Still an anomoly where ball sticks to paddle occassionally (then rolls off)
 *  - IDE 2.0 editor does not handle comments in the same way as earlier IDE 
 *     (no auto astarisk and auto indent when doing block comment). You can do Option-Shift-A
 *     to create a block comment pair, but the form will NOT include the interviening "*" at
 *     in between line beginnings. Annoying!
 *  
 */

#include <TFT_eSPI.h>
#include <PicoPins2.h>

TFT_eSPI tft = TFT_eSPI();                    // create tft object
TFT_eSprite ball = TFT_eSprite(&tft);         // declare the Sprites
TFT_eSprite paddleSprite = TFT_eSprite(&tft);

uint32_t screenColor = TFT_BLUE;
uint32_t ballColor = TFT_YELLOW;

// Initialize variables
int ball_size = 42;      // both x and y
int paddle_height = 10;
int paddle_width = 46;
int x_max = 320-ball_size;
int x_min = 0;
int y_max = 240-ball_size;
int y_min = 0;
int xp = 120;                   // x position of paddle
int xJSval = 0, xJSnom = 520;
bool out_of_bounds = false;
int score = 0;                 // count of successful paddle hits
bool redraw_score = true;

int x=35,y=45;                        // contains horizontal/vertical position of our ball
int x_inc = 1, y_inc=1;                 // initial increments for ball x and y 
#define LEFT  -1; 
#define RIGHT +1;
#define UP    -1;
#define DOWN  +1;
int x_dir = RIGHT; 
int y_dir = DOWN;
int speed_delay = 10;           // initial loop delay in msec


void setup() {

//  Serial.begin (9600);
  tft.init();                   // initialize the tft object
  tft.setRotation(3);           // we are using the screen in rotation orientation 3 (landscape) (1 gives x=0, y=0 at bottom right)
  tft.fillScreen(screenColor);   // fill with background color
  tft.fillCircle(x+21,y+21,19,ballColor);   // place ball in initial position (21 is half size of img sprite box)
  tft.fillRect(xp,228,paddle_width,10,TFT_WHITE);      // place paddle in intitial position 2,228


// create the paddle sprite
  paddleSprite.createSprite(paddle_width+8,paddle_height);    // the paddle sprite
  paddleSprite.fillSprite(screenColor);                        // same as background so edges erase previous position
  paddleSprite.fillRect(4,0,paddle_width,paddle_height,TFT_WHITE);  // top left corner (x,y), width, height, color
  paddleSprite.fillRect(8,0,paddle_width-8,paddle_height,TFT_BLACK);

// create the moving sprite  
  ball.createSprite(ball_size,ball_size);      // create the ball Sprite
  ball.fillSprite(TFT_BLACK);
  ball.fillCircle(ball_size/2,ball_size/2,ball_size/2,screenColor);      // create yellow ball with purple surround
  ball.fillCircle(ball_size/2,ball_size/2,ball_size/2-2,ballColor);    // 2 pixel boarder
  ball.fillCircle(ball_size/2,ball_size/2,ball_size/2-4,TFT_BLACK);     //  now modify sprite by removing the center (make transparent)

  // setup the joystick to control the paddle
  pinSet();
  xJSnom = analogRead(xJS); // zero value of JoyStick
  
}


void loop() {
  
  x = x+x_inc * x_dir;    // next position of ball
  y = y+y_inc * y_dir;
  
  ball.pushSprite(x,y,TFT_BLACK);   // put ball in new position
  
  if (out_of_bounds == false){
  // put the paddle in the new position  
    xJSval = analogRead(xJS);
    xp = xp + (xJSval - xJSnom)/120;
    if (xp > 320-paddle_width){
      xp = 320-paddle_width; 
    }                       // keep paddle on screen
    if (xp < 0){
      xp = 0; 
    }
    paddleSprite.pushSprite(xp,228,TFT_BLACK);
    
  // when we hit the left, right or top edges, change direction (top left = 0,0)
    if (x >= x_max){ 
      x_dir = LEFT;
      tone(bzr,1500,100);
    } 
    if (x <= x_min){ 
      x_dir = RIGHT;
            tone(bzr,1500,100);
    }
    if (y <= y_min){ 
      y_dir = DOWN;
            tone(bzr,1500,100);
    }
    
  /// now check to see if the ball has hit the paddle
    if ((y >= 228 - ball_size -2) && 
        (xp +2 <= x + 10 + ball_size/2) && 
        (xp + 2 + paddle_width >= x + -10 + ball_size/2)){   // hits the paddle
      y_dir = UP; 
      tone(bzr, 1000, 100);  
      score += 1;
      drawScore();
      speed_delay = 10 - score/3;
      if (speed_delay < 2){speed_delay = 2;}
      Serial.print("Hit paddle!   Speed=");
      Serial.println(speed_delay); 
    }
    else if (y >= 228 - ball_size-2){          // missed the paddle...
      out_of_bounds = true;   
    }

  }
  
  if (y>240){   // You Loose
    tft.fillRect(65, 5, 180, 32, TFT_GREEN);
    tft.setCursor(70,10,4);
    tft.setTextColor(TFT_BLACK,TFT_GREEN);
    tft.print("GAME OVER!");
    Serial.println("Game Over!");

    delay(2000);
  
    x=35;                                       // restart game
    y=45;
    tft.fillRect(5,5,250,50,screenColor);                // blank the endMessage
    tft.fillRect(xp,228,paddle_width,10,TFT_WHITE);       // redraw paddle at last position
    tft.fillCircle(x+21,y+21,19,ballColor);              // place ball in initial position
    out_of_bounds = false;
    score = 0;
    speed_delay = 10;
    drawScore();
  }

  // Handle ball passing through score display region
  if ((x<65) && (y< 35) && (redraw_score == false)){     // blank the score until ball clear area
    tft.fillRect(5, 5, 60, 32, screenColor);
    redraw_score = true;
  }
  if (((x>70) || (y>40)) && (redraw_score == true)){    // redraw the score
    redraw_score = false;
    drawScore();
  }
  delay (speed_delay);    // debug
                                      
}


void drawScore(){
  tft.fillRect(5, 5, 60, 32, TFT_GREEN);
  tft.setCursor(10,10,4);
  tft.setTextColor(TFT_BLACK,TFT_GREEN);
  tft.print(String(score));

}
