/*
   Header file containing all the hardware connections of the Pedagog board

   Created 02/24/20 - JRA
   IRtx=3, IRrx=A0  3-10-20 JMK
*/

// Push Buttons
const int rPB = 5;    // Red push button connected to pin 8     (D4)
const int yPB = 7;    // Yellow push button connected to pin 7  (D5)
const int gPB = 9;    // Green push button connected to pin 5   (D7)
const int bPB = 11;    // Blue push button connected to pin 4    (D8)
const int oPB = 24;    // Blue push button connected to pin 4    (D8)

// LEDs
const int rLED = 4;   // Red LED connected to pin 11           (D11)
const int yLED = 6;   // Yellow LED connected to pin 10        (D10)
const int gLED = 8;  // Green LED connected to pin 9            (D9)
const int bLED = 10;  // Blue LED connected to pin 6             (D6)
const int oLED = 25;  // Blue LED connected to pin 13           (D13)

// IR devices
const int IRrx = 12;   // IR receive device connected to pin A0  (A0)
const int IRtx = 13;  // IR transmit device connected to pin A3  (A3)

// Rotary Encoder
const int encA = 2;  // Encoder A connected to pin A1          (A1)
const int encB = 3;   // Encoder B connected to pin 2           (D2)

// Joystick
const int xJS = A0;  // Joystick X axis potentiometer to pin A7  (A7)
const int yJS = A1;  // Joystick Y axis potentiometer to pin A6  (A6)

// Buzzer
const int bzr = 22;   // Buzzer connected to pin 12              (D12)

// Photo resistor & Pot
const int batV = A3;  // Photo resistor connected to pin A2  (A2)
const int pot = A2;  // Pot connected to pin A0  (A2)
//  NOTE: Photores is replaced with POT on Pico
//  Hardwired to A1 on Pico

// Serial Port
const int Tx = 0;  // Serial Transmit (pin 13)                  (D13)
const int Rx = 1;  // Serial Receive (pin 3)                     (D3)


int count;   // count holds the encoder variable.

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
