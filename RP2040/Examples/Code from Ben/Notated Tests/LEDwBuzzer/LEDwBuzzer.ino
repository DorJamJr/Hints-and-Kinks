/*
   Template
   by - J.Koerlin
   This program...

*/
#include"PicoPins2.h"
int dt=1000;
int dt2=500;
int dt3=10;
void setup() {
  pinSet();
  //Serial.begin(9600);
  //attachInterrupt(digitalPinToInterrupt(2), Encode, FALLING);

}

void loop() {
  if(digitalRead(rPB)==0){//Only initiate code in loop if rPB is pressed
    digitalWrite(rLED,HIGH);//Turns on the red LED
    tone(bzr, 500, 1000);//Turns on the buzzer after rPB is released at frequency of 500Hz for one second
  }else{ //If rPB is not pressed do following code
    digitalWrite(rLED,LOW);//Turns off the red LED
    noTone(bzr);//turns off the buzzer
  }//this code will turn on the red LED if rPB is pressed and when rPB is released it will sound the buzzer

  
   if(digitalRead(yPB)==0){//Only initiate code in loop if yPB is pressed
    digitalWrite(yLED,HIGH);//Turns on the yellow LED
    tone(bzr, 200);//Turns on the buzzer at a frequency of 200Hz
    delay(dt);// delays code one second
  }else{ //If yPB is not pressed do following code
    digitalWrite(yLED,LOW);//Turns off the yellow LED
    noTone(bzr);//Turns off the buzzer 
  }//this code will turn on the yellow LED and the buzzer when the pushPutton is pressed 
  //and will keep it on for at least one second

  if(digitalRead(gPB)==0){//Only initiate code in loop if gPB is pressed
    digitalWrite(gLED,HIGH);//Turns on the green LED
    tone(bzr, 1000);//Turns on the buzzer at frequency of 1000Hz
    delay(dt);//delays code one second
    digitalWrite(gLED,LOW);//Turns off the green LED
    delay(dt2);//delays code 500ms
    digitalWrite(gLED,HIGH);//Turns on the green LED
    tone(bzr, 500);//Turns on the buzzer at frequency of 500Hz
    delay(dt);//delays code one second
  }else{ //If gPB is not pressed do following code
    digitalWrite(gLED,LOW);//Turns off the green LED
    noTone(bzr);//Turns off the buzzer 
  }//this code will turn on the green LED and the buzzer for one second turn them off and then
  // turn them back on with the buzzer at 500Hz instead of 1000Hz for one second

 if(digitalRead(bPB)==0){//Only initiate code in loop if bPB is pressed
  digitalWrite(bLED,HIGH);//turns on the blue LED
  tone(bzr,261.626);//c plays a C major scale 
  noTone(bzr);//Turns off the buzzer 
  delay(dt3);//delay of 10ms
  tone(bzr,293.665);//d
  noTone(bzr);//Turns off the buzzer 
   delay(dt3);//delay of 10ms
  tone(bzr,329.628);//e
  noTone(bzr);//Turns off the buzzer 
   delay(dt3);//delay of 10ms
  tone(bzr,349.228);//f
  noTone(bzr);//Turns off the buzzer 
   delay(dt3);//delay of 10ms
  tone(bzr,391.995);//g
  noTone(bzr);//Turns off the buzzer
   delay(dt3);//delay of 10ms 
  tone(bzr,440.000);//a
  noTone(bzr);//Turns off the buzzer
   delay(dt3);//delay of 10ms
  tone(bzr,493.883);//b
  noTone(bzr);//Turns off the buzzer
   delay(dt3);//delay of 10ms
  tone(bzr,523.251);//c
  noTone(bzr);//Turns off the buzzer
   delay(dt3);//delay of 10ms
 }else{
  digitalWrite(bLED,LOW);//Turns off the blue LED
  noTone(bzr);//Turns off the buzzer 
 }// this code turns on the blue LED and plays a C major scale when the bPB is pressed

}
