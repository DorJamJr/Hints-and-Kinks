/*
   Tank
   by - J.Koerlin
   This program is played against the computer.
   The left Tank is driven up and down the screen while firing at the Users Tank.
   The User Tank is moved with the Joystick and aimed with the encoder knob.
   Fire the tank shell with the blue button*/
#include <SPI.h>
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();
#include"PicoPins2.h"
#include <SPI.h>
#include <EEPROM.h>
#include <PNGdec.h>
#include"touchCalibrate.h"
#include "Graphics.h"
TFT_eSprite img = TFT_eSprite(&tft);  // Create Sprite object "img" with pointer to "tft" object
#define TFT_GREY 0x5AEB // New colour
#define MAX_IMAGE_WDITH 240 // Adjust for your images

int16_t xpos = 0;
int16_t ypos = 0;

uint16_t x, y;

PNG png; // PNG decoder inatance
int color = TFT_GREY;

//const int CalTest = 101;  // if the board is calibrated this should be written into the 0 reg
int eTanks = 5, Tanks = 5;
int ctr;      // Center Value of the Joystick
int tankX = 270;  // Tank x position
int tankY = 95;  // Tank y position

int tankSpeed = 150; // factor to determine how fast the paddle moves.  (Note - Bigger is slower)
float gunAngle = 0;
long tankTimerStart, tankRate = 40; // These time the update of the tank and speed of the tank (smaller tankRate = faster)
long shellTimerStart, shellRate = 10;  // timer variables for the shells.  Larger shellRate = slower shell
long eShellTimerStart, eShellRate = 10;// timer variables for the enemy shells
int  eFireMin = 2000, eFireMax = 8000;  // these govern how often the enemy shells are fired
long eFire = random(eFireMin, eFireMax);  // random milisecond timing

int eTankX, dir = -1; // Enemy Tank x position
int eTankY = 100; // Enemy Tank y position
float eGunAngle = 3.15;

int shellX, shellY, oldShellX, oldShellY, shellLen = 8;
int killRad = 13;
float shellAng, shellStep = 10;
bool shellFired = false;


int eShellX, eShellY, eOldShellX, eOldShellY, eShellLen = 8;
int eKillRad = 13;
float eShellAng, eShellStep = 10;
bool eShellFired = false, win = false;

void setup() {
  pinSet();
  Serial.begin(9600);
  ctr = analogRead(yJS);
  Serial.println(ctr);
  //while (!Serial);

  tft.init();
  tft.setRotation(3);
  
  EEPROM.begin(512);
   
  touch_calibrate();
  
  attachInterrupt(digitalPinToInterrupt(2), Encode, FALLING);
  
  
  
  tft.fillScreen(color);
  tankTimerStart = millis();
  eShellTimerStart = millis();
 
  count = 315;
  //drawTank(tankX, tankY, gunAngle);
  tft.setTextColor(TFT_BLACK, color);  tft.setTextSize(1);
  tft.setCursor(50, 10, 4);  // Set "cursor" at top left corner of display (30,40) and select font 4
  tft.println("Tank-Axis Vs. Allies ");  // Print Encoder Count
  tft.setCursor(0, 50, 2);  // Set "cursor" at top left corner of display (30,40) and select font 4
  tft.println("  Move with Joystick");  // Print Encoder Count
  tft.println("  Aim by Touching Enemy Tank  ");  // Print Encoder Count
  tft.println("  Fire by Touching Your Tank");
  tft.println("  Press Screen to Start");  // Print Encoder Count
   while(!tft.getTouch(&x, &y)){}
   tft.setRotation(2);
   graphics(); 
   tft.setRotation(3);
}

void loop() {
  moveTank();
  turnTurret();
  shellControl();
  moveEnemy();
  checkForDamage();
  drawScreen();
  if (win)
  {
    eTanks = 5;
    Tanks = 5;
    tankTimerStart = millis();
    eShellTimerStart = millis();
    win = false;
    delay(1500);
    tft.setRotation(2);
    graphics(); 
    tft.setRotation(3);

  }
}
void moveTank() {
  if (millis() - tankTimerStart > tankRate) // the timer determines how oftern the tank moves
  {
    tankTimerStart = millis();      // the this controlls its speed
    tankY = tankY - ((analogRead(yJS) - ctr) / tankSpeed);  // tank speed determines how big a step the tank makes
    if (tankY > 190)  // don't let the tank go out of bounds
      tankY = 190;
    if (tankY < 0 )
      tankY = 0;
  }
}
void turnTurret() {
  if (count < -12)count = -12; // Limit the angle of the turret
  if (count > 12)count = 12;
  gunAngle = atan(float(x - y) / 250);  
  //gunAngle = count;  // we use count to controll the gun angle
  //gunAngle /= 10;
}
void shellControl() {
  // User Tank Fire
  if ((tft.getTouch(&x, &y))&& !shellFired)  //Initiate a Firing with the blue button
  { // but only if a shell is not in play
    shellX = tankX + 25;  // the location to start the shell
    shellY = tankY + 25;
    oldShellY = shellY;       // the old location. used to erace the previous location
    oldShellX = shellX;
    shellAng = gunAngle;     // the angle of firing
    shellFired = true;     // shell is active / moving
    shellTimerStart = millis(); // start the timer to move the shell
  }
  if (shellFired && (millis() - shellTimerStart > shellRate))  // Move the shell through the space based on time
  {
    shellTimerStart = millis();  // reset the timer
    // draw/erase the old location with the background color
    tft.drawLine(oldShellX, oldShellY, oldShellX - (shellLen * cos(shellAng)), oldShellY - (shellLen * sin(shellAng)), color);  // erace the old shell location
    shellX = shellX - (shellStep * cos(shellAng));   // calculate the new shell location
    shellY = shellY - (shellStep * sin(shellAng));
    tft.drawLine(shellX, shellY, shellX - (shellLen * cos(shellAng)), shellY - (shellLen * sin(shellAng)), TFT_BLACK);// draw the shell
    oldShellY = shellY;
    oldShellX = shellX;
  }
  if (shellX < 0)  // we reached the other side cancel the shell
  {
    shellFired = false;
  }
  //****************  Enemy Shell  **************
  if (millis() - eShellTimerStart > eFire && !eShellFired)  // Firing generated on random time
  {
    eShellX = eTankX + 25;
    eShellY = eTankY + 25;
    eOldShellY = eShellY;
    eOldShellX = eShellY;
    eShellAng = eGunAngle;
    eShellFired = true;
    eShellTimerStart = millis();
  }

  if (eShellFired && (millis() - eShellTimerStart > eShellRate)) // Enemy shell moves through the space
  {
    eShellTimerStart = millis();
    tft.drawLine(eOldShellX, eOldShellY, eOldShellX + (eShellLen * cos(eShellAng)), eOldShellY + (eShellLen * sin(eShellAng)), color);
    eShellX = eShellX + (eShellStep * cos(eShellAng));
    eShellY = eShellY + (eShellStep * sin(eShellAng));
    tft.drawLine(eShellX, eShellY, eShellX + (eShellLen * cos(eShellAng)), eShellY + (eShellLen * sin(eShellAng)), TFT_BLACK);
    eOldShellY = eShellY;
    eOldShellX = eShellX;
  }
  if (eShellX > 320)  // Enemy shell reaches the far edge of the screen
  {
    eShellFired = false;
    eFire = random(eFireMin, eFireMax);  // reset the time for the next shell
  }
}
void moveEnemy() {
  eTankX = 20;
  eTankY += 2 * dir;
  if (eTankY < 3)dir = 1;
  if (eTankY > 200)dir = -1;
  eGunAngle = atan(float(tankY - eTankY) / 250);  // the gun is always pointed at our tank
  /*  So why don't the enemy shells hit with greater accuracy?
     Well this is kinda lucky accident.  While I calculate the angle acurately the shell moves
     along a path which is made up of discrete integer increments.  This means that not all
     angles are available to it.  A bit like moving on a chess board.  If we move 8 squares forward
     then one square left, this will be a certain angle.  2 squares left would be a different angle.
     this could be corrected by using floating point variables for the movement,
     but I kinda like my enemy to have poor aim.  I live longer ;-)
  */
}
void checkForDamage() {

  // did we hit the Computer Tank?
  if (abs((eTankX + 25) - shellX) < killRad && abs((eTankY + 25) - shellY) < killRad)
  {
    for (int x = 1; x < 30; x += 3)  // make an explosion
    {
      tft.fillCircle(eTankX + 25, eTankY + 25, x, TFT_ORANGE);
      delay(20);
    }
    tft.fillCircle(eTankX + 25, eTankY + 25, 30, color);  // erace the explosion
    // erase the shell
    tft.drawLine(oldShellX, oldShellY, oldShellX - (shellLen * cos(shellAng)), oldShellY - (shellLen * sin(shellAng)), color);// erace the shell
    Tanks--;
    shellX = 0;  // move the shell so it won't blow up again.
    shellY = 0;
    shellFired = false;  //done with that shell
    if (Tanks < 1)  // have we run out of tanks?
    {
      tft.setCursor(130, 100, 4);
      tft.print("Allies WIN!!");  // Print the winner
      win = true;
    }
    else
    {
      for (int x = 0; x < 100; x += 5)  // bring in a new tank
      {
        eTankY = x;
        draweTank(eTankX, eTankY, eGunAngle);
        delay(20);
      }
      dir = 1;
    }

  }
  //Did the Enemy Hit the User Tank?
  if (abs((tankX + 25) - eShellX) < killRad && abs((tankY + 25) - eShellY) < eKillRad)
  {
    for (int x = 1; x < 30; x += 3)
    {
      tft.fillCircle(tankX + 25, tankY + 25, x, TFT_ORANGE);
      delay(20);
    }
    //    delay(1500);
    tft.fillCircle(tankX + 25, tankY + 25, 30, color);
    tft.drawLine(eOldShellX, eOldShellY, eOldShellX - (shellLen * cos(shellAng)), eOldShellY - (shellLen * sin(shellAng)), color);
    eTanks--;
    eShellX = 0;
    eShellY = 0;
    eShellFired = false;
    if (eTanks < 1)
    {
      tft.setCursor(130, 100, 4);  //
      tft.print("Axis WINs!!");  // Print the winner
      win = true;
    }
    else
    {
      for (int x = 0; x < 100; x += 5)  // bring in a new tank
      {
        tankY = x;
        drawTank(tankX, tankY, gunAngle);
        delay(20);
      }

    }

  }
}
void drawScreen() {
  drawTank(tankX, tankY, gunAngle);  //Draw the User Tank
  draweTank(eTankX, eTankY, eGunAngle);  //Draw the Enemy Tank

  //Put up the score
  tft.setTextColor(TFT_BLACK, color);  tft.setTextSize(1);
  tft.setCursor(130, 10, 4);
  tft.print(Tanks);  // Print Our Tanks
  tft.print("   ");  //
  tft.setCursor(200, 10, 4);
  tft.print(eTanks);  // Print Enemy Tanks
  tft.print("   ");  // this pads any of the numbers with background color

}
// #########################################################################
// Create sprite, plot graphics in it, plot to screen, then delete sprite
// #########################################################################
void drawTank(int x, int y, float tAng)  //User Tank
{
  int w = 50, h = 50, cx = w / 2, cy = h / 2;
  int tw = 23, th = 30, tr = 7, tl = cx - 3;
  // Create an 8 bit sprite 70x 80 pixels (uses 5600 bytes of RAM)
  //img.setColorDepth(8);
  img.createSprite(w, h);
  img.fillSprite(color);
  // draw tank treads
  img.drawRect(cx - (tw / 2) - 2, cy - (th / 2) - 2, tw + 4, th + 4, TFT_BLACK);
  img.fillRect(cx - (tw / 2) + 8, cy - (th / 2) - 3, tw - 16, th + 8, color);
  // Draw Tank
  img.drawRect(cx - (tw / 2), cy - (th / 2), tw, th, TFT_BLACK);
  // draw Turret
  img.drawCircle(cx, cy, tr, TFT_BLACK);
  // Draw gun
  img.drawLine(cx - (tr * cos(tAng)), cy - (tr * sin(tAng)), cx - (tl * cos(tAng)), cy - (tl * sin(tAng)), TFT_BLACK);

  // Push sprite to TFT screen CGRAM at coordinate x,y (top left corner)
  // Specify what colour is to be treated as transparent.
  img.pushSprite(x, y, TFT_TRANSPARENT);

  // Delete it to free memory
  img.deleteSprite();

}
void draweTank(int x, int y, float tAng)  // Enemy Tank
{
  int w = 50, h = 50, cx = w / 2, cy = h / 2;
  int tw = 23, th = 30, tr = 7, tl = cx - 3;
  // Create an 8 bit sprite 70x 80 pixels (uses 5600 bytes of RAM)
  //img.setColorDepth(8);
  img.createSprite(w, h);
  img.fillSprite(color);
  img.drawRect(cx - (tw / 2) - 2, cy - (th / 2) - 2, tw + 4, th + 4, TFT_BLACK);
  img.fillRect(cx - (tw / 2) + 8, cy - (th / 2) - 3, tw - 16, th + 8, color);
  img.drawRect(cx - (tw / 2), cy - (th / 2), tw, th, TFT_BLACK);
  img.drawCircle(cx, cy, tr, TFT_BLACK);
  img.drawLine(cx + (tr * cos(tAng)), cy + (tr * sin(tAng)), cx + (tl * cos(tAng)), cy + (tl * sin(tAng)), TFT_BLACK);
  img.pushSprite(x, y, TFT_TRANSPARENT);
  img.deleteSprite();
}
  //############################################################
void graphics()
{
int16_t rc = png.openFLASH((uint8_t *)Tank_Graphics, sizeof(Tank_Graphics), pngDraw);
  Serial.print("Top of loop.  rc = ");
  Serial.println(rc);

  if (rc == PNG_SUCCESS) {
    Serial.println("Successfully png file");
    Serial.printf("image specs: (%d x %d), %d bpp, pixel type: %d\n", png.getWidth(), png.getHeight(), png.getBpp(), png.getPixelType());
    tft.startWrite();
    Serial.print("...after startWrite. ");
    uint32_t dt = millis();
    rc = png.decode(NULL, 1);
    Serial.print("rc= ");Serial.println(rc);
    Serial.print(millis() - dt); Serial.println("ms");
    tft.endWrite();
    // png.close(); // not needed for memory->memory decode
  }
}
//####################################################################
  
void pngDraw(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WDITH];
  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
  tft.pushImage(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}
