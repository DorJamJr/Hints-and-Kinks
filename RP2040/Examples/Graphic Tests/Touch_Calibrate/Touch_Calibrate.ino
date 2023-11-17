#include <EEPROM.h>
#include <SPI.h>
#include"touchCalibrate.h"


void setup() {

  EEPROM.begin(512);  // Set aside 512 bytes of Flash as EEPROM
  tft.init();

  touch_calibrate();  //Calibrate and save the data to the EEPROM
}

void loop() {


}
