/*
   PotBzrTest
   by - J.Koerlin
   This program...

*/
#include"PicoPins2.h"
int potVal = 0;
void setup() {
  pinSet();// sets up the pinSet variable
  Serial.begin(115200);// sets up the serial monitor at a baud rate of 115200
  //attachInterrupt(digitalPinToInterrupt(2), Encode, FALLING);
  tone(bzr, 1500, 300);// Makes the buzzer sound once when the code first initalizes
}

void loop() {
  potVal = analogRead(pot);// Sets potVal as what is read at the pot
  if (digitalRead(rPB) == 0)// only play a tone if the red button pressed
    tone(bzr, potVal*2, 10);// Makes th busser sound with different sounds based on 
                              // what is read at the pot
  Serial.print(potVal);// prints potVal in 10bit
  Serial.print(" ");// prints a space
  potVal = map(potVal, 0, 1023, 0, 255);  // scale the 10 bit (0-1023) analogRead value to 
                                       //the 8 bit(0-255)analogWrite needs.
  Serial.println(potVal);// prints potVal in 8bit and ends the line   
  analogWrite(rLED, potVal);// Brightens or dims rLED with PMW based on potVal
  analogWrite(yLED, potVal);// Brightens or dims yLED with PMW based on potVal
  analogWrite(gLED, potVal);// Brightens or dims gLED with PMW based on potVal
  analogWrite(bLED, potVal);// Brightens or dims bLED with PMW based on potVal

}
