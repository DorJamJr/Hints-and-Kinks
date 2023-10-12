/*
   Promblem_3
   by - J.Koerlin
   This program...

*/
#include"PicoPins2.h"
float potVal;
void setup() {
  pinSet();
  //Serial.begin(9600);
  //attachInterrupt(digitalPinToInterrupt(2), Encode, FALLING);

}
//code blinks each LED five times, speed of the blink is adjustable with the pot
void loop() {
  potVal=analogRead(pot);
    for(int x=0; x<5; x++){
    digitalWrite(rLED,HIGH);
    delay(potVal);
    digitalWrite(rLED,LOW);
    delay(potVal);
    Serial.println(potVal);
  }
  potVal=analogRead(pot);
  for(int x=0; x<5; x++){
    digitalWrite(yLED,HIGH);
    delay(potVal);
    digitalWrite(yLED,LOW);
    delay(potVal);
  }
  potVal=analogRead(pot);
   for(int x=0; x<5; x++){
    digitalWrite(gLED,HIGH);
    delay(potVal);
    digitalWrite(gLED,LOW);
    delay(potVal);
  }
  potVal=analogRead(pot);
   for(int x=0; x<5; x++){
    digitalWrite(bLED,HIGH);
    delay(potVal);
    digitalWrite(bLED,LOW);
    delay(potVal);
  }


  }
