
#include <EEPROM.h>

/******************************************
                  EEPROM
*******************************************/
// 7 por causa do NULL no final da String
const short passwordMaxDigits = 7;

void setup() {
  // Essa eh a unica parte do codigo em que esse metodo deve ser chamado
  initializeEEPROM();
}

void loop() {

}

void initializeEEPROM() {
  Serial.println("####### EEPROM Initialization START #######");
  Serial.println("Clearing EEPROM...");
  char initializer[1024] = {};
  EEPROM.put(0, initializer);
  
  Serial.println("Writing default passwords...");

  setPassword(defaultPasswordActivate, 1);
  setPassword(defaultPasswordDeactivate, 2);
  setPassword(defaultPasswordPanic, 3);
  
  Serial.println("####### EEPROM Initialization END #######");

}

boolean setPassword(char* password, int passwordTypeCode) {
  short multiplier = passwordTypeCode - 1;
  int minI = passwordMaxDigits * multiplier;
  int maxI = passwordMaxDigits * multiplier + passwordMaxDigits;
  
  for (int i = minI; i < maxI; i++) {
    EEPROM.write(i, password[i - minI]);
  }
  Serial.println("####### Password written! #######");
  return true;
}

char* getPassword(int passwordTypeCode) {
  char result[passwordMaxDigits] = {};
  short resultIndex = 0;
  short multiplier = passwordTypeCode - 1;
  int minI = passwordMaxDigits * multiplier;
  int maxI = passwordMaxDigits * multiplier + passwordMaxDigits;
  
  for (int i = minI; i < maxI; i++) {
    result[resultIndex] = EEPROM.read(i);
    Serial.println(EEPROM.read(i));
    resultIndex++;
  }
  Serial.println("####### Password obtained! #######");
  return result;
}




