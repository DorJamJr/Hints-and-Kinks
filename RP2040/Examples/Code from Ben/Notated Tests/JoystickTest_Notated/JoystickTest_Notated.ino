/*
   Joystick Test
   by - J.Koerlin
   This program reads the X & Y values from the Joystick anp prints to the Serial Monitor

*/
#include"PicoPins2.h"

int xJSval = 0, yJSval = 0;

void setup() {
  pinSet();// Sets up the pinSet variable
  Serial.begin(115200);// Sets up the serialmonitor at a baud rate of 115200
  //attachInterrupt(digitalPinToInterrupt(2), Encode, FALLING);
  tone(bzr, 1500, 300);// Makes the buzzer sound once when the code first initalizes
}

void loop() {
xJSval = analogRead(xJS);// Reads the joystick coordinates in the x direction
yJSval = analogRead(yJS);// Reads the joystick coordinates in the y direction
  
  Serial.print(xJSval);// Prints the value of the x coordinate
  Serial.print(" ");// Prints a space
  Serial.println(yJSval);// Prints the value of the y coodinates and ends the line 
  delay(300);// Dealys the code for 300 milliseconds
}
