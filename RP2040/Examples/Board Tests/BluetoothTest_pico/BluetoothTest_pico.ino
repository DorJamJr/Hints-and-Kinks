/*
   Bluetooth Module Test
*/

//#include <SoftwareSerial.h>

//SoftwareSerial mySerial(3, 13); // RX, TX

#include"PicoPins2.h"

char msg;

void setup() {
  pinSet();
  Serial.begin(9600);
  Serial1.setRX(1);
  Serial1.setTX(0);
  Serial1.begin(9600);
  delay(5000);
  // mySerial.begin(9600); // Default communication rate of the Bluetooth module
  Serial1.println("Hello, world?");
}

void loop() {


  if (Serial1.available()) { // Checks whether data is comming from the serial port
    msg = Serial1.read(); // Reads the data from the serial port
    Serial.println(msg);
  }

  if (msg == 'L') {
    delay(1);   // wait for the next letter to be transmitted
     while (!Serial1.available()) {}
    msg = Serial1.read(); // Reads the data from the serial port  Switch Case needs an integer.
    Serial.print(msg);
    switch (msg) {
      case 'r':
        digitalWrite(rLED, !digitalRead(rLED)); // Toggle LED
        Serial1.println("rLED:"); // Send back, to the phone, the String "rLED:"
        break;
      case 'y':
        digitalWrite(yLED, !digitalRead(yLED)); // Toggle LED
        Serial1.println("yLED:"); // Send back, to the phone, the String "yLED:"
        break;
      case 'g':
        digitalWrite(gLED, !digitalRead(gLED)); // Toggle LED
        Serial1.println("gLED:"); // Send back, to the phone, the String "gLED:"
        break;
      case 'b':
        digitalWrite(bLED, !digitalRead(bLED)); // Toggle LED
        Serial1.println("bLED:"); // Send back, to the phone, the String "bLED:"
        break;
      default:
        Serial1.println("Duh..."); // Send back, to the phone, the String "Duh.."
        break;
    }
  }

}
