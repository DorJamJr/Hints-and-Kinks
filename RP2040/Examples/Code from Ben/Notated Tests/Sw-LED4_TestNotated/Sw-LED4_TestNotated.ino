/*
   Sw-LED4_Test
   by - J.Koerlin
   This program...

*/
#include"PicoPins2.h"

void setup() {
  pinSet();//Sets up the pinSet variable
  //Serial.begin(9600);
  //attachInterrupt(digitalPinToInterrupt(2), Encode, FALLING);

}

void loop() {
  digitalWrite(rLED,!digitalRead(rPB));// If rPB is not read turn rLED on
  digitalWrite(yLED,!digitalRead(yPB));// If yPB is not read turn yLED on
  digitalWrite(gLED,!digitalRead(gPB));// If gPB is not read turn gLED on
  digitalWrite(bLED,!digitalRead(bPB));// If bPB is not read turn bLED on
 
}
