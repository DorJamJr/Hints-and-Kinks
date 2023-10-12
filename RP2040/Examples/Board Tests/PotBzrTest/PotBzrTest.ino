/*
   Switch Test
   by - J.Koerlin
   This program...

  Updated to include more in the 'Serial Monitor'
  Also shows pot value vs how the map(. . . ) function works
*/
#include"PicoPins2.h"
int potVal = 0;
void setup() {
  pinSet();
  Serial.begin(115200);
  //attachInterrupt(digitalPinToInterrupt(2), Encode, FALLING);
  tone(bzr, 1500, 300);
}

void loop() {
  potVal = analogRead(pot);
  if (digitalRead(rPB) == 0) {// only play a tone if the red button pressed for 1 second
    Serial.print("Red ");
    tone(bzr, potVal*2, 100);  // was 10
  }
  Serial.print("Pot Value= ");
  Serial.print(potVal);
  potVal = map(potVal, 0, 1023, 0, 255);  // scale the 10 bit (0-1023) analogRead value to 
  Serial.print("    Mapped Pot Value= ");
  Serial.println(potVal);               //the 8 bit(0-255)analogWrite needs.
  analogWrite(rLED, potVal);
  analogWrite(yLED, potVal);
  analogWrite(gLED, potVal);
  analogWrite(bLED, potVal);
  delay(1000);
}
