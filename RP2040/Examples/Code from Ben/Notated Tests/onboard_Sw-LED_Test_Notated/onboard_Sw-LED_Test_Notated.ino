/*
   Onboard Switch and LED Test
   by - J.Koerlin
   This program demonstrates the onboard Switch(GPIO24) and LED(GPIO25)

*/
#include"PicoPins2.h"

void setup() {
  pinSet();// Sets up the pinSet variable
  //Serial.begin(9600);
  //attachInterrupt(digitalPinToInterrupt(2), Encode, FALLING);

}

void loop() {
  if(digitalRead(oPB)==0)// initalizes the loop if the reading from oPB is 0
  digitalWrite(oLED,HIGH);// turns the LED on
  else
  digitalWrite(oLED,LOW);// if the loop is not run turn the LED off
 
}
