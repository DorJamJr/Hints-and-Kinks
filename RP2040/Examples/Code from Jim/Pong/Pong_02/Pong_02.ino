/*
 *  PONG_02 - Improved Pong 
 *    JRA - 03/10/23
 *    Improvements over PONG_01
 *      - sound
 *      - score (# of successful returns)
 *      - ball speed increases as score builds up
 *      - paddle speed improvements
 *      - globals for screenColor and ballColor
 *      - leaderboard with entry of initials via rotary encoder
 *      - leaderboard maintained through power cycles via eeprom (offset 0x30)
 *
 *    NOTE on Pico EEPROM usage:
 *      Steve Kliewer found this execllent article on the EEPROM functions of the Pico:
 *        https://arduino-pico.readthedocs.io/en/latest/eeprom.html
 *      Of particular interest is that the Pico doesn't really have an EEPROM on board but 
 *        simulates it by using part of the flash memory. Doing so REQUIRES the use of the
 *        EEPROM.begin(int size) function passing a number between 256 and 4096 for the parameter
 *        size (so like "EEPROM.begin(512)"). Note that passing a parameter outside of this range
 *        results in the use of the boundary value (so like "EEPROM.begin(55)" is equivilant to 
 *        "EEPROM.begin(256)").
 *   
 */

#include <TFT_eSPI.h>
#include <PicoPins2.h>
#include <EEPROM.h>

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

int highScore[3];
char highInitials[3][4];
int eeprom_startAdr = 0x30;


void setup() {
  pinSet();
  Serial.begin (9600);
  // encoder function in PicoPins2.h ... result of encoder is in variable count
  attachInterrupt(digitalPinToInterrupt(2), Encode, FALLING);

  delay(2000);

  // get data from eeprom for high scores
  EEPROM.begin(256);   // see EEPROM comments in top header
  Serial.println("Before fetch...");
  eep_fetchScores();

  tft.init();                   // initialize the tft object
  tft.setRotation(3);           // we are using the screen in rotation orientation 3 (landscape) (1 gives x=0, y=0 at bottom right)
  splashScreen();
  initializeScreen();

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
    Serial.println("Game Over!");
    saveScore();                    // this function will determine if score should be saved
    splashScreen();
    initializeScreen();
  
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

void initializeScreen(){
  tft.fillScreen(screenColor);   // fill with background color
  tft.fillCircle(x+21,y+21,19,ballColor);   // place ball in initial position (21 is half size of img sprite box)
  tft.fillRect(xp,228,paddle_width,10,TFT_WHITE);      // place paddle in intitial position 2,228
}

void splashScreen(){
  tft.fillScreen(TFT_PURPLE);
  tft.setCursor(20,20,4);
  tft.setTextColor(TFT_WHITE);
  tft.print("PONG_02\n\n");
  tft.setCursor(20,180,4);
  tft.print("Press GREEN push button\n     to begin...\n");
  displayHighScores();
  while(digitalRead(gPB)){  }   // wait for button press to start
}

// display running score
void drawScore(){
  tft.fillRect(5, 5, 60, 32, TFT_GREEN);
  tft.setCursor(10,10,4);
  tft.setTextColor(TFT_BLACK,TFT_GREEN);
  tft.print(String(score));
}


/*
*     Score architecture:
*     Scoring info is carried in the EEPROM and transferred (when necessary) into two arrays, highScore[3] and highInitials[3][4].
*     The highest score is in highScore[0] and is associated with highInitials[0][].
*     Note that in the highInitials[][] array, we terminate the three initials of a person with a null character (highInitials[][3]=0)
*      so that we can easily convert it to a string.
*/

// fetch score info from eeprom to local highInitials[][] and highScore[] arrays
void eep_fetchScores(){
  int anInitial = 0;
    // fetch the currently stored initials
  for (int n=0;n<3;n++){       // n = individual
    for (int i=0;i<3;i++){    // i = initials
      anInitial = EEPROM.read(eeprom_startAdr+n*3+i); // initials in first 12 addresses
      if (anInitial != 255){
        highInitials[n][i] = char (anInitial);
      }
      else{
        highInitials[n][i] = 'X';
      }
      Serial.print(String(eeprom_startAdr+n*3+i));
      Serial.print("  =  ");
      Serial.println(String(highInitials[n][i]));
    }
    highInitials[n][3] = 0;       // null terminated char array can be assigned to a string;
    Serial.println(String(highInitials[n]));
  }
  // fetch the currently stored high scores
  for(int s=0; s<3; s++){
    highScore[s] = EEPROM.read(eeprom_startAdr + 9 + s);
    if (highScore[s] == 255){       // 255 implies not written yet so set to 0
      highScore[s] = 0;
    }
    Serial.print(String(eeprom_startAdr+9+s));
    Serial.print("  =  ");
    Serial.println(String(highScore[s]));
    Serial.println();
  }
}

// write current highInitials[][] and highScore[] to eeprom
void eep_saveScore(){

    // fetch the currently stored initials
  for (int n=0;n<3;n++){       // n = individual
    for (int i=0;i<4;i++){     // i = initials (note [3] is the null terminator)
      EEPROM.write(eeprom_startAdr+n*3+i,char(highInitials[n][i]) ); // initials in first 12 addresses   
    }
  }
  // fetch the currently stored high scores
  for(int s=0; s<3; s++){
    EEPROM.write(eeprom_startAdr + 9 + s, highScore[s]);
  }
  // do the commit
  if (EEPROM.commit()) {
   Serial.println("EEPROM successfully committed");
 } else {
   Serial.println("ERROR! EEPROM commit failed");
 }  
}

void displayHighScores(){
  bool validInitials = true;
  String myString;
  int charHeight = 30;
  int greenBoxY = 50;

  tft.fillRect(20, greenBoxY, 250, 4*charHeight, TFT_GREEN);
  tft.setCursor(25,greenBoxY + 5,4);
  tft.setTextColor(TFT_BLACK,TFT_GREEN);
  tft.print("High scores: ");
  // The eep_fetchScores function has already created the initials strings, so just display the info
  // note: highest score at highInitials[0][] 
  for (int n = 0; n<3; n++){
    tft.setCursor(25,greenBoxY + 5 + (n+1)*charHeight,4);
    myString = String(highInitials[n]) + "  " + highScore[n];
    tft.print(myString);
  }
}

void saveScore(){
  int rank;           // captures position for this score
  bool updateRank = false;

  for (int n=2;n>=0;n--){
    // start from lowest score (index=2) and check if it needs to be replaced
    if (score>highScore[n]){      
      rank = n;                 // this is the rank to be replaced
      updateRank = true;
      shiftHighScore(n);      // move score down to next
    }
  }
// now put the new high score and initials in place
    tft.fillRect(5, 40, 310, 150, TFT_GREEN);
    tft.setTextColor(TFT_BLACK,TFT_GREEN);

  if (updateRank == true){
    tft.setCursor(10,45,4);
    tft.print("  Congratulations!");
    tft.setCursor(10,80,4);
    tft.print("You're on the leaderboard!");
    delay(2000);
    highScore[rank] = score;
    captureInitials(rank);        // routine to allow player to enter their initials - saved in highInitials[rank][]
    eep_saveScore();              // save new rankings to eeprom
  }
  else{
      tft.setCursor(10,45,4);
      tft.print("GAME OVER!");
      delay(2000);
  }
}

// shift the score and initials from from passed index to next higher index (highest score is in index=0)
void shiftHighScore(int index){
  if (index == 2){    // this is the lowest score so nothing to shift
    return;
  }
  highScore[index+1] = highScore[index];
  for (int n=0; n<3; n++){
    highInitials[index+1][n] = highInitials[index][n];
  }
}

// use rotatry encoder to select letter, blue PB to go to next character, green PB to commit
void captureInitials(int rank){
  int position = 0;
  int charWidth = 30;
  int char_X, char_Y;           // character positions
  
  tft.setCursor(20,105,2);
  tft.print("Use encodeer knob to set initials, BLUE PB");
  tft.setCursor(20,120,2);
  tft.print("to next initial, GREEN PB to continue...");

  tft.fillRect(90, 150, 100, 36, TFT_RED);
  tft.setTextColor(TFT_YELLOW,TFT_RED);
  for (position=0;position<3;position++){       // pre-fill with former characters
     tft.setCursor(100+position*charWidth,155,4);
     tft.print(char(highInitials[rank][position]));
  }
  position=0;
  while (digitalRead(gPB)){                   // while gPB is up (HIGH) we are editing
    count = highInitials[rank][position];
    char_X = 95+position*charWidth;
    tft.drawLine(char_X+3,182,char_X+20, 182,TFT_YELLOW);   // underline actiove character
    while(digitalRead(bPB)){                  // bPB advances to next char
      if (count>'Z'){count='A';}
      if (count<'A'){count='Z';}
      if (highInitials[rank][position] != count){           // overwrite character if encoder (count) change
        tft.fillRect(char_X-5,150, 36, 30, TFT_RED);        // erase last char
        tft.setCursor(char_X+5,155,4);
        tft.print(char(count)); 
        highInitials[rank][position]=count;                 // store new initial in highInitials[rank][]
      }
      delay(50); 
      if (!digitalRead(gPB)){break;}          // we are done... break out of the entry of initials
 
    }
    if (!digitalRead(gPB)){break;}            // break out of the gPB loop

    // still editing... advance to next character
    while(!digitalRead(bPB)){}         // wait until bPB is released
    delay(50);                        // debounce
    tft.drawLine(char_X+3,182,char_X+20, 182,TFT_RED);      // remove former underline
    position++;
    if (position > 2){position=0;}

  }   // wait for button move to next

}
