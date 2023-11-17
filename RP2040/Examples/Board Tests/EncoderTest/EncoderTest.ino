/*
   Encoder  Test
   by - J.Koerlin
   This program tests the Rotary Encoder.
start the program, open the Serial Monitor and rotate the encoder knob
the "Encode()" function which accumulates the 'count' is in the PicoPins2.h file.

Updated to print more info in the 'Serial Monitor'
*/
#include"PicoPins2.h"
int potVal = 0;
void setup() {
  pinSet();
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(2), Encode2, FALLING); //SK: think this should be encA not '2'
  tone(bzr, 1500, 300);
}

void loop() {
  Serial.print("Counter= "); Serial.print(count);
  Serial.print("           encA= "); Serial.print(digitalRead(encA));
  Serial.print("    encB= "); Serial.println(digitalRead(encB));
  delay(2000);
}

// From the PicoPins2.h file just for reference
// const int encA = 2;  // Encoder A connected to pin A1          (A1)
// const int encB = 3;   // Encoder B connected to pin 2           (D2)

void Encode2()  // Interrupt rputine to manage the encoders
{ // this routine is called if encB transitions from HIGH to LOW
  Serial.print("Interrupt: encA= "); Serial.print(digitalRead(encA));
  Serial.print("    encB= "); Serial.println(digitalRead(encB));
  if (digitalRead(encB)) // Based on the state of encA we know the direction
    count++;
  else
    count--;
}