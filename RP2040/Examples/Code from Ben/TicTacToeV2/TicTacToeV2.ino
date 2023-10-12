/*
   TicTacToeV2
   by - B.Jack
   This program first asks a player their initials
   which are then inserted onto scoreboard then the
   player places an x and the computer places an o,
   working to beat or block the human player. This
   code also stores and prints the score of each
   player while it is on.

   Later versions may use EEPROM to store high
   scores with initials which are displayed before
   the game

   Board Layout:
   #########################
   #       #       #       #
   # box 1 # box 4 # box 7 #
   #       #       #       #
   #########################
   #       #       #       #
   # box 2 # box 5 # box 8 #
   #       #       #       #
   #########################
   #       #       #       #
   # box 3 # box 6 # box 9 #
   #       #       #       #
   #########################
*/
#include <SPI.h>
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();
#include"PicoPins2.h"
#include <EEPROM.h>
#include <PNGdec.h>
#include"touchCalibrate.h"
#include "FireWorks.h"
#include "Graphics.h"
#include "Cat.h"
#include"Musical_Notes.h"
#include "Irish_Jig.h"

#define X_OR_O

PNG png; // PNG decoder inatance

#define MAX_IMAGE_WDITH 240// Adjust for your images

int tempo = 200;
bool keepPlay = true;
int16_t xpos = 0;
int16_t ypos = 0;

uint16_t x, y;
int initiateTime;
bool winnerX = false;
bool winnerO = false;
bool ccat = false;
int touchX;
int touchY;
int touch;
int Xcoord;
int Ycoord;

int scoreX;
int scoreO;

String initials = "BJ";
String computer = "AI";

int a; int b; int c; int d; int e; int f; int g; int h; int i;
int aX; int bX; int cX; int dX; int eX; int fX; int gX; int hX; int iX;
int aO; int bO; int cO; int dO; int eO; int fO; int gO; int hO; int iO;
int box1; int box2; int box3; int box4; int box5; int box6; int box7; int box8; int box9;

int box1x = 40 + 10; //top left box x (box1)
int box1y = 100 - 5; //top left box y

int box2x = 40 + 10; //middle left box x (box2)
int box2y = 160 - 5; //middle left box y

int box3x = 40 + 10; //bottom left box x (box3)
int box3y = 220 - 5; //bottom left box y

int box4x = 100 + 10; //middle top box x (box4)
int box4y = 100 - 5; //middle top box y

int box5x = 100 + 10; //middle middle box x (box5)
int box5y = 160 - 5; //middle middle box y

int box6x = 100 + 10; //middle bottom box x (box6)
int box6y = 220 - 5; //middle bottom box y

int box7x = 160 + 10; //top right box x (box7)
int box7y = 100 - 5; //top right box y

int box8x = 160 + 10; //middle right box x (box8)
int box8y = 160 - 5; //middle right box y

int box9x = 160 + 10; //bottom right box x (box9)
int box9y = 220 - 5; //bottom right box y
// // // // // // // // // // // // // // // // // // // // // // // // // // // // //
void setup() {
  pinSet();
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(gPB), PBchange, FALLING);
  attachInterrupt(digitalPinToInterrupt(gPB), PBchange, FALLING);

  EEPROM.begin(512);

  tft.init();
  tft.setRotation(2);

  touch_calibrate();
  tft.fillScreen(TFT_BLACK);

  //pre-game graphics. Displays until screen is touched
  tft.setCursor(0, 7.5, 2); //top left
  tft.setTextColor(TFT_WHITE, TFT_BLACK);// white text black surround
  tft.println("Tic Tac Toe V2");//game name
  tft.setCursor(240 / 2 - 75, 320 / 2 - 5, 4); // middle
  tft.println("Touch screen");
  tft.println("                 to start");

  while (!tft.getTouch(&x, &y)) {} //keeps Xturn from starting until screen is touched
  graphics();
}
// // // // // // // // // // // // // // // // // // // // // // // // // // // // //
void loop() {
  while (!tft.getTouch(&x, &y)) {} //keeps Xturn from starting until screen is touched
  turnP();
  if (winnerX == true) {
    Xwin();
  }
  if (ccat == true) {
    catGame();
  }
  while (!tft.getTouch(&x, &y)) {} //keeps Xturn from starting until screen is touched

  turnC();
  if (winnerO == true) {
    Owin();
  }
  if (ccat == true) {
    catGame();
  }
}
//##################TURNS##################################################################################################
//#########################################################################################################################
void  turnP() {
  //while{}
  Touch("X", TFT_RED);

}
void turnC() {
  Touch("O", TFT_BLUE);
}

//######################GAME_ENDINGS#########################################################################################################

//###########################################################################################################################################
void Owin() {
  //Serial.print("oWin");
  scoreO++;
  delay(500);
  Fireworks();
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(0, 7.5, 2);
  tft.print("O Win");
  tft.setCursor(0, 7.5, 2);
  delay(2000);
  tft.fillScreen(TFT_BLACK);
  delay(200);
  tft.println("You Lose");
  tft.setCursor(240 / 2 - 75, 320 / 2 - 5, 4);
  tft.println("Restarting...");
  delay(1000);
  graphics();
  a = 0;  b = 0;  c = 0;  d = 0;  e = 0;  f = 0;  g = 0;  h = 0;  i = 0;
  aX = 0;  bX = 0;  cX = 0;  dX = 0;  eX = 0;  fX = 0;  gX = 0;  hX = 0;  iX = 0;
  aO = 0;  bO = 0;  cO = 0;  dO = 0;  eO = 0;  fO = 0;  gO = 0;  hO = 0;  iO = 0;
}

void Xwin() {
  //Serial.print("xWin");
  scoreX++;
  delay(500);
  Fireworks();
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(0, 7.5, 2);
  tft.print("X");
  tft.setCursor(0, 7.5, 2);
  delay(2000);
  tft.fillScreen(TFT_BLACK);
  delay(200);
  tft.println("You Win");
  tft.setCursor(240 / 2 - 75, 320 / 2 - 5, 4);
  tft.println("Restarting...");
  delay(1000);
  graphics();
  a = 0;  b = 0;  c = 0;  d = 0;  e = 0;  f = 0;  g = 0;  h = 0;  i = 0;
  aX = 0;  bX = 0;  cX = 0;  dX = 0;  eX = 0;  fX = 0;  gX = 0;  hX = 0;  iX = 0;
  aO = 0;  bO = 0;  cO = 0;  dO = 0;  eO = 0;  fO = 0;  gO = 0;  hO = 0;  iO = 0;
}
void catGame() {
  delay(500);
  cat();
  delay(2000);
  tft.fillScreen(TFT_BLACK);
  //displays until screen is touched
  tft.setCursor(240 / 2 - 75, 320 / 2 - 5, 4);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println("Cat");
  tft.println("Restarting...");
  delay(2000);
  graphics();
  a = 0;  b = 0;  c = 0;  d = 0;  e = 0;  f = 0;  g = 0;  h = 0;  i = 0;
  aX = 0;  bX = 0;  cX = 0;  dX = 0;  eX = 0;  fX = 0;  gX = 0;  hX = 0;  iX = 0;
  aO = 0;  bO = 0;  cO = 0;  dO = 0;  eO = 0;  fO = 0;  gO = 0;  hO = 0;  iO = 0;
}
//############################################TOUCH###########################################################################
//############################################################################################################################
//############################################################################################################################
void Touch(String XorO, int TFTcolor) {
  Serial.print(x);
  Serial.print(" ");
  Serial.println(y);
  if (x >= 30 && x <= 90 && y >= 90 && y <= 148 && box1 == 0) { //box 1
    Xcoord = box1x;
    Ycoord = box1y;
    box1++;
    aO++;
  }
  if (x >= 30 && x <= 90 && y >= 148 && y <= 208 && box2 == 0) { //box 2
    Xcoord = box2x;
    Ycoord = box2y;
    box2++;
    bO++;
  }
  if (x >= 30 && x <= 90 && y >= 208 && y <= 270 && box3 == 0) { //box 3
    Xcoord = box3x;
    Ycoord = box3y;
    box3++;
    cO++;
  }
  //##############################################
  if (x >= 90 && x <= 150 && y >= 90 && y <= 148 && box4 == 0) { //box 4
    Xcoord = box4x;
    Ycoord = box4y;
    box4++;
    aO++;
  }
  if (x >= 90 && x <= 150 && y >= 148 && y <= 208 && box5 == 0) { //box 5
    Xcoord = box5x;
    Ycoord = box5y;
    box5++;
    bO++;
  }
  if (x >= 90 && x <= 150 && y >= 208 && y <= 270 && box6 == 0) { //box 6
    Xcoord = box6x;
    Ycoord = box6y;
    box6++;
    cO++;
  }
  //###############################################
  if (x >= 150 && x <= 270 && y >= 90 && y <= 148 && box7 == 0) { //box 7
    Xcoord = box7x;
    Ycoord = box7y;
    box7++;
    aO++;
  }
  if (x >= 150 && x <= 270 && y >= 148 && y <= 208 && box8 == 0) { //box 8
    Xcoord = box8x;
    Ycoord = box8y;
    box8++;
    bO++;
  }
  if (x >= 150 && x <= 270 && y >= 208 && y <= 270 && box9 == 0) { //box 9
    Xcoord = box9x;
    Ycoord = box9y;
    box9++;
    cO++;
  }
  if (x >= 0 && x <= 240 && y >= 0 && y <= 240 / 4) {} //padding around top
  if (x >= 0 && x <= 30 && y >= 0 && y <= 320) {} //padding around left
  if (x >= 210 && x <= 240 && y >= 0 && y <= 320) {} //padding around right
  if (x >= 0 && x <= 240 && y >= 270 && y <= 320) {} //padding around bottom

  tft.setCursor(Xcoord, Ycoord, 2); //which box to print in
  tft.setTextColor(TFTcolor, TFT_BLACK);
  tft.setTextSize(2);
  tft.print(XorO);// X in box
  delay(500);

  /*
    Serial.print("variables= ");
    Serial.print(a);Serial.print(b);Serial.print(c);Serial.print(d);Serial.print(e);Serial.print(f);Serial.print(g);Serial.print(h);Serial.println(i);
    Serial.print(aX);Serial.print(bX);Serial.print(cX);Serial.print(dX);Serial.print(eX);Serial.print(fX);Serial.print(gX);Serial.print(hX);Serial.println(iX);
    Serial.print(aO);Serial.print(bO);Serial.print(cO);Serial.print(dO);Serial.print(eO);Serial.print(fO);Serial.print(gO);Serial.print(hO);Serial.println(iO);
  */

  if ((aX >= 1 && bX >= 1 && cX >= 1) || (dX >= 1 && eX >= 1 && fX >= 1) || (gX >= 1 && hX >= 1 && iX >= 1) || (aX >= 1 && eX >= 1 && iX >= 1) ||
      (gX >= 1 && eX >= 1 && cX >= 1) || (cX >= 1 && fX >= 1 && iX >= 1) || (bX >= 1 && eX >= 1 && hX >= 1) || (aX >= 1 && dX >= 1 && gX >= 1)) { //all possible wins for X
    //Serial.println("Xwin")
    winnerX = true;
  }


  if (box1 == 1 && box2 == 1 && box3 == 1 && box4 == 1 && box5 == 1 && box6 == 1 && box7 == 1 && box8 == 1 && box9 == 1) { //all boxes filled (cat game)
    //Serial.print("cat");
    ccat = true;
  }
}
//###################################COMPUTER_TURN#############################################################################

//#############################################################################################################################
void computerTurn() {

  if ((aO >= 1 && bO >= 1 && cO >= 1) || (dO >= 1 && eO >= 1 && fO >= 1) || (gO >= 1 && hO >= 1 && iO >= 1) || (aO >= 1 && eO >= 1 && iO >= 1) ||
      (gO >= 1 && eO >= 1 && cO >= 1) || (cO >= 1 && fO >= 1 && iO >= 1) || (bO >= 1 && eO >= 1 && hO >= 1) || (aO >= 1 && dO >= 1 && gO >= 1)) { //all possible wins for Y
    //Serial.println("Owin")
    winnerO = true;
  }

}
//###################################GRAPHICS##################################################################################

//#############################################################################################################################
void Fireworks() {
  int16_t rc = png.openFLASH((uint8_t *)FireWorks, sizeof(FireWorks), pngDraw);
  //int16_t rc = png.openFLASH((uint8_t *)Image, sizeof(Image), pngDraw);
  Serial.print("Top of loop.  rc = ");
  Serial.println(rc);

  if (rc == PNG_SUCCESS) {
    Serial.println("Successfully png file");
    Serial.printf("image specs: (%d x %d), %d bpp, pixel type: %d\n", png.getWidth(), png.getHeight(), png.getBpp(), png.getPixelType());
    tft.startWrite();
    Serial.print("...after startWrite. ");
    uint32_t dt = millis();
    rc = png.decode(NULL, 1);
    Serial.print("rc= "); Serial.println(rc);
    Serial.print(millis() - dt); Serial.println("ms");
    tft.endWrite();

  }

}
void graphics() {
  int16_t rc = png.openFLASH((uint8_t *)Graphics, sizeof(Graphics), pngDraw);
  //int16_t rc = png.openFLASH((uint8_t *)Image, sizeof(Image), pngDraw);
  Serial.print("Top of loop.  rc = ");
  Serial.println(rc);

  if (rc == PNG_SUCCESS) {
    Serial.println("Successfully png file");
    Serial.printf("image specs: (%d x %d), %d bpp, pixel type: %d\n", png.getWidth(), png.getHeight(), png.getBpp(), png.getPixelType());
    tft.startWrite();
    Serial.print("...after startWrite. ");
    uint32_t dt = millis();
    rc = png.decode(NULL, 1);
    Serial.print("rc= "); Serial.println(rc);
    Serial.print(millis() - dt); Serial.println("ms");
    tft.endWrite();
    // png.close(); // not needed for memory->memory decode
  }
  tft.setCursor(40, 281, 2);
  tft.setTextSize(1);
  tft.print(scoreO);
  tft.setCursor(175, 281, 2);
  tft.setTextSize(1);
  tft.print(scoreX);
  tft.setCursor(165, 35, 4);
  tft.print(computer);
  tft.setCursor(40, 35, 4);
  tft.print(initials);
  /*
    tone(bzr, NC4, 200);delay(100);noTone(bzr);delay(10);
    tone(bzr, NE4, 200);delay(100);noTone(bzr);delay(10);
    tone(bzr, NG4, 200);delay(100);noTone(bzr);delay(10);
    tone(bzr, NC5, 200);delay(100);noTone(bzr);delay(10);
    tone(bzr, NE5, 200);delay(100);noTone(bzr);delay(10);
    tone(bzr, NG5, 200);delay(100);noTone(bzr);delay(10);
  */
}
void cat() {
  int16_t rc = png.openFLASH((uint8_t *)Cat, sizeof(Cat), pngDraw);
  //int16_t rc = png.openFLASH((uint8_t *)Image, sizeof(Image), pngDraw);
  Serial.print("Top of loop.  rc = ");
  Serial.println(rc);

  if (rc == PNG_SUCCESS) {
    Serial.println("Successfully png file");
    Serial.printf("image specs: (%d x %d), %d bpp, pixel type: %d\n", png.getWidth(), png.getHeight(), png.getBpp(), png.getPixelType());
    tft.startWrite();
    Serial.print("...after startWrite. ");
    uint32_t dt = millis();
    rc = png.decode(NULL, 1);
    Serial.print("rc= "); Serial.println(rc);
    Serial.print(millis() - dt); Serial.println("ms");
    tft.endWrite();
    // png.close(); // not needed for memory->memory decode
  }
}


void pngDraw(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WDITH];
  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
  tft.pushImage(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}
void setup1() {
  pinSet();
  Serial.begin(9600);


  EEPROM.begin(512);

}
void loop1() {

  if (digitalRead(bPB) == 0 && keepPlay == true) {
    playJig(tempo--);
    keepPlay = true;
  }
}
void PBchange() {
  if (digitalRead(yPB) == 0) {
    keepPlay = false;
  }
  else if (digitalRead(bPB) == 0) {
    keepPlay = true;
  }
}
