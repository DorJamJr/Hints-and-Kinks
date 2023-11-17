/*
   TicTacToe1
   by - B.Jack
   This program...

*/
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();
#include"PicoPins2.h"
#include <SPI.h>
#include <EEPROM.h>
#include <PNGdec.h>
#include"touchCalibrate.h"

#define WHITE_SPOT
#define X_or_O
#include "FireWorks.h"
PNG png; // PNG decoder inatance

#define MAX_IMAGE_WDITH 180// Adjust for your images

int16_t xpos = 0;
int16_t ypos = 0;

uint16_t x, y;
int initiateTime;
bool Winner=false;
int touchX;
int touchY;
int touch;
int Xcoord;
int Ycoord;
int a; int b; int c; int d; int e; int f; int g; int h; int i;
int aX; int bX; int cX; int dX; int eX; int fX; int gX; int hX; int iX;
int aO; int bO; int cO; int dO; int eO; int fO; int gO; int hO; int iO;
int TlBxY=(0+240/2)/2;//top left box x
int TlBxX=(320/4+320/2)/2;//top left box y

int MlBxX=(0+240/2)/2;//middle left box x
int MlBxY=(320/4*2+320/4*3)/2;//middle left box y

int BlBxX=(0+240/2)/2;//bottom left box x
int BlBxY=(320/4*3+320/4*4)/2;//bottom left box y

int MtBxX=(240/3+240/3*2)/2;//middle top box x
int MtBxY=(320/4+320/2)/2;//middle top box y

int MmBxX=(240/3+240/3*2)/2;//middle middle box x
int MmBxY=(320/4*2+320/4*3)/2;//middle middle box y

int MbBxX=(240/3+240/3*2)/2;//middle bottom box x
int MbBxY=(320/4*3+320)/2;//middle bottom box y

int TrBxX=(240/4*2);//top right box x
int TrBxY=((320/4*3)+(240-240/3))/2;//top right box y

int MrBxX=(240+240/3*2)/2;//middle right box x
int MrBxY=(320/4*2+320/4*3)/2;//middle right box y

int BrBxX=(240+240/3*2)/2;//bottom right box
int BrBxY=(320/4*3+320)/2;//bottom right box

void setup() {
  pinSet();
  Serial.begin(9600);
  //attachInterrupt(digitalPinToInterrupt(2), Encode, FALLING);

  EEPROM.begin(512);
  
  tft.init();
  tft.setRotation(2);
  
  touch_calibrate();
  tft.fillScreen(TFT_BLACK); 
  
  //displays until screen is touched 
  tft.setCursor(0,7.5,2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println("Tic Tac Toe V1");
  tft.setCursor(240/2-75,320/2-5,4);
  tft.println("Touch screen");
  tft.println("                 to start");
  
 
}
void loop() {
 while(!tft.getTouch(&x, &y)){}//keeps Xturn from starting until screen is touched
  
 initiate();
 
 Xturn();
 
    
while(!tft.getTouch(&x, &y)){}//keeps Oturn from starting until screen is touched
 Oturn();
 
   
    
}

//#############################################################
void Xturn(){
   while(!tft.getTouch(&x, &y)){}
  
   while((x>=0&&x<=240/3&&y>=0&&y<=320/4&&a==1)&&(x>=0&&x<=240/3&&y>=320/2&&y<=320/4*3&&b==1)&&(x>=0&&x<=240/3&&y>=320/2&&y<=320/4*1&&b==1)&&
  (x>=0&&x<=240/3&&y<=320&&y>=320/4*3&&c==1)&&(x>=240/3&&x<=240/3*2&&y>=320/4&&y<=320/2&&d==1)&&(x>=240/3&&x<=240/3*2&&y>=
  320/2&&y<=320/4*3&&e==1)&&(x>240/3&&x<=240/3*2&&y<=320&&y>=320/4*3&&f==1)&&(x>=240/3*2&&x<=240&&y>=320/4&&y<=320/2&&g==1)&&
  (x>=240/3*2&&x<=240&&y>=320/2&&y<=320/4*3&&h==1)&&(x>240/3*2&&x<=240&&y<=320&&y>=320/4*3&&i==1)&&(x>=0&&x<=240&&y>=0&&y<=320/4)){
     
    }
    Touch("X ",TFT_BLUE,0);
 //Serial.print("End X"); 
  while(!tft.getTouch(&x, &y)&&(x>=0&&x<=240&&y>=0&&y<=240/4)){}
  delay(100);
 tft.drawLine(240-10,50,240-40,50,TFT_WHITE);
    tft.drawLine(10,50,40,50,TFT_BLUE);
    }


//#############################################################
void  Oturn(){
    while(!tft.getTouch(&x, &y)){}
    while((x>=0&&x<=240/3&&y>=320/4&&y<=320/2&&a==1)&&(x>=0&&x<=240/3&&y>=320/2&&y<=320/4*3&&b==1)&&
  (x>=0&&x<=240/3&&y<=320&&y>=320/4*3&&c==1)&&(x>=240/3&&x<=240/3*2&&y>=320/4&&y<=320/2&&d==1)&&(x>=240/3&&x<=240/3*2&&y>=
  320/2&&y<=320/4*3&&e==1)&&(x>240/3&&x<=240/3*2&&y<=320&&y>=320/4*3&&f==1)&&(x>=240/3*2&&x<=240&&y>=320/4&&y<=320/2&&g==1)&&
  (x>=240/3*2&&x<=240&&y>=320/2&&y<=320/4*3&&h==1)&&(x>240/3*2&&x<=240&&y<=320&&y>=320/4*3&&i==1)&&(x>=0&&x<=240&&y>=0&&y<=320/4)){
   
  }
    Touch("O",TFT_RED,1);
 while(!tft.getTouch(&x, &y)){}
 //Serial.print("End O"); 
 delay(100);
 tft.drawLine(240-10,50,240-40,50,TFT_RED);
    tft.drawLine(10,50,40,50,TFT_WHITE);
    }

//#############################################################
void Owin(){
  Serial.print("oWin");
  delay(500);
  //Pic();
  tft.fillScreen(TFT_BLACK);
  delay(200);
  //displays until screen is touched 
  tft.setCursor(0,7.5,2);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println("Restarting...");
  tft.setCursor(240/2-35,320/2-5,4);
  tft.println("O Won");
  
 delay(1000);
 initiateTime=0;
   a=0;  b=0;  c=0;  d=0;  e=0;  f=0;  g=0;  h=0;  i=0;
  aX=0;  bX=0;  cX=0;  dX=0;  eX=0;  fX=0;  gX=0;  hX=0;  iX=0;
  aO=0;  bO=0;  cO=0;  dO=0;  eO=0;  fO=0;  gO=0;  hO=0;  iO=0;
 initiate();
 
}
//#################################################################
void Xwin(){
  Serial.print("xWin");
  delay(500);
  //Pic();
  tft.fillScreen(TFT_BLACK);
  delay(200);
  //displays until screen is touched 
  tft.setCursor(0,7.5,2);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println("Restarting...");
  tft.setCursor(240/2-35,320/2-5,4);
  tft.println("X Won");
  
 delay(1000);
 initiateTime=0;
  a=0;  b=0;  c=0;  d=0;  e=0;  f=0;  g=0;  h=0;  i=0;
  aX=0;  bX=0;  cX=0;  dX=0;  eX=0;  fX=0;  gX=0;  hX=0;  iX=0;
  aO=0;  bO=0;  cO=0;  dO=0;  eO=0;  fO=0;  gO=0;  hO=0;  iO=0;
 initiate();

}
//#################################################################
void GameEnd(){
    delay(500);
  tft.fillScreen(TFT_BLACK);
  delay(200);
  tft.setCursor(0,7.5,2);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println("Restarting...");
  //displays until screen is touched 
  tft.setCursor(240/2-35,320/2-5,4);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println("Cat");
  
 delay(1000);
 initiateTime=0;
  a=0;  b=0;  c=0;  d=0;  e=0;  f=0;  g=0;  h=0;  i=0;
  aX=0;  bX=0;  cX=0;  dX=0;  eX=0;  fX=0;  gX=0;  hX=0;  iX=0;
  aO=0;  bO=0;  cO=0;  dO=0;  eO=0;  fO=0;  gO=0;  hO=0;  iO=0;
 initiate();
}
//#################################################################
//Sets up game graphics
void initiate(){
  //if screen is touched happens only once
//if(tft.getTouch(&x, &y)){
 if(initiateTime==0){
  
  //removes origional graphics 
  tft.fillScreen(TFT_BLACK);
  
  //lines for tic tac toe board
  tft.drawLine(240/3+1,320,240/3+1,320-240,TFT_WHITE);
  tft.drawLine(240/3,320,240/3,320-240,TFT_WHITE);
  tft.drawLine(240/3-1,320,240/3-1,320-240,TFT_WHITE);
  
  tft.drawLine(240/1.5+1,320,240/1.5+1,320-240,TFT_WHITE);
  tft.drawLine(240/1.5,320,240/1.5,320-240,TFT_WHITE);
  tft.drawLine(240/1.5-1,320,240/1.5-1,320-240,TFT_WHITE);

  tft.drawLine(0,240+1,240,240+1,TFT_WHITE);
  tft.drawLine(0,240,240,240,TFT_WHITE);
  tft.drawLine(0,240-1,240,240-1,TFT_WHITE);

  tft.drawLine(0,240/1.5+1,240,240/1.5+1,TFT_WHITE);
  tft.drawLine(0,240/1.5,240,240/1.5,TFT_WHITE);
  tft.drawLine(0,240/1.5-1,240,240/1.5-1,TFT_WHITE);
 
  //top bar
  tft.fillRect(0,0,30,320-240,TFT_BLUE);
  tft.fillRect(240-30,0,30,320-240,TFT_RED);
  tft.fillRectHGradient(0+30,0,240-60,320-240, TFT_BLUE, TFT_RED);

  //top bar border
  tft.drawRect(0,0,240,320-240,TFT_WHITE);
  tft.drawRect(1,1,240-3,320-240-3,TFT_BLACK);
  tft.drawRect(2,2,240-4,320-240-4,TFT_WHITE);

  //outside border
  tft.drawRect(0,0,240,320,TFT_WHITE);
  tft.drawRect(1,1,240-3,320-3,TFT_BLACK);
  tft.drawRect(2,2,240-4,320-4,TFT_WHITE);

  //Vs. in middle of top bar
  tft.setCursor(240/2-15, 25, 4);
  tft.setTextColor(TFT_WHITE, TFT_PURPLE); 
  tft.setTextSize(.5);
  tft.println("Vs.");

  //X on left of top bar
  tft.setCursor(20,25,4);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.setTextSize(1.2);
  tft.print("X");

  //O on right of top bar
  tft.setCursor(200,25,4);
  tft.setTextColor(TFT_WHITE, TFT_RED);
  tft.setTextSize(1.2);
  tft.print("O");
 
  initiateTime++;
  }
 }

void Touch(String XorO, int tftColor, int turn){
   uint16_t x, y;
   int Xcoord;
   int Ycoord;
   int a;
  // See if there's any touch data for us
  if (tft.getTouch(&x, &y))
  Serial.print(x);
  Serial.print(" ");
  Serial.println(y);
  if(turn==0){
    if (x>=0&&x<=240/3&&y>=320/4&&y<=320/2&&a==0){//
    Xcoord=TlBxY;
    Ycoord=TlBxX;
    a++;
    aX++;
  
  }
  if (x>=0&&x<=240/3&&y>=320/2&&y<=320/4*3&&b==0){//
    Xcoord=MlBxX;
    Ycoord=MlBxY;
    b++;
    bX++;
  
  }
   if (x>=0&&x<=240/3&&y<=320&&y>=320/4*3&&c==0){//
    Xcoord=BlBxX;
    Ycoord=BlBxY;
    c++;
    cX++;
    
   }
   if (x>=240/3&&x<=240/3*2&&y>=320/4&&y<=320/2&&d==0){//
    Xcoord=MtBxY+17;
    Ycoord=MtBxX;
    d++;
    dX++;
   }
  if (x>=240/3&&x<=240/3*2&&y>=320/2&&y<=320/4*3&&e==0){//
    Xcoord=MmBxX+17;
    Ycoord=MmBxY;
    e++;
    eX++;
  }
   if (x>240/3&&x<=240/3*2&&y<=320&&y>=320/4*3&&f==0){//
    Xcoord=MbBxX+17;
    Ycoord=MbBxY;
    f++;
    fX++;
   }
  
   if (x>=240/3*2&&x<=240&&y>=320/4&&y<=320/2&&g==0){//
    Xcoord=TrBxY+17;
    Ycoord=TrBxX;
    g++;
    gX++;
   }
  if (x>=240/3*2&&x<=240&&y>=320/2&&y<=320/4*3&&h==0){//
    Xcoord=MrBxX+17;
    Ycoord=MrBxY;
    h++;
    hX++;
  
  }
   if (x>240/3*2&&x<=240&&y<=320&&y>=320/4*3&&i==0){//
    Xcoord=BrBxX+17;
    Ycoord=BrBxY;
    i++;
    iX++;
   }
  if (x>=0&&x<=240&&y>=0&&y<=240/4){
  }
  
  }
  if (turn==1){
    if (x>=0&&x<=240/3&&y>=320/4&&y<=320/2&&a==0){//
    Xcoord=TlBxY;
    Ycoord=TlBxX;
    a++;
    aO++;
  
  }
  if (x>=0&&x<=240/3&&y>=320/2&&y<=320/4*3&&b==0){//
    Xcoord=MlBxX;
    Ycoord=MlBxY;
    b++;
    bO++;
  
  }
   if (x>=0&&x<=240/3&&y<=320&&y>=320/4*3&&c==0){//
    Xcoord=BlBxX;
    Ycoord=BlBxY;
    c++;
    cO++;
    
   }
   if (x>=240/3&&x<=240/3*2&&y>=320/4&&y<=320/2&&d==0){//
    Xcoord=MtBxY+17;
    Ycoord=MtBxX;
    d++;
    dO++;
   }
  if (x>=240/3&&x<=240/3*2&&y>=320/2&&y<=320/4*3&&e==0){//
    Xcoord=MmBxX+17;
    Ycoord=MmBxY;
    e++;
    eO++;
  }
   if (x>240/3&&x<=240/3*2&&y<=320&&y>=320/4*3&&f==0){//
    Xcoord=MbBxX+17;
    Ycoord=MbBxY;
    f++;
    fO++;
   }
  
   if (x>=240/3*2&&x<=240&&y>=320/4&&y<=320/2&&g==0){//
    Xcoord=TrBxY+17;
    Ycoord=TrBxX;
    g++;
    gO++;
   }
  if (x>=240/3*2&&x<=240&&y>=320/2&&y<=320/4*3&&h==0){//
    Xcoord=MrBxX+17;
    Ycoord=MrBxY;
    h++;
    hO++;
  
  }
   if (x>240/3*2&&x<=240&&y<=320&&y>=320/4*3&&i==0){//
    Xcoord=BrBxX+17;
    Ycoord=BrBxY;
    i++;
    iO++;
   }
  if (x>=0&&x<=240&&y>=0&&y<=240/4){
  }
  
  }
  {
    // Draw a white spot to show where touch was calculated to be
#ifdef X_or_O
    tft.setCursor(Xcoord-35,Ycoord-20,4);
    tft.setTextColor(tftColor, TFT_BLACK);
    tft.setTextSize(2);
    tft.print(XorO);
   #endif
  }
  Serial.print("variables= ");
  Serial.print(a);
  Serial.print(b);
  Serial.print(c);
  Serial.print(d);
  Serial.print(e);
  Serial.print(f);
  Serial.print(g);
  Serial.print(h);
  Serial.println(i);

  Serial.print(aX);
  Serial.print(bX);
  Serial.print(cX);
  Serial.print(dX);
  Serial.print(eX);
  Serial.print(fX);
  Serial.print(gX);
  Serial.print(hX);
  Serial.println(iX);

  Serial.print(aO);
  Serial.print(bO);
  Serial.print(cO);
  Serial.print(dO);
  Serial.print(eO);
  Serial.print(fO);
  Serial.print(gO);
  Serial.print(hO);
  Serial.println(iO);
   
 if(aX>=1&&bX>=1&&cX>=1)
{
  Xwin();
}
if(dX>=1&&eX>=1&&fX>=1)
{
  Xwin();
}
if(gX>=1&&hX>=1&&iX>=1)
{
  Xwin();
}
if(aX>=1&&eX>=1&&iX>=1)
{
  Xwin();
}
if(gX>=1&&eX>=1&&cX>=1)
{
  Xwin();
}
if(cX>=1&&fX>=1&&iX>=1)
{
  Xwin();
}
if(bX>=1&&eX>=1&&hX>=1)
{
  Xwin();
}
if(aX>=1&&dX>=1&&gX>=1)
{
  Xwin();
}
//########################
 if(aO>=1&&bO>=1&&cO>=1)
{
  Owin();
}
if(dO>=1&&eO>=1&&fO>=1)
{
  Owin();
}
if(gO>=1&&hO>=1&&iO>=1)
{
  Owin();
}
if(aO>=1&&eO>=1&&iO>=1)
{
  Owin();
}
if(gO>=1&&eO>=1&&cO>=1)
{
  Owin();
}
if(cO>=1&&fO>=1&&iO>=1)
{
  Owin();
}
if(bO>=1&&eO>=1&&hO>=1)
{
  Owin();
}
if(aO>=1&&dO>=1&&gO>=1)
{
  Owin();
}
if(a==1&&b==1&&c==1&&d==1&&e==1&&f==1&&g==1&&h==1&&i==1){
      Serial.print("game end");
      GameEnd();
 }
  }  

  //############################################################
void Pic()
{
int16_t rc = png.openFLASH((uint8_t *)FireWorks, sizeof(FireWorks), pngDraw);
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
