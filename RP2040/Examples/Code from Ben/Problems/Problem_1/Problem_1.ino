/*
   Problem1
   by - J.Koerlin
   This program...

*/
#include"PicoPins2.h"

float potVal;

void setup() {
  pinSet();
  Serial.begin(9600);
  //attachInterrupt(digitalPinToInterrupt(2), Encode, FALLING);

}

void loop() {
  if(digitalRead(rPB)==0){
    for(int x=0;x<255;x++){
     potVal=analogRead(pot);
     analogWrite(rLED,1*x); 
     delay(255/potVal);
    }
    for(int x=255;x>0;x--){
     potVal=pot;
     analogWrite(rLED,1*x); 
     delay(255/potVal); 
     digitalWrite(rLED,LOW);
    }

    
    for(int x=0;x<255;x++){
     potVal=analogRead(pot);
     analogWrite(yLED,1*x); 
     Serial.println(255/potVal);
     delay(255/potVal);
    }
    for(int x=255;x>0;x--){
     potVal=pot;
     analogWrite(yLED,1*x); 
     delay(255/potVal);
     digitalWrite(yLED,LOW);
    }

    
    for(int x=0;x<255;x++){
     potVal=analogRead(pot);
     analogWrite(gLED,1*x); 
     Serial.println(255/potVal);
     delay(255/potVal);
    }
    for(int x=255;x>0;x--){
     potVal=pot;
     analogWrite(gLED,1*x); 
     delay(255/potVal);
     digitalWrite(gLED,LOW);
    }

    
    for(int x=0;x<255;x++){
     potVal=analogRead(pot);
     analogWrite(bLED,1*x); 
     Serial.println(255/potVal);
     delay(255/potVal);
    }
    for(int x=255;x>0;x--){
     potVal=pot;
     analogWrite(bLED,1*x); 
     delay(255/potVal);
     digitalWrite(bLED,LOW);
    }
    delay(10);
  }
}
