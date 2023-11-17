/*
   Header file containing all the hardware connections of the Pedagog board
Rev 3.3
*/

// Push Buttons
const int rPB = 8;    // Red push button connected to pin 8 
const int yPB = 7;    // Yellow push button connected to pin 7
const int gPB = A1;    // Green push button connected to pin 5
const int bPB = A0;    // Blue push button connected to pin 4

// LEDs
const int rLED = 11;   // Red LED connected to pin 11  
const int yLED = 10;   // Yellow LED connected to pin 10
const int gLED = 9;  // Green LED connected to pin 9 
const int bLED = 6;  // Blue LED connected to pin 6 
const int sLED = 13;  // Blue LED connected to pin 13 

// IR devices
const int IRrx = 4;   // IR receive device connected to pin A0 
const int IRtx = A3;  // IR transmit device connected to pin A3 

// Rotary Encoder
const int encA = 2;   // Encoder B connected to pin 2 
const int encB = 5;  // Encoder A connected to pin A1  
const int encSw = 8;  // Encoder switch connected to pin 8 

// Joystick
const int xJS = A7;  // Joystick X axis potentiometer to pin A7 
const int yJS = A6;  // Joystick Y axis potentiometer to pin A6 
const int swJS = 8;  // Joystick push switch connected to pin 8 

// Buzzer
const int bzr = 12;   // Buzzer connected to pin 12 

// Var resistor - Selected with the Jumper.
const int photoRes = A2;  // Photo resistor connected to pin A2
const int pot = A2;  // Potentiometer connected to pin A2

// Serial Port
const int Tx = 13;  // Serial Transmit (pin 13)  
const int Rx = 3;  // Serial Receive (pin 3)  


volatile int count;   // count holds the encoder variable.

void pinSet()
{
  pinMode(rLED, OUTPUT);
  pinMode(yLED, OUTPUT);
  pinMode(gLED, OUTPUT);
  pinMode(bLED, OUTPUT);
  pinMode(IRtx, OUTPUT);
  pinMode(bzr, OUTPUT);
  pinMode(rPB, INPUT_PULLUP);
  pinMode(yPB, INPUT_PULLUP);
  pinMode(gPB, INPUT_PULLUP);
  pinMode(bPB, INPUT_PULLUP);
  pinMode(IRrx, INPUT);
  pinMode(encA, INPUT_PULLUP);
  pinMode(encB, INPUT_PULLUP);
}
void Encode()  // Interrupt rputine to manage the encoders
{ // this routine is called if encB transitions from HIGH to LOW
  if (digitalRead(encB)) // Based on the state of encB we know the direction
    count++;
  else
    count--;
}
