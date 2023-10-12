/*
   Switch Test
   by - J.Koerlin
   This program...

*/
#include"PicoPins2.h"

void setup() {
  pinSet();
  //Serial.begin(9600);
  //attachInterrupt(digitalPinToInterrupt(2), Encode, FALLING);

}

void loop() {
  digitalWrite(rLED,!digitalRead(rPB));
  digitalWrite(yLED,!digitalRead(yPB));
  digitalWrite(gLED,!digitalRead(gPB));
  digitalWrite(bLED,!digitalRead(bPB));
 
}
