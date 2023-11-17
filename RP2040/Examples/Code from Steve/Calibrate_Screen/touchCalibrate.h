
/*
   Screen callibration data is stored in the EEPROM in a 5 cell array: uint16_t calData[5].
   This data type uses 10 addresses in the eeprom.  The eeprom is setup to store all 4
   orientations of the tft screen.  Additionally, each set of data is preceded by a 
   rotation ID that represents which orientation the screen is using right now. A total
   of 12 eeprom addresses are used for each orientation.
   
   If calibration data exists for the current rotation, calibration is skipped
   and the current data is used.  If it is not present, calibration is performed and stored.

   The value used for each Screen Rotation follows:
      Rotation ID    Rotation    EEPROM addresss
            200          0               0
            201          1              12
            202          2              24
            203          3              36

  2023-4-2  Updated so that this can go in the 'Arduino/libraries/TouchCalibrate' folder.
            Also, you are now required to pass your instance of
            the touch screen to the touch_calibrate(yourTFT) function

  2023-4-21 Rewrite supports the fact that EEPROM.get(...) and EEPROM.put(...)
            must use a locally declared calData array. 
            Also found what I think is an error in the library or hardware for 
            the tft.  Right after a calibration the touch screen is mirrored and
            a reset of the processor is necessary to force a reread of the 
            calData[] array from EEPROM.  See notes below too
*/

void calibrateNow(uint16_t newRotation, TFT_eSPI tft){
  // must use a locally declared calibration array for the EEPROM put/get functions
  uint16_t localData[5];

  tft.fillScreen(TFT_BLACK);
  tft.setCursor(20, 0);
  tft.setTextFont(2);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  tft.print("Touch corners to calibrate rotation: "); tft.print(newRotation);

  tft.setTextFont(1); 
  tft.println();

  tft.calibrateTouch(localData, TFT_MAGENTA, TFT_BLACK, 15);  // updates caldata with passed in or local

  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.fillScreen(TFT_BLACK);

  Serial.println("Writing Calibration Data"); 
  EEPROM.write(newRotation * 12, uint16_t(200 + newRotation));  // write truncates anything past 8 bits

  EEPROM.put((newRotation * 12) + 2, localData);
  if (EEPROM.commit()){
    Serial.println("EEPROM calibration successfully committed");
  }
  else
    Serial.println("ERROR! EEPROM commit failed");

  // The next line causes a complete restart of the code as if the hardware reset
  // button was pushed. This reads the EEPROM and loads that calibration data to the tft.
  // This was necessary because the touch screen seemed mirrored right after a calibration
  // until the unit was reset by the button
  watchdog_reboot(0,0,0);
}

// This overloaded function is to remind you that you must pass your instance of the 
// tft object for calibration to take place 
void touch_calibrate()
{
  delay(1000);
  TFT_eSPI wrongTFT = TFT_eSPI();
  wrongTFT.fillScreen(TFT_BLACK);
  wrongTFT.setCursor(20, 0);
  wrongTFT.setTextFont(2);
  wrongTFT.setTextSize(1);
  wrongTFT.setTextColor(TFT_WHITE, TFT_BLACK);
  wrongTFT.println("See Serial Port Output for error");
  Serial.println("You must pass in your instance of the TFT object. Like:  touch_calibrate(mytft);");
  delay(1000);
  // This exit(1) should prevent your code from working properly since you need to calibrate the screen
  exit(1);
}

void touch_calibrate(TFT_eSPI &tft)
{
  uint16_t calData[5]; 
  uint16_t x, y;
  uint16_t rotationID;  // This value is read from the EEPROM and if valid should be 200, 201, 202, 203
  uint16_t currentRotation; // This value is what the TFT screen rotation is currently
  bool repeatCalibration = false;  

  // Check whether to force a calibration for this rotation
  if (tft.getTouch(&x, &y))    repeatCalibration = true;
  while (tft.getTouch(&x, &y)) {}   // holds the program here until you stop pushing the TFT screen
  
  currentRotation = tft.getRotation();
  
  if (repeatCalibration) {
    Serial.println("Forcing Calibration");
    calibrateNow(currentRotation, tft);
  }
  else { // check for valid calibration data
    rotationID = EEPROM.read(currentRotation * 12);  // it takes 12 address cells for a full rotation set
    if (rotationID == (200 + currentRotation)){
      Serial.print("Good Calibration data for Rotation= "); Serial.println(currentRotation);

      EEPROM.get((currentRotation * 12) + 2, calData);
      for (int i=0; i< 5; i++){
        Serial.print(" USING- CalData[");Serial.print(i); Serial.print("]= "); Serial.println(calData[i]); 
      }
      Serial.println("To Recalibrate: Press Reset while touching the screen");
      tft.setTouch(calData);
    }
    else {
      Serial.print("Needs Calibrated for rotation= ");Serial.println(currentRotation);
      calibrateNow(currentRotation, tft);
    }
  }

  // These lines show the contents of the EEPROM's calibration data sets
  // uint16_t readID;
  // for (int k=0; k<4; k++) {
  //   readID = EEPROM.read(k * 12);
  //   Serial.print("address: "); Serial.print(k * 12);Serial.print("    = ");Serial.println(readID);
  //   EEPROM.get((k * 12) + 2, calData);
  //   for (int i=0; i< 5; i++){
  //     Serial.print(" CalData[");Serial.print(i); Serial.print("]= "); Serial.println(calData[i]); 
  //   }
  // Serial.println();
  // }

}
