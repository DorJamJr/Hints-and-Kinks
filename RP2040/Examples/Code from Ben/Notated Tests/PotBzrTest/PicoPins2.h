/*
   Header file containing all the hardware connections of the Pedagog board

   Created 02/24/20 - JRA
   IRtx=3, IRrx=A0  3-10-20 JMK
*/

// Push Buttons
const int rPB = 5;    // Red push button connected to pin 5 
const int yPB = 7;    // Yellow push button connected to pin 7
const int gPB = 9;    // Green push button connected to pin 9 
const int bPB = 11;    // Blue push button connected to pin 11 
const int oPB = 24;    // Onboard push button connected to pin 24 

// LEDs
const int rLED = 4;   // Red LED connected to pin 4 
const int yLED = 6;   // Yellow LED connected to pin 6 
const int gLED = 8;  // Green LED connected to pin 8  
const int bLED = 10;  // Blue LED connected to pin 10         
const int oLED = 25;  // Blue LED connected to pin 25 

// IR devices
const int IRrx = 12;   // IR receive device connected to pin 12
const int IRtx = 13;  // IR transmit device connected to pin 13

// Rotary Encoder
const int encA = 2;  // Encoder A connected to pin 2 
const int encB = 3;   // Encoder B connected to pin 3 

// Joystick
const int xJS = A0;  // Joystick X axis potentiometer to pin A0
const int yJS = A1;  // Joystick Y axis potentiometer to pin A1

// Buzzer
const int bzr = 22;   // Buzzer connected to pin 22

//  Pot & Battery Voltage
const int batV = A3;  // Photo resistor connected to pin A3
const int pot = A2;  // Pot connected to pin A2
//  NOTE: A3 is not available on the original Pico

// Serial Port
const int Tx = 0;  // Serial Transmit (GPIO 0)
const int Rx = 1;  // Serial Receive (GPIO 1) 


volatile int count;   // count holds the encoder variable.

void pinSet()
{
  pinMode(rLED, OUTPUT);
  pinMode(yLED, OUTPUT);
  pinMode(gLED, OUTPUT);
  pinMode(bLED, OUTPUT);
  pinMode(oLED, OUTPUT);
  pinMode(IRtx, OUTPUT);
  pinMode(bzr, OUTPUT);
  pinMode(rPB, INPUT_PULLUP);
  pinMode(yPB, INPUT_PULLUP);
  pinMode(gPB, INPUT_PULLUP);
  pinMode(bPB, INPUT_PULLUP);
  pinMode(oPB, INPUT_PULLUP);
  pinMode(IRrx, INPUT);
  pinMode(encA, INPUT_PULLUP);
  pinMode(encB, INPUT_PULLUP);
}
void Encode()  // Interrupt rputine to manage the encoders
{ // this routine is called if encB transitions from HIGH to LOW
  if (digitalRead(encB)) // Based on the state of encA we know the direction
    count++;
  else
    count--;
}
