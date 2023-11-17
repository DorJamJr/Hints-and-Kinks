/*
  Fade

  This example shows how to fade the onboard Raspberry Pi Pico LED

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Fade
*/
#include"PicoPins2.h"

//the PWM pin the LED is attached to the Pico board
int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

// the setup routine runs once when you press reset:
void setup() {
  // declare pin to be an output:
  //pinMode(oLED, OUTPUT);
  //instead... we will call pinSet from PicoPins2.h
    pinSet();
}

// the loop routine runs over and over again forever:
void loop() {
  // set the brightness
  analogWrite(oLED, brightness);
analogWrite(bLED, brightness);

  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade:
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }
  // wait for 30 milliseconds to see the dimming effect
  delay(30);
}