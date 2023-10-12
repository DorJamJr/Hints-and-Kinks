/* Released into the public domain */
/* Earle F. Philhower, III <earlephilhower@yahoo.com> */

#include <Keyboard.h>
#include "PicoPins2.h"
#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
TFT_eSPI tft = TFT_eSPI();  // Invoke library

char keys[100] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890.,;:!?/\+=@#$%^&*()~`<>";


void setup() {
  //Serial.begin(115200);
  pinSet();
  attachInterrupt(digitalPinToInterrupt(encA), Encode, FALLING);

  Keyboard.begin();

  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_ORANGE);
    tft.setTextColor(TFT_BLACK, TFT_ORANGE);  tft.setTextSize(1);
  tft.setCursor(10, 20, 4);  // Set "cursor" at top left corner of display (30,40) and select font 4
  tft.println("Rotate the Encoder to");  // Print Encoder Count keys
  tft.println("   Select Character");  // Print Encoder Count keys
  tft.setCursor(10, 180, 4);  // Set "cursor" at top left corner of display (30,40) and select font 4
  tft.println("Press the Blue Button");  // Print Encoder Count keys
  tft.println("   to type key");  // Print Encoder Count keys

}

void loop() {
  if (count > 84)count = 0;
  if (count < 0)count = 84;
    tft.setTextColor(TFT_BLACK, TFT_ORANGE);  tft.setTextSize(2);
  tft.setCursor(150, 100, 4);  // Set "cursor" location and select font 4
  tft.print(keys[count]);  // Print Encoder Count keys
  tft.print("  ");  // add space to cover last character

  if (digitalRead(bPB) == 0) {
    Keyboard.print(keys[count]);
    while (digitalRead(bPB) == 0) {}
    delay(50);
  }
}
