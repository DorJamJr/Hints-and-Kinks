#include <sys/_stdint.h>


void calNow(){
  uint16_t newRotation = 0;
  uint16_t calData[5] = {35, 270, 300, 50, 10};

  EEPROM.write(100+newRotation * 12, 200 + newRotation);
  EEPROM.put((100+newRotation * 12) + 1, calData);
  if (EEPROM.commit())
    Serial.println("EEPROM calibration successfully committed");
  else
    Serial.println("ERROR! EEPROM commit failed");

}

void readCalData(uint16_t r_calData[]){
  uint16_t r2_calData[5]={5,6,7,8,9};
  for (int i=0;i<5;i++){
    Serial.print("r_calData[");Serial.print(i);Serial.print("]  ");Serial.println(r_calData[i]);
  }

  EEPROM.get(100+1, r2_calData);

 // r_calData should not have changed (it didn't)
  for (int i=0;i<5;i++){
    Serial.print("after get: r_calData[");Serial.print(i);Serial.print("]  ");Serial.println(r_calData[i]);
  }
  // copy r2_calData (the correct read result) to the passed in r_calData array and print the result
  //  (this works)
  for (int i=0;i<5;i++){
    r_calData[i] = r2_calData[i];
  Serial.print("r_calData[");Serial.print(i);Serial.print("]  ");Serial.println(r_calData[i]);
  }
}
