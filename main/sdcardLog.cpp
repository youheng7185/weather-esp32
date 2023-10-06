#include <Arduino.h>
#include "sdcardLog.h"
#include "FS.h"
#include "SD.h"
#include "return.h"

#define SD_CS 5

// Write to the SD card (DON'T MODIFY THIS FUNCTION)
void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);
  File file = fs.open(path, FILE_WRITE);
  if(!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

// Append data to the SD card (DON'T MODIFY THIS FUNCTION)
void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);
  File file = fs.open(path, FILE_APPEND);
  if(!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

void subMenuSDcard() {
  SD.begin(SD_CS);  
  if(!SD.begin(SD_CS)) {
    lcd.clear();
    lcd.print("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();
  if(cardType == CARD_NONE) {
    lcd.clear();
    lcd.print("No SD card attached");
    return;
  }
  lcd.clear();
  lcd.print("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    lcd.clear();
    lcd.print("ERROR - SD card initialization failed!");
    return;    // init failed
  }
  File file = SD.open("/iaq_data.txt");
  if(!file) {
    lcd.clear();
    lcd.print("File doens't exist");
    lcd.setCursor(0, 1);
    lcd.print("Creating file...");
    writeFile(SD, "/iaq_data.txt", "Time, Temp, Pressure, Humidnity, IAQ \r\n");
  }
  else {
    lcd.clear();
    lcd.print("File already exists");  
  }
  file.close();
  delay(200);
  returntoHome();
}
