/* Earle F. Philhower, III <earlephilhower@yahoo.com> */
/* Released to the public domain */


#include <Mouse.h>
#include <SPI.h>
#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
TFT_eSPI tft = TFT_eSPI();  // Invoke library
int bPB = 11;
void setup() {
//  Serial.begin(115200);
pinMode(bPB,INPUT_PULLUP);
  Mouse.begin();
//  delay(5000);
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_ORANGE);
  // Set the font colour to be white with a black background, set text size multiplier to 1
  tft.setTextColor(TFT_BLACK, TFT_ORANGE);  tft.setTextSize(1);
  // We can now plot text on screen using the "print" class
  tft.setCursor(10, 30, 4);  // Set "cursor" 
  tft.println("Press the Boot Button");
  tft.setCursor(10, 60, 4);  // Set "cursor" 
  tft.println("to Move the Mouse");

}

void loop() {
  if (digitalRead(bPB)==0) {  //this is the Boot button on the pico.
  tft.setCursor(50, 150, 4);  // Set "cursor" 
  tft.println("BARREL ROLL!!!");

    float r = 100;
    float ox = 0.0;
    float oy = 0.0;
    for (float a = 0; a < 2.0 * 3.14159; a += 0.1) {
      float ax = r * cos(a);
      float ay = r * sin(a);
      float dx = ax - ox;
      float dy = ay - oy;
      Mouse.move(dx, dy, 0);
      ox = ax;
      oy = ay;
      delay(10);
    }
    while (digitalRead(bPB)==0) {}
    delay(50);
  }
}
