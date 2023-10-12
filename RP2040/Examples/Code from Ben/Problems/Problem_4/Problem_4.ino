/*
   Problem4
   by - J.Koerlin
   This program...

*/
#include"PicoPins2.h"
//creates values
int Red;
int Grn;
int Yel;
int Blu;

void setup() {
  pinSet();
  //Serial.begin(9600);
  //attachInterrupt(digitalPinToInterrupt(2), Encode, FALLING);

}

void loop() {
  /*
  Serial.print("Coordinates");
  Serial.print(" x=");
  Serial.print(analogRead(xJS));
  Serial.print(" y=");
  Serial.println(analogRead(yJS));
  delay(10);
*/
//makes values rise if joystick is moved to the corresponding location
 if(analogRead(xJS)<10){
  Red++;
  delay(50);
 }

 if(analogRead(xJS)>990){
  Grn++;
  delay(50);
 }

 if(analogRead(yJS)>990){
  Yel++;
  delay(50);
 }

 if(analogRead(yJS)<10){
  Blu++;
  delay(50);
 }
 //#################################################
 //prints values to serial monitor
 Serial.print(Red);
 Serial.print(Grn);
 Serial.print(Yel);
 Serial.println(Blu);
//##################################################
//writes values to LEDs
analogWrite(rLED,Red);
analogWrite(gLED,Grn);
analogWrite(yLED,Yel);
analogWrite(bLED,Blu);
 //#################################################
 //resets values to zero if push buttons are pressed
if(digitalRead(rPB)==0){
  Red=0;
}
if(digitalRead(yPB)==0){
  Yel=0;
}
if(digitalRead(gPB)==0){
  Grn=0;
}
if(digitalRead(bPB)==0){
  Blu=0;
}
  
}
