/*
   Dial Lock
   by - J.Koerlin
   This program simulates a Combination Lock
*/
#include"NanoPins.h"

#include "U8glib.h"  // load the Ug8 graphics library
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);  // The Nano Project Display
int blank = -1;
int Code[4] = {12, 24, 8};
int Guess[4] = {blank, blank, blank};
int ptr = 0;
int tumblers = 3;
int Open = false;






const uint8_t Locked[] U8G_PROGMEM = {
  0b00000000,  0b00000000,
  0b00000000,  0b00000000,
  0b00000000, 0b00000000,
  0b11110000, 0b00000001,
  0b11111100, 0b00000111,
  0b00001100, 0b00000110,
  0b00000110, 0b00001100,
  0b00000110, 0b00001100,
  0b00000110, 0b00001100,
  0b11111111, 0b00011111,
  0b11111111, 0b00011111,
  0b10111111, 0b00011111,
  0b10111111, 0b00011111,
  0b11111111, 0b00011111,
  0b11111111, 0b00011111,
  0b11111111, 0b00011111,
  0b11111111, 0b00011111,
};

const uint8_t unLocked[] U8G_PROGMEM = {
  0b11110000,  0b00000001,
  0b11111100, 0b00000111,
  0b00001100, 0b00000110,
  0b00000110, 0b00001100,
  0b00000110, 0b00001100,
  0b00000110, 0b00000000,
  0b00000110, 0b00000000,
  0b00000110, 0b00000000,
  0b00000110, 0b00000000,
  0b11111111, 0b00011111,
  0b11111111, 0b00011111,
  0b10111111, 0b00011111,
  0b10111111, 0b00011111,
  0b11111111, 0b00011111,
  0b11111111, 0b00011111,
  0b11111111, 0b00011111,
  0b11111111, 0b00011111
};
void setup() {
  // put your setup code here, to run once:
  pinSet();

  //Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(2), Encode, FALLING);  // uses the encoder to adjust 'count' variable

  u8g.setColorIndex(1);         // pixel on
  //u8g.setFont(u8g_font_unifont);  // the font used to write to the display
  u8g.setFont(u8g_font_osb21);   // bigger Font

}


void loop() {
  // put your main code here, to run repeatedly:
  Spin();
  Display();
}
void Spin()
{
  if (count > 36)
    count = 0;
  if (count < 0)
    count = 36;
  if (digitalRead(bPB) == 0 && ptr < 3)
  {
    Guess[ptr] = count;
    ptr++;
    if (ptr == 3)
      Test();
    delay(50);
    while (digitalRead(bPB) == 0) {}
    delay(50);
  }
  if (digitalRead(rPB) == 0)
  {
    Guess[0] = blank;
    Guess[1] = blank;
    Guess[2] = blank;
    ptr = 0;
    count = 0;
    Open = false;
  }
}
void Test()
{
  Open = true;
  for (int x = 0; x < 3; x++)
  {
    if (Guess[x] != Code[x])
      Open = false;
  }
  if (Open)
  {
    tone(bzr, 523, 50);
    delay(50);
    tone(bzr, 784, 50);

  }
  else
  {
    tone(bzr, 330, 90);
    delay(50);
    tone(bzr, 220, 90);
  }
}
void Draw()
{
  // Your Graphical Display Commands go here  ==> See Examples Below
  u8g.setPrintPos(50, 28);  // set the position the text will start.
  u8g.print(count); // print count
  u8g.setPrintPos(10, 60);  // set the position the text will start.
  if (Guess[0] == blank)
    u8g.print("__");
  else
    u8g.print(Guess[0]);
  u8g.print(" ");
  u8g.setPrintPos(49, 60);  // set the position the text will start.
  if (Guess[1] == blank)
    u8g.print("__");
  else
    u8g.print(Guess[1]);
  u8g.print(" ");
  u8g.setPrintPos(88, 60);  // set the position the text will start.
  if (Guess[2] == blank)
    u8g.print("__");
  else
    u8g.print(Guess[2]);

  u8g.setPrintPos(100, 30);  // set the position the text will start.
  if (Open)
  //  u8g.print("O");
   u8g.drawXBMP(90, 5, 16, 17, unLocked);
  else
   // u8g.print("L");
  u8g.drawXBMP(90, 5, 16, 17, Locked);


}
/*
  // These are Examples of what can be drawn on the OLED Graphical Display
  u8g.setPrintPos(10, 30);  // set the position the text will start.
  u8g.print("count:"); // print "count" label
  //u8g.setPrintPos(55, 30);  //if this is not included the text continues to print on the same line
  u8g.print(count); // print count
  u8g.drawVLine(3, 5, 50); // (x1,y1,height) Point and height (extending down)
  u8g.drawLine(15, 5, 110, 20); // (x1,y1,x2,y2) Point to Point
  u8g.drawDisc(90, 45, 10);     // (x,y,Radius)
  u8g.drawCircle(90, 45, 15);     // (x,y,Radius)
  u8g.drawFrame(10, 35, 50, 20); // (x,y,width,height) point is upper left
  u8g.drawBox(40, 38, 13, 13); // (x,y,width,height) point is upper left
  u8g.drawTriangle(60, 1, 120, 1, 120, 15); // (x1,y1,x2,y2,x3,y3)Triangle between 3 points
  u8g.drawPixel(25, 45); // (x,y) Point
*/


// Picture Loop for writing to the OLED Display
void Display()
{
  // ***************  Write to the Display ******************
  u8g.firstPage();  //Start the Picture Loop
  do {
    Draw();
  } while ( u8g.nextPage() );  // End the picture loop
  // ***************  Done Writing to the Display ***********
}
