#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
TFT_eSPI tft = TFT_eSPI();  // Invoke library

/*
   The value of CalTest is written to the EEPROM at address 1.
   If the value test number is present, the Calibration is skipped
   The value used for each application is related to the Screen Rotation as follows:
   CalTest    Rotation Value
     200          0
     201          1
     202          2
     203          3
   In this way calibration will be run the application is loaded on a new board or
   if the orentation changes from a previous application.

   Please set the CalTest value for your application below
*/
bool REPEAT_CAL = false;  //true will force a recalibration with each running of the program.


void touch_calibrate()
{
  const int CalTest = 203;  // if the board is calibrated this should be written into address 1 on the EEPROM
  uint16_t calData[5];
  int CalAdd = 2;
  int  calVal = 0;
  uint16_t x, y;

  calVal = EEPROM.read(1);
  if (tft.getTouch(&x, &y))REPEAT_CAL = true;
  while (tft.getTouch(&x, &y)) {}
  if (REPEAT_CAL || calVal != CalTest)
  {
    // data not valid so recalibrate
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 0);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.println("Touch corners as indicated");

    tft.setTextFont(1);
    tft.println();

    if (REPEAT_CAL) {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.println("Set REPEAT_CAL to false to stop this running again!");
    }

    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibration complete!");

    // store data
    Serial.println("Writing Cal Data");
    EEPROM.write(1, CalTest);
    EEPROM.put(CalAdd, calData);
    if (EEPROM.commit()) {
      Serial.println("EEPROM successfully committed");
    } else {
      Serial.println("ERROR! EEPROM commit failed");
    }

  }
  else
  {
    // calibration data valid
    EEPROM.get(CalAdd, calData);
    tft.setTouch(calData);
  }
  tft.fillScreen(TFT_BLACK);

}
//------------------------------------------------------------------------------------------
