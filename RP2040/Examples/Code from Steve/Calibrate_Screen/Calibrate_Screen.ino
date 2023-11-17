/*
   Calibrate Screen development code
   by - Steve Kliewer
   This program is used to develop the touchCalibrate.h code
*/


#include <EEPROM.h>
#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip

#include "touchCalibrate.h" 
#include "PicoPins2.h"
TFT_eSPI mytft = TFT_eSPI();  // Invoke library

int rotation = 0;
int currentRotation;
int offset = 0;

void setup() {
  pinSet();
  Serial.begin(9600);
  while (!Serial);
  EEPROM.begin(512);

// Uncomment these lines to clear the entire calibration 
// structure for all 4 rotations
  // Serial.println("Before:");
  // int aa = 0;
  // for (int i= 0; i < 48; i++ ){
  //   aa = EEPROM.read(i);
  //   Serial.print(i);  Serial.print(" - \t"); Serial.println(aa);
  // }
  // for (int i= 0; i < 48; i++ ){
  //   EEPROM.write(i, i+100);
  // }
  // EEPROM.commit();
  // delay(1000);
  // Serial.println("After:");
  // for (int i= 0; i < 48; i++ ){
  //   aa = EEPROM.read(i);
  //   Serial.print(i);  Serial.print(" - \t"); Serial.println(aa);
  // }
  // return;
// end of clear entire calibration


  mytft.init();

//  for (int i = 0; i < 4; i++){
    int i=3;
    mytft.setRotation(i);
    mytft.fillScreen(TFT_BLUE);
    
    touch_calibrate(mytft);

    delay(1000);
    mytft.fillScreen(TFT_BLUE);
//  }

    // uint16_t junk = 222;
    // uint16_t anArray[5];
    // Serial.print("junk (uint16_t) size= "); Serial.println(sizeof(junk));
    // Serial.print("junk (uint16_t) value= "); Serial.println(junk);
    // junk = 203;
    // Serial.print("junk (uint16_t) value= "); Serial.println(junk);
    // Serial.print("rotation (int) size= "); Serial.println(sizeof(rotation));
    // Serial.print("uint16_t anArray[5] size= "); Serial.println(sizeof(anArray));
    // Serial.print("uint16_t array element size= "); Serial.println(sizeof(anArray[0]));
    // Serial.print("myTft size= "); Serial.println(sizeof(mytft));
    // Serial.println();
    // Serial.print("200 + junk size= "); Serial.println(sizeof(uint16_t(200 + junk)));
}

void loop() {
  uint16_t x, y;

  // See if there's any touch data for us
  if (mytft.getTouch(&x, &y))
  {
    while (mytft.getTouch(&x, &y)) {}  // holds the program here until you stop pushing the TFT screen
    Serial.print("X= "); Serial.print(x);Serial.print("   Y= "); Serial.println(y);

    currentRotation = mytft.getRotation();
    Serial.println(currentRotation);
    
    mytft.fillCircle(5+offset, 5+offset, 1, TFT_WHITE);
    // Draw a spot to show where touch was calculated to be
    mytft.fillCircle(x, y, 1, TFT_ORANGE);

    rotation ++;
    offset+=5;
    if (rotation > 3) 
      rotation = 0;

    //mytft.setRotation(rotation);
 
    delay(100);
  }
}

