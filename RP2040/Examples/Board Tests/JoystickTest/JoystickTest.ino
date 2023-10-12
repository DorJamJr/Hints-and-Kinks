/*
   Joystick Test
   by - J.Koerlin
   This program reads the X & Y values from the Joystick anp prints to the Serial Monitor

*/
#include"PicoPins2.h"

int xJSval = 0, yJSval = 0;

void setup() {
  pinSet();
  Serial.begin(115200);
  //attachInterrupt(digitalPinToInterrupt(2), Encode, FALLING);
  tone(bzr, 1500, 300);
}

void loop() {
xJSval = analogRead(xJS);
yJSval = analogRead(yJS);
  
  Serial.print(xJSval);
  Serial.print(" ");
  Serial.println(yJSval);
  delay(300);
}
