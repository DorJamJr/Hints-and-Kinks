/*
   Switch Test
   by - J.Koerlin
   
   Updated by Steve Kliewer:
    2023/2/20 Created buzzer and LED test with LEDs using a 'Class'

*/
#include "PicoPins2.h"

class MyLED {  // Definition of the class (note uppercase B for class definition)
  private:
    int ledPin;
 
  public:
    MyLED(int ledID) {  // This is the constructor for the object
      ledPin = ledID;
    }

    void turnLEDon(bool on){
      digitalWrite(ledPin, on);
    }

    void blinkLED(){
      if (millis() / 100 % 2 == 0){
        turnLEDon(true);
      }
      else
        turnLEDon(false);
    }

    // This is an overload of the blinkLED method.  It allows the same name but
    // with different parameters.  It allows a custom blink speed
    void blinkLED(int howOften){
      if ((millis() / howOften) % 2 == 0)
        turnLEDon(true);
      else
        turnLEDon(false);
    }


};

// Create the 4 LED objects of type MyLED
MyLED ledRed(rLED);
MyLED ledYellow(yLED);
MyLED ledGreen(gLED);
MyLED ledBlue(bLED);

void setup() {
  pinSet();
  Serial.begin(9600);
}

void loop() {
  if (!digitalRead(rPB))
    ledRed.blinkLED();
  else
    ledRed.turnLEDon(false);
  
  if (!digitalRead(yPB))
    ledYellow.turnLEDon(true);
  else
    ledYellow.turnLEDon(false);

  if (!digitalRead(gPB))
    ledGreen.blinkLED(500);
  else
    ledGreen.turnLEDon(false);

  if (!digitalRead(bPB))
    ledBlue.turnLEDon(true);
  else
    ledBlue.turnLEDon(false);

  // original code from James:
  // digitalWrite(rLED,!digitalRead(rPB));
  // digitalWrite(yLED,!digitalRead(yPB));
  // digitalWrite(gLED,!digitalRead(gPB));
  // digitalWrite(bLED,!digitalRead(bPB));
 
  int duration = 800;

  // when testing notice that the buzzer doesn't start until you release the button
  if (digitalRead(rPB) == 0) {// only play a tone if the red button pressed
      tone(bzr, 50, duration+150); 
  } 

    if (digitalRead(yPB) == 0) {// only play a tone if the red button pressed
      tone(bzr, 110, duration+300); 
  } 

    if (digitalRead(gPB) == 0) {// only play a tone if the red button pressed
      tone(bzr, 170, duration-150);  
  } 

    if (digitalRead(bPB) == 0) {// only play a tone if the red button pressed
      tone(bzr, 230, duration-300);
  } 

}
