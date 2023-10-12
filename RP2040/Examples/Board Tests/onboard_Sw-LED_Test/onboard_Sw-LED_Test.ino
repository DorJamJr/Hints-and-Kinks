/*
   Onboard Switch and LED Test
   by - J.Koerlin
   This program demonstrates the onboard Switch(GPIO24) and LED(GPIO25)

*/
#include"PicoPins2.h"

void setup() {
  pinSet();
  //Serial.begin(9600);
  //attachInterrupt(digitalPinToInterrupt(2), Encode, FALLING);

}

void loop() {
  if(digitalRead(oPB)==0)
  digitalWrite(oLED,HIGH);
  else
  digitalWrite(oLED,LOW);
 
}
