/*
This is a demo of how to use the EEPROM in the RPi Pico (and likely all other platforms)

Basics:
  EEPROM.write(adr,byte) 
  Writes as single byte at the address specified. The passed in "byte" variable is truncated to 8 bits

  EEPROM.read(adr)
  Reads and returns a single byte from the specified address

  EEPROM.put(adr,var)
  Writes any type of variable (including custom struct and arrays) to EEPROM memory, starting at address
  adr and using up space as required by the variable type.

  EEPROM.get(adr, var)
  Reads the EEPROM starting at address adr and fills in the passed in var. Also returns pointer to the variable.

  IMPORTANT: We have found (experimentally) that EEPROM.put(adr,var) and EEPROM.get(adr,var) do not work correctly 
   if the passed in var is an array that is is declared outside of the function in which the EEPROM.get() 
   or EEPROM.put() is called. (Example: you pass an array myArray into a function and then call the 
   EEPROM.get(adr, myArray) within that function.) 
   To be safe, declare the array in the same function as you use the EEPROM.get() or EEPROM.put() function.
   For EEPROM.get() using a passed in array, use the local array in the function then copy the local array to the
    passed in array.
   For EEPROM.put() using a passed in array, copy the passed in array to the local array and then use the local array
    in the function. 

  Below are three functions to demonstrate this:
  writeTest()   // loops to write single bytes to address and incrementing both value and address
  putTest()     // loops to write an int type to sequential addresses, where the loop keeps track of the needed
                //  address updates to keep data correctly separated
  putArray()    // no looping. An array of ints is written to the EEPROM in one instruction. The EEPROM.put is passed
                //  the starting address and the array name... the EEPROM.put function takes care of handling the 
                //  address incrementing
  calNow()      // writes a locally created array to EEPROM
  readCalData() // uses a passed in array as the object of a call to EEPROM.get(). Used to demonstrate the problems
                //  in uing a passed in array as the object of EEPROM.get() (see note above on EEPROM.get)

  Also included is function printEEPROM() which reads the EEPROM byte by byte and puts the result to the 
   serial monitor screen


  To be explored:
  How do the EEPROM read functions work? We have had some issues (SteveK code where we thought we were using the 
   functions correctly (as above) but results were confusing)                             

*/
//uint16_t r_calData[5];

#include <EEPROM.h>
#include "PicoPins2.h"
#include "calFunctions.h"

int myArray[3] = {276,277,278};
uint16_t myArray1[3] = {0,0,0};
//uint16_t r_calData[5];

void setup() {
  // this code will run just once
  uint16_t r_calData[5]={1,2,3,4,5};

  pinSet();
  Serial.begin(9600);
  while (!Serial);
  EEPROM.begin(512);
  delay(5000);

//  writeTest();
//  putTest();
//  putArray();
  calNow();
  printEEPROM(100,50);
  readCalData(r_calData);
  for (int i=0;i<5;i++){
  Serial.print("Returned r_calData[");Serial.print(i);Serial.print("]  ");Serial.println(r_calData[i]);
  }

}

void loop() {
  // intentionally empty loop

}

void writeTest(){
  for (int i=100; i<110; i++){
    EEPROM.write(i, 266+i);   // should write 10+i since it is uint8... verified   
  }
  committEEPROM();
}


void putTest(){
  for (int i=110; i<120; i=i+4){
  EEPROM.put(i, 276+i);
  }
  committEEPROM();
}

void putArray(){
  EEPROM.put(135,myArray);
  committEEPROM();
}

void getArray(){
  EEPROM.get(135,myArray1);
}

void printEEPROM(int startAdr, int readCount){
  for (int i=startAdr; i<startAdr+readCount;i++){
    Serial.print(i); Serial.print("  "); Serial.println(EEPROM.read(i));
  }
}

void committEEPROM(){
  if (EEPROM.commit())
    Serial.println("EEPROM calibration successfully committed");
  else
    Serial.println("ERROR! EEPROM commit failed");
}


// void readCalData(){
//   EEPROM.get(100+1, r_calData);
//   for (int i=0;i<5;i++){
//     Serial.print("r_calData[");Serial.print(i);Serial.print("]  ");Serial.println(r_calData[i]);
//   }
// }


// The following example code is from https://docs.arduino.cc/learn/built-in-libraries/eeprom#put
//  Just sitting here for now for my quick reference...

// #include <EEPROM.h>

// struct MyObject {
//   float field1;
//   byte field2;
//   char name[10];
// };

// void setup() {

//   Serial.begin(9600);
//   while (!Serial) {
//     ; // wait for serial port to connect. Needed for native USB port only
//   }

//   float f = 123.456f;  //Variable to store in EEPROM.
//   int eeAddress = 0;   //Location we want the data to be put.


//   //One simple call, with the address first and the object second.
//   EEPROM.put(eeAddress, f);

//   Serial.println("Written float data type!");

//   /** Put is designed for use with custom structures also. **/

//   //Data to store.
//   MyObject customVar = {
//     3.14f,
//     65,
//     "Working!"
//   };

//   eeAddress += sizeof(float); //Move address to the next byte after float 'f'.

//   EEPROM.put(eeAddress, customVar);
//   Serial.print("Written custom data type! \n\nView the example sketch eeprom_get to see how you can retrieve the values!");
// }

// void loop() {   /* Empty loop */ }
