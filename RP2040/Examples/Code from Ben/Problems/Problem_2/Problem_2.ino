/*
   Template
   by - J.Koerlin
   This program...

*/

int myCount;
#include"PicoPins2.h"

void setup() {
  pinSet();
  //Serial.begin(9600);
  //attachInterrupt(digitalPinToInterrupt(2), Encode, FALLING);

}
//code increases myCount by one when the red push putton is pressed
//and decreases myCount by one when the blue push button is pressed
void loop() {
  if (digitalRead(rPB) == 0) {
    myCount = myCount + 1;

    //******************************************************************************
    //Ben, adding these lines will control the push button better.
    // it will only run once per button press
    delay(50);  // let the switch bounce settle (pressed)
    while (digitalRead(rPB) == 0) {}
    delay(50);  // let the switch bounce settle  (released)
    //******************************************************************************

  }
  if (digitalRead(bPB) == 0) {
    myCount = myCount - 1;
  }
  Serial.println(myCount);//prints myCount
  //  delay(100);//debounce
}
