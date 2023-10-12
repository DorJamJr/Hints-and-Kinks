//Time Test
//Sets the Onboard Real Time Clock from Compile Time and Date
// J.Koerlin 2/26/23

#include "TimeLib.h"  // Functions to manipulate time
#include <time.h>     // Library Structures and functions to store and retreive time
#include <sys/time.h>  // Onboard Real Time Clock

#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
TFT_eSPI tft = TFT_eSPI();  // Invoke library

void setup() {
  //Serial.begin(9600);

  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_ORANGE);

  // convert the compile date and time into unix time, offset 1970
  parseTime();
}
void loop() {
  Display();
  delay(1000);
}

void Display()  // display time and date
{
  tft.setTextColor(TFT_BLACK, TFT_ORANGE);  
  tft.setTextSize(1);
  tft.setCursor(60, 50, 6);  // Set "cursor" to display Time and selects font 6 for clocks
  tft.print(hour());
  tft.print(':');
  tft.print(minute());
  tft.print(':');
  tft.print(second());

  tft.setCursor(45, 130, 6);  // Set "cursor" to display Date and selects font 6 for clocks
  tft.print(month());
  tft.print('-');
  tft.print(day());
  tft.print('-');
  tft.print(year());
  tft.setCursor(45, 180, 4);  // Set "cursor" to display Day of the Week and selects font 7
//tft.setFreeFont(FMB18);  
  switch (weekday()) {
    case 1:
      {
        tft.print("Sunday");
        break;
      }
    case 2:
      {
        tft.print("Monday");
        break;
      }
    case 3:
      {
        tft.print("Tuesday");
        break;
      }
    case 4:
      {
        tft.print("Wednesday");
        break;
      }
    case 5:
      {
        tft.print("Thursday");
        break;
      }
    case 6:
      {
        tft.print("Friday");
        break;
      }
    case 7:
      {
        tft.print("Saturday");
        break;
      }
  }

}

// Break the compile time and date( __DATE__ and __TIME__) into separate components
// Creates a unix or epoch time value and sets the onboard Real Time Clock
void parseTime()
{
  tmElements_t te;  //Time elements structure
  time_t unixTime; // a unix time stamp

  int myDay, myYear, myMonth, myMin, myHr, mySec;

  char mo[5];
  sscanf(__DATE__, "%s %d %d", mo, &myDay, &myYear);
  if (strcmp(mo, "Jan") == 0)myMonth = 1;
  if (strcmp(mo, "Feb") == 0)myMonth = 2;
  if (strcmp(mo, "Mar") == 0)myMonth = 3;
  if (strcmp(mo, "Apr") == 0)myMonth = 4;
  if (strcmp(mo, "May") == 0)myMonth = 5;
  if (strcmp(mo, "Jun") == 0)myMonth = 6;
  if (strcmp(mo, "Jul") == 0)myMonth = 7;
  if (strcmp(mo, "Aug") == 0)myMonth = 8;
  if (strcmp(mo, "Sep") == 0)myMonth = 9;
  if (strcmp(mo, "Oct") == 0)myMonth = 10;
  if (strcmp(mo, "Nov") == 0)myMonth = 11;
  if (strcmp(mo, "Dec") == 0)myMonth = 12;
  sscanf(__TIME__, "%d:%d:%d", &myHr, &myMin, &mySec);
  
//Load the individual time and date components into the time structure
  te.Second = mySec;
  te.Hour = myHr; // 24 hr time
  te.Minute = myMin;
  te.Day = myDay;
  te.Month = myMonth;
  te.Year = myYear - 1970; //offset from 1970

// convert the te time structure into a Unix Time Value
  unixTime =  makeTime(te);

  setTime(unixTime); //set the current time in the RTC
}
