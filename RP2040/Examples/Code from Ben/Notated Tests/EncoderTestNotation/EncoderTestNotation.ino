/*
   Encoder  Test
   by - J.Koerlin
   This program tests the Rotary Encoder.
start the program, open the Serial Monitor and rotate the encoder knob
the "Encode()" function which accumulates the 'count' is in the PicoPins2.h file.
*/
#include"PicoPins2.h"
int potVal = 0;
void setup() {
  pinSet();// Sets up the pinSet variable 
  Serial.begin(9600);// Sets up the serial monitor at a baud rate of 9600
  attachInterrupt(digitalPinToInterrupt(2), Encode, FALLING);
  tone(bzr, 1500, 300); // Makes the buzzer sound once when the code first initalizes
}

void loop() {
  Serial.println(count);// Reads from the encoder and prints the value (count) on a new line
  delay(300);// Delays the code for 300 milliseconds
}
